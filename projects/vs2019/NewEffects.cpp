/******************************
MOD			:	Beta 1999
FILE		:	NewEffects.cpp
FILE DESC	:	New Effects/Entity functions
CREDITS		:	sourcemodding.net
*******************************/

#include "NewEffects.h"

/*******************************

ENV_MODEL

*******************************/

// Links the entity name in FGD to the class file, located in NewEffects.h
LINK_ENTITY_TO_CLASS(env_model, CEnvModel);

// Spawn function from NewEffects.h/CEnvModel
void CEnvModel::Spawn(void)
{
	PRECACHE_MODEL((char*)STRING(pev->model));
	SET_MODEL(ENT(pev), STRING(pev->model));

	// Basic collision using BBoxing.
	// Checks if the "IS SOLID" flag is ticked. If so, then set the BBox to be solid.
	// If it's ticked, also set debug to be toggleable.
	if (FBitSet(pev->spawnflags, ENV_MODEL_IS_SOLID))
	{
		pev->solid = SOLID_BBOX;

		if (m_iCollisionMode == 1)
		{
			UTIL_SetSize(pev, mins, maxs);
		}
		else if (m_iCollisionMode == 2)
		{
			ExtractBbox(m_iSequence, mins, maxs);
			UTIL_SetSize(pev, mins, maxs);
		}

		// This is supposed to enable debug mode if the debug flag is ticked.
		if (FBitSet(pev->spawnflags, ENV_MODEL_DEBUG_BB))
		{
			m_bDebugBB = true;
		}
	}

	// User animation input through hammer.
	if (FBitSet(pev->spawnflags, ENV_MODEL_ANIMATED))
	{
		m_bAnimate = true;
		pev->sequence = m_iSequence;
	}

	// This enables the "think" function
	SetThink(&CEnvModel::Animate);
	pev->nextthink = gpGlobals->time + 0.01;
}

void CEnvModel::Animate(void)
{
	// Checks if Debug mode is on. If so, then turn on the BBox visibility.
	pev->nextthink = gpGlobals->time + 0.01;
	if (m_bDebugBB)
	{
		UTIL_RenderBBox(pev->origin, pev->mins, pev->maxs, 1, 0, 255, 0);
	}

	// Checks if the user wants animations to play.
	if (m_bAnimate)
	{
		if (m_flAnimationSpeed >= 0.0f)
		{
			pev->frame > 255 ? pev->frame = 0 : pev->frame += m_flAnimationSpeed;
		}
		else
		{
			pev->frame < 0 ? pev->frame = 255 : pev->frame += m_flAnimationSpeed;
		}
	}
}

// Had to change all of these to return true or return false, hopefully this doesn't break anything.
bool CEnvModel::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "animationspeed"))
	{
		m_flAnimationSpeed = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "animate"))
	{
		m_iSequence = atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "collisionmode"))
	{
		m_iCollisionMode = atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "bbmins"))
	{
		UTIL_StringToVector(mins, pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "bbmaxs"))
	{
		UTIL_StringToVector(maxs, pkvd->szValue);
		return true;
	}
	else
		return CBaseEntity::KeyValue(pkvd);
}