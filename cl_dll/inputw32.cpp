//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

// in_win.c -- windows 95 mouse and joystick code
// 02/21/97 JCB Added extended DirectInput code to support external controllers.

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"
#include "camera.h"
#include "in_defs.h"
#include "../public/keydefs.h"
#include "view.h"
#include "Exports.h"

#include "PlatformHeaders.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_gamecontroller.h>

void IN_ResetMouse();

#define MOUSE_BUTTON_COUNT 5

// Set this to 1 to show mouse cursor.  Experimental
bool g_iVisibleMouse = false;

extern cl_enginefunc_t gEngfuncs;
extern bool iMouseInUse;

extern kbutton_t in_strafe;
extern kbutton_t in_mlook;
extern kbutton_t in_speed;
extern kbutton_t in_jlook;

extern cvar_t* m_pitch;
extern cvar_t* m_yaw;
extern cvar_t* m_forward;
extern cvar_t* m_side;

extern cvar_t* lookstrafe;
extern cvar_t* lookspring;
extern cvar_t* cl_pitchdown;
extern cvar_t* cl_pitchup;
extern cvar_t* cl_yawspeed;
extern cvar_t* cl_sidespeed;
extern cvar_t* cl_forwardspeed;
extern cvar_t* cl_pitchspeed;
extern cvar_t* cl_movespeedkey;


static cvar_t* m_rawinput = nullptr;

static bool IN_UseRawInput()
{
	// a1ba: m_rawinput 1 is SDL input on Windows
	// Linux only has SDL input, so return true here
	return m_rawinput ? m_rawinput->value != 0 : true;
}

static SDL_bool mouseRelative = SDL_TRUE;

static void IN_SetMouseRelative(bool enable)
{
	const SDL_bool value = enable ? SDL_TRUE : SDL_FALSE;

	SDL_SetRelativeMouseMode(value);
	mouseRelative = value;
}

static bool m_bMouseThread = false;

// mouse variables
cvar_t* m_filter;
cvar_t* sensitivity;

// Custom mouse acceleration (0 disable, 1 to enable, 2 enable with separate yaw/pitch rescale)
static cvar_t* m_customaccel;
//Formula: mousesensitivity = ( rawmousedelta^m_customaccel_exponent ) * m_customaccel_scale + sensitivity
// If mode is 2, then x and y sensitivity are scaled by m_pitch and m_yaw respectively.
// Custom mouse acceleration value.
static cvar_t* m_customaccel_scale;
//Max mouse move scale factor, 0 for no limit
static cvar_t* m_customaccel_max;
//Mouse move is raised to this power before being scaled by scale factor
static cvar_t* m_customaccel_exponent;

// if threaded mouse is enabled then the time to sleep between polls
static cvar_t* m_mousethread_sleep;

int mouse_buttons;
int mouse_oldbuttonstate;
int old_mouse_x, old_mouse_y, mx_accum, my_accum;
float mouse_x, mouse_y;

static bool restore_spi;
static int originalmouseparms[3], newmouseparms[3] = {0, 0, 1};
static bool mouseactive = false;
bool mouseinitialized;
static bool mouseparmsvalid;
static bool mouseshowtoggle = true;

// joystick defines and variables
// where should defines be moved?
#define JOY_ABSOLUTE_AXIS 0x00000000 // control like a joystick
#define JOY_RELATIVE_AXIS 0x00000010 // control like a mouse, spinner, trackball
#define JOY_MAX_AXES 6				 // X, Y, Z, R, U, V
#define JOY_AXIS_X 0
#define JOY_AXIS_Y 1
#define JOY_AXIS_Z 2
#define JOY_AXIS_R 3
#define JOY_AXIS_U 4
#define JOY_AXIS_V 5

enum _ControlList
{
	AxisNada = 0,
	AxisForward,
	AxisLook,
	AxisSide,
	AxisTurn
};



std::uint32_t dwAxisMap[JOY_MAX_AXES];
std::uint32_t dwControlMap[JOY_MAX_AXES];
int pdwRawValue[JOY_MAX_AXES];
std::uint32_t joy_oldbuttonstate, joy_oldpovstate;

int joy_id;
std::uint32_t joy_numbuttons;

SDL_GameController* s_pJoystick = NULL;

