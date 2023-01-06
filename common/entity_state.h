/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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

#pragma once

// For entityType below
#define ENTITY_NORMAL (1 << 0)
#define ENTITY_BEAM (1 << 1)

// Entity state is used for the baseline and for delta compression of a packet of
//  entities that is sent to a client.
typedef struct entity_state_s entity_state_t;

struct entity_state_s
{
	// Fields which are filled in by routines outside of delta compression
	int entityType;
	// Index into cl_entities array for this entity.
	int number;
	float msg_time;

	// Message number last time the player/entity state was updated.
	int messagenum;

	// Fields which can be transitted and reconstructed over the network stream
	Vector origin;
	Vector angles;

	int modelindex;
	int sequence;
	float frame;
	int colormap;
	short skin;
	short solid;
	int effects;
	float scale;

	byte eflags;

	// Render information
	int rendermode;
	int renderamt;
	color24 rendercolor;
	int renderfx;

	int movetype;
	float animtime;
	float framerate;
	int body;
	byte controller[4];
	byte blending[4];
	Vector velocity;

	// Send bbox down to client for use during prediction.
	Vector mins;
	Vector maxs;

	int aiment;
	// If owned by a player, the index of that player ( for projectiles ).
	int owner;

	// Friction, for prediction.
	float friction;
	// Gravity multiplier
	float gravity;

	// PLAYER SPECIFIC
	int team;
	int playerclass;
	int health;
	qboolean spectator;
	int weaponmodel;
	int gaitsequence;
	// If standing on conveyor, e.g.
	Vector basevelocity;
	// Use the crouched hull, or the regular player hull.
	int usehull;
	// Latched buttons last time state updated.
	int oldbuttons;
	// -1 = in air, else pmove entity number
	int onground;
	int iStepLeft;
	// How fast we are falling
	float flFallVelocity;

	float fov;
	int weaponanim;

	// Parametric movement overrides
	Vector startpos;
	Vector endpos;
	float impacttime;
	float starttime;

	// For mods
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	Vector vuser1;
	Vector vuser2;
	Vector vuser3;
	Vector vuser4;
};

#include "pm_info.h"

typedef struct clientdata_s
{
	Vector origin;
	Vector velocity;

	int viewmodel;
	Vector punchangle;
	int flags;
	int waterlevel;
	int watertype;
	Vector view_ofs;
	float health;

	int bInDuck;

	int weapons; // remove?

	int flTimeStepSound;
	int flDuckTime;
	int flSwimTime;
	int waterjumptime;

	float maxspeed;

	float fov;
	int weaponanim;

	int m_iId;
	int ammo_shells;
	int ammo_nails;
	int ammo_cells;
	int ammo_rockets;
	float m_flNextAttack;

	int tfstate;

	int pushmsec;

	int deadflag;

	char physinfo[MAX_PHYSINFO_STRING];

	// For mods
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	Vector vuser1;
	Vector vuser2;
	Vector vuser3;
	Vector vuser4;
} clientdata_t;

#include "weaponinfo.h"

typedef struct local_state_s
{
	entity_state_t playerstate;
	clientdata_t client;
	weapon_data_t weapondata[64];
} local_state_t;
