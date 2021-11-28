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
//  cdll_int.h
//
// 4-23-98
// JOHN:  client dll interface declarations
//

#pragma once

#include "const.h"
#include "steam/steamtypes.h"
#include "ref_params.h"
#include "r_efx.h"
#include "studio_event.h"
#include "common_types.h"

// this file is included by both the engine and the client-dll,
// so make sure engine declarations aren't done twice

typedef int HSPRITE; // handle to a graphic

#define SCRINFO_SCREENFLASH 1
#define SCRINFO_STRETCHED 2

typedef struct SCREENINFO_s
{
	int iSize;
	int iWidth;
	int iHeight;
	int iFlags;
	int iCharHeight;
	short charWidths[256];
} SCREENINFO;


typedef struct client_data_s
{
	// fields that cannot be modified  (ie. have no effect if changed)
	Vector origin;

	// fields that can be changed by the cldll
	Vector viewangles;
	int iWeaponBits;
	//	int		iAccessoryBits;
	float fov; // field of view
} client_data_t;

typedef struct client_sprite_s
{
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	Rect rc;
} client_sprite_t;



typedef struct hud_player_info_s
{
	char* name;
	short ping;
	byte thisplayer; // true if this is the calling player

	byte spectator;
	byte packetloss;

	char* model;
	short topcolor;
	short bottomcolor;

	uint64 m_nSteamID;
} hud_player_info_t;

#include "in_buttons.h"

#define CLDLL_INTERFACE_VERSION 7

//#include "server.h" // server_static_t define for apiproxy
#include "APIProxy.h"