// none of these cvars are saved over a session
// this means that advanced controller configuration needs to be executed
// each time.  this avoids any problems with getting back to a default usage
// or when changing from one controller to another.  this way at least something
// works.
cvar_t* in_joystick;
cvar_t* joy_name;
cvar_t* joy_advanced;
cvar_t* joy_advaxisx;
cvar_t* joy_advaxisy;
cvar_t* joy_advaxisz;
cvar_t* joy_advaxisr;
cvar_t* joy_advaxisu;
cvar_t* joy_advaxisv;
cvar_t* joy_supported;
cvar_t* joy_forwardthreshold;
cvar_t* joy_sidethreshold;
cvar_t* joy_pitchthreshold;
cvar_t* joy_yawthreshold;
cvar_t* joy_forwardsensitivity;
cvar_t* joy_sidesensitivity;
cvar_t* joy_pitchsensitivity;
cvar_t* joy_yawsensitivity;
cvar_t* joy_wwhack1;
cvar_t* joy_wwhack2;

bool joy_avail = false, joy_advancedinit, joy_haspov;

/*
===========
Force_CenterView_f
===========
*/
void Force_CenterView_f()
{
	Vector viewangles;

	if (!iMouseInUse)
	{
		gEngfuncs.GetViewAngles((float*)viewangles);
		viewangles[PITCH] = 0;
		gEngfuncs.SetViewAngles((float*)viewangles);
	}
}

#ifdef WIN32
struct MouseThread
{
	std::thread Thread;
	std::mutex Mutex;
	std::condition_variable Condition;
	bool QuittingTime = false;
};

MouseThread s_MouseThread;

std::atomic<Point> s_mouseDelta;
std::atomic<Point> current_pos;
std::atomic<Point> old_mouse_pos;

Point GetMousePosition()
{
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);

	return {
		static_cast<int>(mouse_pos.x),
		static_cast<int>(mouse_pos.y)};
}

void MousePos_ThreadFunction()
{
	while (true)
	{
		{
			std::unique_lock lock{s_MouseThread.Mutex};

			//TODO: accessing the cvar value is a race condition
			if (s_MouseThread.Condition.wait_for(
					lock,
					std::chrono::milliseconds{(int)m_mousethread_sleep->value},
					[]()
					{ return s_MouseThread.QuittingTime; }))
			{
				break;
			}
		}

		if (mouseactive)
		{
			const auto mouse_pos = GetMousePosition();

			const auto oldPos = old_mouse_pos.load();
			const auto delta = s_mouseDelta.load();

			const Point pos{
				mouse_pos.x - oldPos.x + delta.x,
				mouse_pos.y - oldPos.y + delta.y};

			old_mouse_pos = mouse_pos;
			s_mouseDelta = pos;
		}
	}
}
#endif

/*
===========
IN_ActivateMouse
===========
*/
void DLLEXPORT IN_ActivateMouse()
{
	if (mouseinitialized)
	{
#ifdef WIN32
		if (mouseparmsvalid)
			restore_spi = SystemParametersInfo(SPI_SETMOUSE, 0, newmouseparms, 0) != FALSE;

#endif
		mouseactive = true;
	}

	if (g_iVisibleMouse || !IN_UseRawInput())
	{
		IN_SetMouseRelative(false);
	}
	else
	{
		IN_SetMouseRelative(true);
	}

	// Clear out accumulated mouse input from main menu movement.
	IN_ResetMouse();
}


/*
===========
IN_DeactivateMouse
===========
*/
void DLLEXPORT IN_DeactivateMouse()
{
	if (mouseinitialized)
	{
#ifdef WIN32
		if (restore_spi)
			SystemParametersInfo(SPI_SETMOUSE, 0, originalmouseparms, 0);

#endif

		mouseactive = false;
	}

	IN_SetMouseRelative(false);
}

/*
===========
IN_StartupMouse
===========
*/
void IN_StartupMouse()
{
	if (0 != gEngfuncs.CheckParm("-nomouse", NULL))
		return;

	mouseinitialized = true;
#ifdef WIN32
	mouseparmsvalid = SystemParametersInfo(SPI_GETMOUSE, 0, originalmouseparms, 0) != FALSE;

	if (mouseparmsvalid)
	{
		if (0 != gEngfuncs.CheckParm("-noforcemspd", NULL))
			newmouseparms[2] = originalmouseparms[2];

		if (0 != gEngfuncs.CheckParm("-noforcemaccel", NULL))
		{
			newmouseparms[0] = originalmouseparms[0];
			newmouseparms[1] = originalmouseparms[1];
		}

		if (0 != gEngfuncs.CheckParm("-noforcemparms", NULL))
		{
			newmouseparms[0] = originalmouseparms[0];
			newmouseparms[1] = originalmouseparms[1];
			newmouseparms[2] = originalmouseparms[2];
		}
	}
#endif

	mouse_buttons = MOUSE_BUTTON_COUNT;
}

