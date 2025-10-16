/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// cl_util.h
//

#pragma once

#include "cvardef.h"

#include "Platform.h"

// Macros to hook function calls into the HUD object
#define HOOK_MESSAGE(x) gEngfuncs.pfnHookUserMsg(#x, __MsgFunc_##x);

#define DECLARE_MESSAGE(y, x)                                     \
	int __MsgFunc_##x(const char* pszName, int iSize, void* pbuf) \
	{                                                             \
		return gHUD.y.MsgFunc_##x(pszName, iSize, pbuf);          \
	}


#define HOOK_COMMAND(x, y) gEngfuncs.pfnAddCommand(x, __CmdFunc_##y);
#define DECLARE_COMMAND(y, x) \
	void __CmdFunc_##x()      \
	{                         \
		gHUD.y.UserCmd_##x(); \
	}

inline float CVAR_GET_FLOAT(const char* x) { return gEngfuncs.pfnGetCvarFloat((char*)x); }
inline const char* CVAR_GET_STRING(const char* x) { return gEngfuncs.pfnGetCvarString((char*)x); }
inline struct cvar_s* CVAR_CREATE(const char* cv, const char* val, const int flags) { return gEngfuncs.pfnRegisterVariable((char*)cv, (char*)val, flags); }

#define SPR_Load (*gEngfuncs.pfnSPR_Load)
void SPR_Set(HSPRITE hPic, int r, int g, int b);
#define SPR_Frames (*gEngfuncs.pfnSPR_Frames)
#define SPR_GetList (*gEngfuncs.pfnSPR_GetList)

// SPR_Draw  draws a the current sprite as solid
void SPR_Draw(int frame, int x, int y, const Rect* prc);
// SPR_DrawHoles  draws the current sprites,  with color index255 not drawn (transparent)
void SPR_DrawHoles(int frame, int x, int y, const Rect* prc);
// SPR_DrawAdditive  adds the sprites RGB values to the background  (additive transulency)
void SPR_DrawAdditive(int frame, int x, int y, const Rect* prc);

// SPR_EnableScissor  sets a clipping rect for HUD sprites.  (0,0) is the top-left hand corner of the screen.
#define SPR_EnableScissor (*gEngfuncs.pfnSPR_EnableScissor)
// SPR_DisableScissor  disables the clipping rect
#define SPR_DisableScissor (*gEngfuncs.pfnSPR_DisableScissor)
//
void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a);

// ScreenHeight returns the height of the screen, in pixels
#define ScreenHeight (gHUD.m_scrinfo.iHeight)
// ScreenWidth returns the width of the screen, in pixels
#define ScreenWidth (gHUD.m_scrinfo.iWidth)

#define BASE_XRES 640.f

// use this to project world coordinates to screen coordinates
#define XPROJECT(x) ((1.0f + (x)) * ScreenWidth * 0.5f)
#define YPROJECT(y) ((1.0f - (y)) * ScreenHeight * 0.5f)

#define XRES(x) ((x) * ((float)ScreenWidth / 640))
#define YRES(y) ((y) * ((float)ScreenHeight / 480))
#define XRES_HD(x) ((x) * V_max(1, (float)ScreenWidth / 1280))
#define YRES_HD(y) ((y) * V_max(1, (float)ScreenHeight / 720))

#define GetScreenInfo (*gEngfuncs.pfnGetScreenInfo)
#define ServerCmd (*gEngfuncs.pfnServerCmd)
#define EngineClientCmd (*gEngfuncs.pfnClientCmd)
#define EngineFilteredClientCmd (*gEngfuncs.pfnFilteredClientCmd)
void SetCrosshair(HSPRITE hspr, Rect rc, int r, int g, int b);
#define AngleVectors (*gEngfuncs.pfnAngleVectors)


// Gets the height & width of a sprite,  at the specified frame
inline int SPR_Height(HSPRITE x, int f) { return gEngfuncs.pfnSPR_Height(x, f); }
inline int SPR_Width(HSPRITE x, int f) { return gEngfuncs.pfnSPR_Width(x, f); }

inline client_textmessage_t* TextMessageGet(const char* pName) { return gEngfuncs.pfnTextMessageGet(pName); }
inline int TextMessageDrawChar(int x, int y, int number, int r, int g, int b)
{
	return gEngfuncs.pfnDrawCharacter(x, y, number, r, g, b);
}

int DrawConsoleString(int x, int y, const char* string);

void GetConsoleStringSize(const char* string, int* width, int* height);

inline int ConsoleStringLen(const char* string)
{
	int _width, _height;
	GetConsoleStringSize(string, &_width, &_height);
	return _width;
}

inline void ConsolePrint(const char* string)
{
	gEngfuncs.pfnConsolePrint(string);
}

inline void CenterPrint(const char* string)
{
	gEngfuncs.pfnCenterPrint(string);
}

int DrawString(int x, int y, const char* str, int r, int g, int b);

int DrawStringReverse(int x, int y, const char* str, int r, int g, int b);

inline char* safe_strcpy(char* dst, const char* src, int len_dst)
{
	if (len_dst <= 0)
	{
		return NULL; // this is bad
	}

	strncpy(dst, src, len_dst);
	dst[len_dst - 1] = '\0';

	return dst;
}

inline int safe_sprintf(char* dst, int len_dst, const char* format, ...)
{
	if (len_dst <= 0)
	{
		return -1; // this is bad
	}

	va_list v;

	va_start(v, format);

	vsnprintf(dst, len_dst, format, v);

	va_end(v);

	dst[len_dst - 1] = '\0';

	return 0;
}

// sound functions
inline void PlaySound(const char* szSound, float vol) { gEngfuncs.pfnPlaySoundByName(szSound, vol); }
inline void PlaySound(int iSound, float vol) { gEngfuncs.pfnPlaySoundByIndex(iSound, vol); }

void ScaleColors(int& r, int& g, int& b, int a);

float Length(const float* v);
void VectorMA(const float* veca, float scale, const float* vecb, float* vecc);
void VectorScale(const float* in, float scale, float* out);
float VectorNormalize(float* v);
void VectorInverse(float* v);

// disable 'possible loss of data converting float to int' warning message
#pragma warning(disable : 4244)
// disable 'truncation from 'const double' to 'float' warning message
#pragma warning(disable : 4305)

inline void UnpackRGB(int& r, int& g, int& b, unsigned long ulRGB)
{
	r = (ulRGB & 0xFF0000) >> 16;
	g = (ulRGB & 0xFF00) >> 8;
	b = ulRGB & 0xFF;
}

HSPRITE LoadSprite(const char* pszName);
