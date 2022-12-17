/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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

#define SF_ENVEXPLOSION_NODAMAGE (1 << 0)	// when set, ENV_EXPLOSION will not actually inflict damage
#define SF_ENVEXPLOSION_REPEATABLE (1 << 1) // can this entity be refired?
#define SF_ENVEXPLOSION_NOFIREBALL (1 << 2) // don't draw the fireball
#define SF_ENVEXPLOSION_NOSMOKE (1 << 3)	// don't draw the smoke
#define SF_ENVEXPLOSION_NODECAL (1 << 4)	// don't make a scorch mark
#define SF_ENVEXPLOSION_NOSPARKS (1 << 5)	// don't make a scorch mark

extern void ExplosionCreate(const Vector& center, const Vector& angles, edict_t* pOwner, int magnitude, bool doDamage);