/*
===========
IN_Shutdown
===========
*/
void IN_Shutdown()
{
	IN_DeactivateMouse();

#ifdef WIN32
	if (s_MouseThread.Thread.joinable())
	{
		//Mouse thread is active, signal it to quit and wait.
		{
			std::lock_guard guard{s_MouseThread.Mutex};
			s_MouseThread.QuittingTime = true;
		}

		s_MouseThread.Condition.notify_one();
		s_MouseThread.Thread.join();
	}
#endif
}

/*
===========
IN_GetMousePos

Ask for mouse position from engine
===========
*/
void IN_GetMousePos(int* mx, int* my)
{
	gEngfuncs.GetMousePosition(mx, my);
}

/*
===========
IN_GetMouseSensitivity

Get mouse sensitivity with sanitization
===========
*/
float IN_GetMouseSensitivity()
{
	// Absurdly high sensitivity values can cause the game to hang, so clamp
	if (sensitivity->value > 10000.0)
	{
		gEngfuncs.Cvar_SetValue("sensitivity", 10000.0);
	}
	else if (sensitivity->value < 0.01)
	{
		gEngfuncs.Cvar_SetValue("sensitivity", 0.01);
	}

	return sensitivity->value;
}

/*
===========
IN_ResetMouse

FIXME: Call through to engine?
===========
*/
void IN_ResetMouse()
{
	// no work to do in SDL
#ifdef WIN32
	if (!IN_UseRawInput() && mouseactive && gEngfuncs.GetWindowCenterX && gEngfuncs.GetWindowCenterY)
	{
		SetCursorPos(gEngfuncs.GetWindowCenterX(), gEngfuncs.GetWindowCenterY());

		const Point center{gEngfuncs.GetWindowCenterX(), gEngfuncs.GetWindowCenterY()};
		old_mouse_pos = center;
	}
#endif
}

/*
===========
IN_ResetRelativeMouseState
===========
*/
void IN_ResetRelativeMouseState(void)
{
	if (IN_UseRawInput())
	{
		SDL_PumpEvents();
		int deltaX, deltaY;
		SDL_GetRelativeMouseState(&deltaX, &deltaY);
	}
}

/*
===========
IN_MouseEvent
===========
*/
void DLLEXPORT IN_MouseEvent(int mstate)
{
	int i;

	if (iMouseInUse || g_iVisibleMouse)
		return;

	// perform button actions
	for (i = 0; i < mouse_buttons; i++)
	{
		if ((mstate & (1 << i)) != 0 &&
			(mouse_oldbuttonstate & (1 << i)) == 0)
		{
			gEngfuncs.Key_Event(K_MOUSE1 + i, 1);
		}

		if ((mstate & (1 << i)) == 0 &&
			(mouse_oldbuttonstate & (1 << i)) != 0)
		{
			gEngfuncs.Key_Event(K_MOUSE1 + i, 0);
		}
	}

	mouse_oldbuttonstate = mstate;
}

//-----------------------------------------------------------------------------
// Purpose: Allows modulation of mouse scaling/senstivity value and application
//  of custom algorithms.
// Input  : *x -
//			*y -
//-----------------------------------------------------------------------------
void IN_ScaleMouse(float* x, float* y)
{
	float mx = *x;
	float my = *y;

	// This is the default sensitivity
	float mouse_senstivity = (gHUD.GetSensitivity() != 0) ? gHUD.GetSensitivity() : IN_GetMouseSensitivity();

	// Using special accleration values
	if (m_customaccel->value != 0)
	{
		float raw_mouse_movement_distance = sqrt(mx * mx + my * my);
		float acceleration_scale = m_customaccel_scale->value;
		float accelerated_sensitivity_max = m_customaccel_max->value;
		float accelerated_sensitivity_exponent = m_customaccel_exponent->value;
		float accelerated_sensitivity = ((float)pow(raw_mouse_movement_distance, accelerated_sensitivity_exponent) * acceleration_scale + mouse_senstivity);

		if (accelerated_sensitivity_max > 0.0001f &&
			accelerated_sensitivity > accelerated_sensitivity_max)
		{
			accelerated_sensitivity = accelerated_sensitivity_max;
		}

		*x *= accelerated_sensitivity;
		*y *= accelerated_sensitivity;

		// Further re-scale by yaw and pitch magnitude if user requests alternate mode 2
		// This means that they will need to up their value for m_customaccel_scale greatly (>40x) since m_pitch/yaw default
		//  to 0.022
		if (m_customaccel->value == 2)
		{
			*x *= m_yaw->value;
			*y *= m_pitch->value;
		}
	}
	else
	{
		// Just apply the default
		*x *= mouse_senstivity;
		*y *= mouse_senstivity;
	}
}

