/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
****/
// updates:
// 1-4-98	fixed initialization

#include <chrono>
#include <thread>

#include <stdio.h>

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "mathlib.h"
#undef DotProduct
#include "../../dlls/vector.h"
#include "../../public/steam/steamtypes.h" // defines int32, required by studio.h
#include "..\..\engine\studio.h"
#include "mdlviewer.h"


#pragma warning(disable : 4244) // conversion from 'double ' to 'float ', possible loss of data
#pragma warning(disable : 4305) // truncation from 'const double ' to 'float '

vec3_t g_vright; // needs to be set to viewer's right in order for chrome to work
float g_lambert = 1.5;

float gldepthmin = 0;
float gldepthmax = 10.0;

SDL_Window* g_pWindow = nullptr;

SDL_GLContext g_GLContext = NULL;

/*
=============
R_Clear
=============
*/
void R_Clear(void)
{
	glDepthFunc(GL_LEQUAL);
	glDepthRange(gldepthmin, gldepthmax);
	glDepthMask(1);
}

static StudioModel tempmodel;

void mdlviewer_display()
{
	R_Clear();

	tempmodel.SetBlending(0, 0.0);
	tempmodel.SetBlending(1, 0.0);

	static long long prev;
	auto curr = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	tempmodel.AdvanceFrame((curr - prev) / 1000.0);
	prev = curr;

	tempmodel.DrawModel();
}


void mdlviewer_init(char* modelname)
{
	// make a bogus texture
	// R_InitTexture( );

	tempmodel.Init(modelname);
	tempmodel.SetSequence(0);

	tempmodel.SetController(0, 0.0);
	tempmodel.SetController(1, 0.0);
	tempmodel.SetController(2, 0.0);
	tempmodel.SetController(3, 0.0);
	tempmodel.SetMouth(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50., 1., .1, 10.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0.5, 0);
}


void mdlviewer_nextsequence(void)
{
	int iSeq = tempmodel.GetSequence();
	if (iSeq == tempmodel.SetSequence(iSeq + 1))
	{
		tempmodel.SetSequence(0);
	}
}


//////////////////////////////////////////////////////////////////


static int pstyle;
static int translate = 1;
static int mesh = 1;
static float transx = 0, transy = 0, transz = -2, rotx = 235, roty = -90;
static float amplitude = 0.03;
static float freq = 5.0f;
static float phase = .00003;
static int ox = -1, oy = -1;
static int show_t = 1;
static int mot;
#define PAN 1
#define ROT 2
#define ZOOM 3

void pan(int x, int y)
{
	transx += (x - ox) / 500.;
	transy -= (y - oy) / 500.;
	ox = x;
	oy = y;
	//glutPostRedisplay();
}

void zoom(int x, int /*y*/)
{
	transz += (x - ox) / 20.;
	ox = x;
	//glutPostRedisplay();
}

void rotate(int x, int y)
{
	rotx += x - ox;
	if (rotx > 360.)
		rotx -= 360.;
	else if (rotx < -360.)
		rotx += 360.;
	roty += y - oy;
	if (roty > 360.)
		roty -= 360.;
	else if (roty < -360.)
		roty += 360.;
	ox = x;
	oy = y;
	//glutPostRedisplay();
}

void motion(int x, int y)
{
	if (mot == PAN)
		pan(x, y);
	else if (mot == ROT)
		rotate(x, y);
	else if (mot == ZOOM)
		zoom(x, y);
}

void mouse(const SDL_MouseButtonEvent& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		switch (event.button)
		{
		case SDL_BUTTON_LEFT:
			mot = PAN;
			motion(ox = event.x, oy = event.y);
			break;
		case SDL_BUTTON_RIGHT:
			mot = ROT;
			motion(ox = event.x, oy = event.y);
			break;
		case SDL_BUTTON_MIDDLE:
			break;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		mot = 0;
	}
}

void help(void)
{
	printf("left mouse     - pan\n");
	printf("right mouse    - rotate\n");
}

void init(char* arg)
{
	mdlviewer_init(arg);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50., 1., .1, 10.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// glTranslatef(0.,0.,-5.5);
	// glTranslatef(-.2.,1.0,-1.5);

	glClearColor(0, 0, 0.5, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glTranslatef(transx, transy, transz);

	glRotatef(rotx, 0., 1., 0.);
	glRotatef(roty, 1., 0., 0.);

	glScalef(0.01, 0.01, 0.01);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);

	mdlviewer_display();

	glPopMatrix();
	SDL_GL_SwapWindow(g_pWindow);

	//glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

/*ARGSUSED1*/
void key(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key)
	{
	case 'h':
		help();
		break;

	case 'p':
		printf("Translation: %f, %f %f\n", transx, transy, transz);
		break;

	case '\033': // Escape
		exit(EXIT_SUCCESS);
		break;

	case ' ':
		mdlviewer_nextsequence();
		break;

	default:
		break;
	}
	//glutPostRedisplay();
}

#define WIN_SIZE 512
#define FRAMERATE 60

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage : %s <filename>\n", argv[0]);
		exit(1);
	}

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Couldn't initialize SDL2\n");
		exit(EXIT_FAILURE);
	}

	g_pWindow = SDL_CreateWindow(argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_SIZE, WIN_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!g_pWindow)
	{
		printf("Failed to create SDL Window\n");
		exit(EXIT_FAILURE);
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

	g_GLContext = SDL_GL_CreateContext(g_pWindow);

	SDL_GL_MakeCurrent(g_pWindow, g_GLContext);

	init(argv[1]);

	SDL_Event event;

	bool bQuit = false;

	auto prev = std::chrono::system_clock::now();

	while (!bQuit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				bQuit = true;
				break;
			}

			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				{
					reshape(event.window.data1, event.window.data2);
					break;
				}

				case SDL_WINDOWEVENT_CLOSE:
				{
					bQuit = true;
					break;
				}
				}
				break;
			}

			case SDL_KEYUP:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				key(event.key.keysym.sym, x, y);
				break;
			}

			case SDL_MOUSEMOTION:
			{
				motion(event.motion.x, event.motion.y);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				mouse(event.button);
				break;
			}

			default: break;
			}
		}

		//Quit immediately.
		if (bQuit)
			break;

		auto now = std::chrono::system_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() >= (1000 / FRAMERATE))
		{
			display();

			prev = now;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	SDL_GL_DeleteContext(g_GLContext);
	g_GLContext = NULL;
	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;

	SDL_Quit();

	return 0;
}
