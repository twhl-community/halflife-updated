//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "cmdlib.h"
#include "mathlib.h"
#include "bspfile.h"
#include "threads.h"


#define DEFAULTLIGHTLEVEL 300

typedef struct entity_s
{
	char classname[64];
	vec3_t origin;
	float angle;
	vec3_t light;
	int style;
	bool targetent;
	vec3_t targetorigin;
} lightentity_t;

extern lightentity_t lightentities[MAX_MAP_ENTITIES];
extern int numlightentities;

#define LIGHT_ON_EPSILON 0.1

#define MAXLIGHTS 1024

void LoadNodes(char* file);
bool TestLine(vec3_t start, vec3_t stop);

void LightFace(int surfnum);
void LightLeaf(dleaf_t* leaf);

void MakeTnodes();

extern float scaledist;
extern float scalecos;
extern float rangescale;

extern int c_culldistplane, c_proper;

byte* GetFileSpace(int size);
extern byte* filebase;

extern vec3_t bsp_origin;
extern vec3_t bsp_xvector;
extern vec3_t bsp_yvector;

void TransformSample(vec3_t in, vec3_t out);
void RotateSample(vec3_t in, vec3_t out);

extern bool extrasamples;

extern float minlights[MAX_MAP_FACES];