/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove(float frametime, usercmd_t* cmd)
{
	Point pos;
	Vector viewangles;

	gEngfuncs.GetViewAngles((float*)viewangles);

	if ((in_mlook.state & 1) != 0)
	{
		V_StopPitchDrift();
	}

	//jjb - this disbles normal mouse control if the user is trying to
	//      move the camera, or if the mouse cursor is visible or if we're in intermission
	if (!iMouseInUse && !gHUD.m_iIntermission && !g_iVisibleMouse)
	{
		int deltaX, deltaY;
#ifdef WIN32
		if (!IN_UseRawInput())
		{
			if (m_bMouseThread)
			{
				current_pos = s_mouseDelta.load();
				s_mouseDelta = Point{};
			}
			else
			{
				current_pos = GetMousePosition();
			}
		}
		else
#endif
		{
			SDL_GetRelativeMouseState(&deltaX, &deltaY);
#ifdef WIN32
			current_pos = {deltaX, deltaY};
#endif
		}

#ifdef WIN32
		if (!IN_UseRawInput())
		{
			pos = current_pos.load();

			if (!m_bMouseThread)
			{
				pos.x = pos.x - gEngfuncs.GetWindowCenterX() + mx_accum;
				pos.y = pos.y - gEngfuncs.GetWindowCenterY() + my_accum;
			}
		}
		else
#endif
		{
			pos.x = deltaX + mx_accum;
			pos.y = deltaY + my_accum;
		}

		mx_accum = 0;
		my_accum = 0;

		if (m_filter && 0 != m_filter->value)
		{
			mouse_x = (pos.x + old_mouse_x) * 0.5;
			mouse_y = (pos.y + old_mouse_y) * 0.5;
		}
		else
		{
			mouse_x = pos.x;
			mouse_y = pos.y;
		}

		old_mouse_x = pos.x;
		old_mouse_y = pos.y;

		// Apply custom mouse scaling/acceleration
		IN_ScaleMouse(&mouse_x, &mouse_y);

		// add mouse X/Y movement to cmd
		if ((in_strafe.state & 1) != 0 || (0 != lookstrafe->value && (in_mlook.state & 1) != 0))
			cmd->sidemove += m_side->value * mouse_x;
		else
			viewangles[YAW] -= m_yaw->value * mouse_x;

		if ((in_mlook.state & 1) != 0 && (in_strafe.state & 1) == 0)
		{
			viewangles[PITCH] += m_pitch->value * mouse_y;
			if (viewangles[PITCH] > cl_pitchdown->value)
				viewangles[PITCH] = cl_pitchdown->value;
			if (viewangles[PITCH] < -cl_pitchup->value)
				viewangles[PITCH] = -cl_pitchup->value;
		}
		else
		{
			if ((in_strafe.state & 1) != 0 && 0 != gEngfuncs.IsNoClipping())
			{
				cmd->upmove -= m_forward->value * mouse_y;
			}
			else
			{
				cmd->forwardmove -= m_forward->value * mouse_y;
			}
		}

		// if the mouse has moved, force it to the center, so there's room to move
		if (0 != pos.x || 0 != pos.y)
		{
			IN_ResetMouse();
		}
	}

	gEngfuncs.SetViewAngles((float*)viewangles);

	if ((!IN_UseRawInput() && SDL_FALSE != mouseRelative) || g_iVisibleMouse)
	{
		IN_SetMouseRelative(false);
	}
	else if (IN_UseRawInput() && SDL_FALSE == mouseRelative)
	{
		IN_SetMouseRelative(true);
	}

	/*
//#define TRACE_TEST
#if defined( TRACE_TEST )
	{
		int mx, my;
		void V_Move( int mx, int my );
		IN_GetMousePos( &mx, &my );
		V_Move( mx, my );
	}
#endif
*/
}

