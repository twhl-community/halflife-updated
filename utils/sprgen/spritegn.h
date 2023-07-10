/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
****/

//
// spritegn.h: header file for sprite generation program
//

// **********************************************************
// * This file must be identical in the spritegen directory *
// * and in the Quake directory, because it's used to       *
// * pass data from one to the other via .spr files.        *
// **********************************************************

//-------------------------------------------------------
// This program generates .spr sprite package files.
// The format of the files is as follows:
//
// dsprite_t file header structure
// <repeat dsprite_t.numframes times>
//   <if spritegroup, repeat dspritegroup_t.numframes times>
//     dspriteframe_t frame header structure
//     sprite bitmap
//   <else (single sprite frame)>
//     dspriteframe_t frame header structure
//     sprite bitmap
// <endrepeat>
//-------------------------------------------------------

#pragma once

#ifdef INCLUDELIBS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "cmdlib.h"
#include "scriplib.h"
#include "lbmlib.h"

#endif

constexpr int SPRITE_VERSION = 2;

// must match definition in modelgen.h
enum synctype_t
{
	ST_SYNC = 0,
	ST_RAND
};

// TODO: shorten these?
typedef struct
{
	int ident;
	int version;
	int type;
	int texFormat;
	float boundingradius;
	int width;
	int height;
	int numframes;
	float beamlength;
	synctype_t synctype;
} dsprite_t;

enum SPR_POSITION
{
	SPR_VP_PARALLEL_UPRIGHT = 0,
	SPR_FACING_UPRIGHT,
	SPR_VP_PARALLEL,
	SPR_ORIENTED,
	SPR_VP_PARALLEL_ORIENTED
};

enum SPR
{
	SPR_NORMAL = 0,
	SPR_ADDITIVE,
	SPR_INDEXALPHA,
	SPR_ALPHTEST
};

typedef struct
{
	int origin[2];
	int width;
	int height;
} dspriteframe_t;

typedef struct
{
	int numframes;
} dspritegroup_t;

typedef struct
{
	float interval;
} dspriteinterval_t;

enum spriteframetype_t
{
	SPR_SINGLE = 0,
	SPR_GROUP
};

typedef struct
{
	spriteframetype_t type;
} dspriteframetype_t;

#define IDSPRITEHEADER (('P' << 24) + ('S' << 16) + ('D' << 8) + 'I')
// little-endian "IDSP"