/*
===========
IN_Accumulate
===========
*/
void DLLEXPORT IN_Accumulate()
{
	//only accumulate mouse if we are not moving the camera with the mouse
	if (!iMouseInUse && !g_iVisibleMouse)
	{
		if (mouseactive)
		{
#ifdef WIN32
			if (!IN_UseRawInput())
			{
				if (!m_bMouseThread)
				{
					const auto pos = GetMousePosition();
					current_pos = pos;

					mx_accum += pos.x - gEngfuncs.GetWindowCenterX();
					my_accum += pos.y - gEngfuncs.GetWindowCenterY();
				}
			}
			else
#endif
			{
				int deltaX, deltaY;
				SDL_GetRelativeMouseState(&deltaX, &deltaY);
				mx_accum += deltaX;
				my_accum += deltaY;
			}
			// force the mouse to the center, so there's room to move
			IN_ResetMouse();
		}
	}
}

/*
===================
IN_ClearStates
===================
*/
void DLLEXPORT IN_ClearStates()
{
	if (!mouseactive)
		return;

	mx_accum = 0;
	my_accum = 0;
	mouse_oldbuttonstate = 0;
}

/* 
=============== 
IN_StartupJoystick 
=============== 
*/
void IN_StartupJoystick()
{
	// abort startup if user requests no joystick
	if (0 != gEngfuncs.CheckParm("-nojoy", NULL))
		return;

	static float flLastCheck = 0.0f;
	if (flLastCheck > 0.0f && (gEngfuncs.GetAbsoluteTime() - flLastCheck) < 1.0f)
		return;

	//gEngfuncs.Con_Printf("IN_StartupJoystick, %f\n", flLastCheck);

	flLastCheck = gEngfuncs.GetAbsoluteTime();

	int nJoysticks = SDL_NumJoysticks();
	if (nJoysticks > 0)
	{
		if (s_pJoystick == NULL)
		{
			for (int i = 0; i < nJoysticks; i++)
			{
				if (SDL_FALSE != SDL_IsGameController(i))
				{
					s_pJoystick = SDL_GameControllerOpen(i);
					if (s_pJoystick)
					{
						// save the joystick's number of buttons and POV status
						joy_numbuttons = SDL_CONTROLLER_BUTTON_MAX;
						joy_haspov = false;

						// old button and POV states default to no buttons pressed
						joy_oldbuttonstate = joy_oldpovstate = 0;

						// mark the joystick as available and advanced initialization not completed
						// this is needed as cvars are not available during initialization
						gEngfuncs.Con_Printf("joystick found %s\n\n", SDL_GameControllerName(s_pJoystick));
						joy_avail = true;
						joy_advancedinit = false;
						break;
					}
				}
			}
		}
	}
	else
	{
		if (s_pJoystick)
			SDL_GameControllerClose(s_pJoystick);

		s_pJoystick = NULL;
		if (joy_avail)
		{
			joy_avail = 0;
			gEngfuncs.Con_DPrintf("joystick not found -- driver not present\n\n");
		}
	}
}


int RawValuePointer(int axis)
{
	switch (axis)
	{
	default:
	case JOY_AXIS_X:
		return SDL_GameControllerGetAxis(s_pJoystick, SDL_CONTROLLER_AXIS_LEFTX);
	case JOY_AXIS_Y:
		return SDL_GameControllerGetAxis(s_pJoystick, SDL_CONTROLLER_AXIS_LEFTY);
	case JOY_AXIS_Z:
		return SDL_GameControllerGetAxis(s_pJoystick, SDL_CONTROLLER_AXIS_RIGHTX);
	case JOY_AXIS_R:
		return SDL_GameControllerGetAxis(s_pJoystick, SDL_CONTROLLER_AXIS_RIGHTY);
	}
}

/*
===========
Joy_AdvancedUpdate_f
===========
*/
void Joy_AdvancedUpdate_f()
{

	// called once by IN_ReadJoystick and by user whenever an update is needed
	// cvars are now available
	int i;
	std::uint32_t dwTemp;

	// initialize all the maps
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		dwAxisMap[i] = AxisNada;
		dwControlMap[i] = JOY_ABSOLUTE_AXIS;
		pdwRawValue[i] = RawValuePointer(i);
	}

	if (joy_advanced->value == 0.0)
	{
		// default joystick initialization
		// 2 axes only with joystick control
		dwAxisMap[JOY_AXIS_X] = AxisTurn;
		// dwControlMap[JOY_AXIS_X] = JOY_ABSOLUTE_AXIS;
		dwAxisMap[JOY_AXIS_Y] = AxisForward;
		// dwControlMap[JOY_AXIS_Y] = JOY_ABSOLUTE_AXIS;
	}
	else
	{
		if (strcmp(joy_name->string, "joystick") != 0)
		{
			// notify user of advanced controller
			gEngfuncs.Con_Printf("\n%s configured\n\n", joy_name->string);
		}

		// advanced initialization here
		// data supplied by user via joy_axisn cvars
		dwTemp = (std::uint32_t)joy_advaxisx->value;
		dwAxisMap[JOY_AXIS_X] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_X] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (std::uint32_t)joy_advaxisy->value;
		dwAxisMap[JOY_AXIS_Y] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Y] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (std::uint32_t)joy_advaxisz->value;
		dwAxisMap[JOY_AXIS_Z] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Z] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (std::uint32_t)joy_advaxisr->value;
		dwAxisMap[JOY_AXIS_R] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_R] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (std::uint32_t)joy_advaxisu->value;
		dwAxisMap[JOY_AXIS_U] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_U] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (std::uint32_t)joy_advaxisv->value;
		dwAxisMap[JOY_AXIS_V] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_V] = dwTemp & JOY_RELATIVE_AXIS;
	}
}


/*
===========
IN_Commands
===========
*/
void IN_Commands()
{
	int i, key_index;

	if (!joy_avail)
	{
		return;
	}

	std::uint32_t buttonstate, povstate;

	// loop through the joystick buttons
	// key a joystick event or auxillary event for higher number buttons for each state change
	buttonstate = 0;
	for (i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		if (0 != SDL_GameControllerGetButton(s_pJoystick, (SDL_GameControllerButton)i))
		{
			buttonstate |= 1 << i;
		}
	}

	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		pdwRawValue[i] = RawValuePointer(i);
	}

	for (i = 0; i < (int)joy_numbuttons; i++)
	{
		if ((buttonstate & (1 << i)) != 0 && (joy_oldbuttonstate & (1 << i)) == 0)
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			gEngfuncs.Key_Event(key_index + i, 1);
			//gEngfuncs.Con_Printf("Button %d pressed\n", i);
		}

		if ((buttonstate & (1 << i)) == 0 && (joy_oldbuttonstate & (1 << i)) != 0)
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			gEngfuncs.Key_Event(key_index + i, 0);
			//gEngfuncs.Con_Printf("Button %d released\n", i);
		}
	}
	joy_oldbuttonstate = buttonstate;

	if (joy_haspov)
	{
		// convert POV information into 4 bits of state information
		// this avoids any potential problems related to moving from one
		// direction to another without going through the center position
		povstate = 0;
		// determine which bits have changed and key an auxillary event for each change
		for (i = 0; i < 4; i++)
		{
			if ((povstate & (1 << i)) != 0 && (joy_oldpovstate & (1 << i)) == 0)
			{
				gEngfuncs.Key_Event(K_AUX29 + i, 1);
			}

			if ((povstate & (1 << i)) == 0 && (joy_oldpovstate & (1 << i)) != 0)
			{
				gEngfuncs.Key_Event(K_AUX29 + i, 0);
			}
		}
		joy_oldpovstate = povstate;
	}
}


/* 
=============== 
IN_ReadJoystick
=============== 
*/
bool IN_ReadJoystick()
{
	SDL_JoystickUpdate();
	return true;
}


/*
===========
IN_JoyMove
===========
*/
void IN_JoyMove(float frametime, usercmd_t* cmd)
{
	float speed, aspeed;
	float fAxisValue, fTemp;
	int i;
	Vector viewangles;

	gEngfuncs.GetViewAngles((float*)viewangles);


	// complete initialization if first time in
	// this is needed as cvars are not available at initialization time
	if (!joy_advancedinit)
	{
		Joy_AdvancedUpdate_f();
		joy_advancedinit = true;
	}

	// re-scan for joystick presence
	IN_StartupJoystick();

	// verify joystick is available and that the user wants to use it
	if (!joy_avail || 0 == in_joystick->value)
	{
		return;
	}

	// collect the joystick data, if possible
	if (!IN_ReadJoystick())
	{
		return;
	}

	if ((in_speed.state & 1) != 0)
		speed = cl_movespeedkey->value;
	else
		speed = 1;

	aspeed = speed * frametime;

	// loop through the axes
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		// get the floating point zero-centered, potentially-inverted data for the current axis
		fAxisValue = (float)pdwRawValue[i];

		if (joy_wwhack2->value != 0.0)
		{
			if (dwAxisMap[i] == AxisTurn)
			{
				// this is a special formula for the Logitech WingMan Warrior
				// y=ax^b; where a = 300 and b = 1.3
				// also x values are in increments of 800 (so this is factored out)
				// then bounds check result to level out excessively high spin rates
				fTemp = 300.0 * pow(fabs(fAxisValue) / 800.0, 1.3);
				if (fTemp > 14000.0)
					fTemp = 14000.0;
				// restore direction information
				fAxisValue = (fAxisValue > 0.0) ? fTemp : -fTemp;
			}
		}

		// convert range from -32768..32767 to -1..1
		fAxisValue /= 32768.0;

		switch (dwAxisMap[i])
		{
		case AxisForward:
			if ((joy_advanced->value == 0.0) && (in_jlook.state & 1) != 0)
			{
				// user wants forward control to become look control
				if (fabs(fAxisValue) > joy_pitchthreshold->value)
				{
					// if mouse invert is on, invert the joystick pitch value
					// only absolute control support here (joy_advanced is 0)
					if (m_pitch->value < 0.0)
					{
						viewangles[PITCH] -= (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					else
					{
						viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					V_StopPitchDrift();
				}
				else
				{
					// no pitch movement
					// disable pitch return-to-center unless requested by user
					// *** this code can be removed when the lookspring bug is fixed
					// *** the bug always has the lookspring feature on
					if (lookspring->value == 0.0)
					{
						V_StopPitchDrift();
					}
				}
			}
			else
			{
				// user wants forward control to be forward control
				if (fabs(fAxisValue) > joy_forwardthreshold->value)
				{
					cmd->forwardmove += (fAxisValue * joy_forwardsensitivity->value) * speed * cl_forwardspeed->value;
				}
			}
			break;

		case AxisSide:
			if (fabs(fAxisValue) > joy_sidethreshold->value)
			{
				cmd->sidemove += (fAxisValue * joy_sidesensitivity->value) * speed * cl_sidespeed->value;
			}
			break;

		case AxisTurn:
			if ((in_strafe.state & 1) != 0 || (0 != lookstrafe->value && (in_jlook.state & 1) != 0))
			{
				// user wants turn control to become side control
				if (fabs(fAxisValue) > joy_sidethreshold->value)
				{
					cmd->sidemove -= (fAxisValue * joy_sidesensitivity->value) * speed * cl_sidespeed->value;
				}
			}
			else
			{
				// user wants turn control to be turn control
				if (fabs(fAxisValue) > joy_yawthreshold->value)
				{
					if (dwControlMap[i] == JOY_ABSOLUTE_AXIS)
					{
						viewangles[YAW] += (fAxisValue * joy_yawsensitivity->value) * aspeed * cl_yawspeed->value;
					}
					else
					{
						viewangles[YAW] += (fAxisValue * joy_yawsensitivity->value) * speed * 180.0;
					}
				}
			}
			break;

		case AxisLook:
			if ((in_jlook.state & 1) != 0)
			{
				if (fabs(fAxisValue) > joy_pitchthreshold->value)
				{
					// pitch movement detected and pitch movement desired by user
					if (dwControlMap[i] == JOY_ABSOLUTE_AXIS)
					{
						viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					else
					{
						viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * speed * 180.0;
					}
					V_StopPitchDrift();
				}
				else
				{
					// no pitch movement
					// disable pitch return-to-center unless requested by user
					// *** this code can be removed when the lookspring bug is fixed
					// *** the bug always has the lookspring feature on
					if (lookspring->value == 0.0)
					{
						V_StopPitchDrift();
					}
				}
			}
			break;

		default:
			break;
		}
	}

	// bounds check pitch
	if (viewangles[PITCH] > cl_pitchdown->value)
		viewangles[PITCH] = cl_pitchdown->value;
	if (viewangles[PITCH] < -cl_pitchup->value)
		viewangles[PITCH] = -cl_pitchup->value;

	gEngfuncs.SetViewAngles((float*)viewangles);
}

/*
===========
IN_Move
===========
*/
void IN_Move(float frametime, usercmd_t* cmd)
{
	if (!iMouseInUse && mouseactive)
	{
		IN_MouseMove(frametime, cmd);
	}

	IN_JoyMove(frametime, cmd);
}

/*
===========
IN_Init
===========
*/
void IN_Init()
{
	m_filter = gEngfuncs.pfnRegisterVariable("m_filter", "0", FCVAR_ARCHIVE);
	sensitivity = gEngfuncs.pfnRegisterVariable("sensitivity", "3", FCVAR_ARCHIVE | FCVAR_FILTERSTUFFTEXT); // user mouse sensitivity setting.

	in_joystick = gEngfuncs.pfnRegisterVariable("joystick", "0", FCVAR_ARCHIVE);
	joy_name = gEngfuncs.pfnRegisterVariable("joyname", "joystick", 0);
	joy_advanced = gEngfuncs.pfnRegisterVariable("joyadvanced", "0", 0);
	joy_advaxisx = gEngfuncs.pfnRegisterVariable("joyadvaxisx", "0", 0);
	joy_advaxisy = gEngfuncs.pfnRegisterVariable("joyadvaxisy", "0", 0);
	joy_advaxisz = gEngfuncs.pfnRegisterVariable("joyadvaxisz", "0", 0);
	joy_advaxisr = gEngfuncs.pfnRegisterVariable("joyadvaxisr", "0", 0);
	joy_advaxisu = gEngfuncs.pfnRegisterVariable("joyadvaxisu", "0", 0);
	joy_advaxisv = gEngfuncs.pfnRegisterVariable("joyadvaxisv", "0", 0);
	joy_supported = gEngfuncs.pfnRegisterVariable("joysupported", "1", 0);
	joy_forwardthreshold = gEngfuncs.pfnRegisterVariable("joyforwardthreshold", "0.15", 0);
	joy_sidethreshold = gEngfuncs.pfnRegisterVariable("joysidethreshold", "0.15", 0);
	joy_pitchthreshold = gEngfuncs.pfnRegisterVariable("joypitchthreshold", "0.15", 0);
	joy_yawthreshold = gEngfuncs.pfnRegisterVariable("joyyawthreshold", "0.15", 0);
	joy_forwardsensitivity = gEngfuncs.pfnRegisterVariable("joyforwardsensitivity", "-1.0", 0);
	joy_sidesensitivity = gEngfuncs.pfnRegisterVariable("joysidesensitivity", "-1.0", 0);
	joy_pitchsensitivity = gEngfuncs.pfnRegisterVariable("joypitchsensitivity", "1.0", 0);
	joy_yawsensitivity = gEngfuncs.pfnRegisterVariable("joyyawsensitivity", "-1.0", 0);
	joy_wwhack1 = gEngfuncs.pfnRegisterVariable("joywwhack1", "0.0", 0);
	joy_wwhack2 = gEngfuncs.pfnRegisterVariable("joywwhack2", "0.0", 0);

	m_customaccel = gEngfuncs.pfnRegisterVariable("m_customaccel", "0", FCVAR_ARCHIVE);
	m_customaccel_scale = gEngfuncs.pfnRegisterVariable("m_customaccel_scale", "0.04", FCVAR_ARCHIVE);
	m_customaccel_max = gEngfuncs.pfnRegisterVariable("m_customaccel_max", "0", FCVAR_ARCHIVE);
	m_customaccel_exponent = gEngfuncs.pfnRegisterVariable("m_customaccel_exponent", "1", FCVAR_ARCHIVE);

#ifdef WIN32
	m_rawinput = gEngfuncs.pfnGetCvarPointer("m_rawinput");
	m_bMouseThread = gEngfuncs.CheckParm("-mousethread", NULL) != NULL;
	m_mousethread_sleep = gEngfuncs.pfnRegisterVariable("m_mousethread_sleep", "10", FCVAR_ARCHIVE);

	if (!IN_UseRawInput() && m_bMouseThread && m_mousethread_sleep)
	{
		s_mouseDelta = Point{};

		s_MouseThread.Thread = std::thread{&MousePos_ThreadFunction};
	}
#endif

	gEngfuncs.pfnAddCommand("force_centerview", Force_CenterView_f);
	gEngfuncs.pfnAddCommand("joyadvancedupdate", Joy_AdvancedUpdate_f);

	IN_StartupMouse();
	IN_StartupJoystick();
}
