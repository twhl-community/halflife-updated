/******************************
MOD			:	Cookie's Half-Life Updated
FILE		:	NewEffects.cpp
FILE DESC	:	New Effects/Entity functions
CREDITS		:	sourcemodding.net, twhl.info
*******************************/

#include "neweffects.h"

/*******************************

ENV_MODEL

*******************************/

// Links the entity name in FGD to the class file, located in NewEffects.h
LINK_ENTITY_TO_CLASS(env_model, CEnvModel);        

// Spawn function from NewEffects.h/CEnvModel
void CEnvModel::Spawn(void)
{
	if (pev->model != 0)
	{
		PRECACHE_MODEL((char*)STRING(pev->model));
		SET_MODEL(ENT(pev), STRING(pev->model));
	}
	else
	{
		ALERT(at_console, "[env_model] Error, file failed to load!\n");
		ALERT(at_console, "[env_model] Setting model/null.mdl in its place!\n");
		PRECACHE_MODEL("models/null.mdl");
		SET_MODEL(ENT(pev), "models/null.mdl");
	}

	// Basic collision using BBoxing.
	// Checks if the "IS SOLID" flag is ticked. If so, then set the BBox to be solid.
	// If it's ticked, also set debug to be toggleable.
	if (FBitSet(pev->spawnflags, ENV_MODEL_IS_SOLID))
	{
		pev->solid = SOLID_BBOX;

		if (m_iCollisionMode == 1)
		{
			mins = mins * m_fModelScale;
			maxs = maxs * m_fModelScale;

			UTIL_SetSize(pev, mins, maxs);
		}
		else if (m_iCollisionMode == 2)
		{
			ExtractBbox(m_iSequence, mins, maxs);

			mins = mins * m_fModelScale;
			maxs = maxs * m_fModelScale;

			UTIL_SetSize(pev, mins, maxs);
		}

		// This is supposed to enable debug mode if the debug flag is ticked.
		if (FBitSet(pev->spawnflags, ENV_MODEL_DEBUG_BB))
		{
			m_bDebugBB = true;
		}
	}

	pev->scale = m_fModelScale;
	
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
	else if (FStrEq(pkvd->szKeyName, "modelscale"))
	{
		m_fModelScale = atof(pkvd->szValue);
		return true;
	}
	else
		return CBaseEntity::KeyValue(pkvd);
}

/*******************************

ENV_FOG

*******************************/

// Global Variables

int CEnvFog::g_iCurrentEndDist = 0;
int CEnvFog::g_iIdealEndDist = 0;
float CEnvFog::g_flBlendDoneTime = 0;

LINK_ENTITY_TO_CLASS(env_fog, CEnvFog);

TYPEDESCRIPTION CEnvFog::m_SaveData[] =
{
		DEFINE_FIELD(CEnvFog, m_iStartDist, FIELD_INTEGER),
		DEFINE_FIELD(CEnvFog, m_iEndDist, FIELD_INTEGER),
		DEFINE_FIELD(CEnvFog, m_bActive, FIELD_BOOLEAN),
		DEFINE_FIELD(CEnvFog, m_flBlendTime, FIELD_FLOAT)
};
IMPLEMENT_SAVERESTORE(CEnvFog, CBaseEntity);

void CEnvFog::Spawn(void)
{
	Precache();

	pev->solid = SOLID_NOT;
	pev->effects |= EF_NODRAW;
	pev->movetype = MOVETYPE_NONE;

	if (FBitSet(pev->spawnflags, ENV_FOG_ACTIVE) || FStringNull(pev->targetname))
	{
		m_bActive = true;
	}
	else
	{
		m_bActive = false;
	}
}

bool CEnvFog::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "startDist"))
	{
		m_iStartDist = atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "endDist"))
	{
		m_iEndDist = atoi(pkvd->szValue);
		return true;
	}
	if (FStrEq(pkvd->szKeyName, "blendTime"))
	{
		m_flBlendTime = atof(pkvd->szValue);
		return true;
	}

	return CBaseEntity::KeyValue(pkvd);
}

void CEnvFog::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	bool prevSate = m_bActive;

	switch (useType)
	{
	case USE_OFF:
		m_bActive = false;
		break;
	case USE_ON:
		m_bActive = true;
		break;
	default:
		m_bActive = !m_bActive;
		break;
	}

	if (prevSate != m_bActive)
	{
		UpdateFog(true, m_bActive, NULL);

		if (m_bActive || !m_flBlendTime)
		{
			CEnvFog::SetCurrentEndDist(m_bActive ? m_iEndDist : 0, m_flBlendTime);
		}
	}
}

void CEnvFog::UpdateFog(bool isOn, bool doBlend, CBaseEntity* pPlayer)
{
	if (isOn)
	{
		if (pPlayer)
			MESSAGE_BEGIN(MSG_ONE, gmsgFog, NULL, pPlayer->pev);
		else
			MESSAGE_BEGIN(MSG_ALL, gmsgFog, NULL);

		WRITE_COORD(m_iStartDist);
		WRITE_COORD(m_iEndDist);
		WRITE_BYTE(pev->rendercolor.x);
		WRITE_BYTE(pev->rendercolor.y);
		WRITE_BYTE(pev->rendercolor.z);
		if (doBlend)
			WRITE_COORD(m_flBlendTime);
		else
			WRITE_COORD(0);
		MESSAGE_END();
	}
	else if (!m_flBlendTime)
	{
		if (pPlayer)
			MESSAGE_BEGIN(MSG_ONE, gmsgFog, NULL, pPlayer->pev);
		else
			MESSAGE_BEGIN(MSG_ALL, gmsgFog, NULL);

		WRITE_COORD(0);
		WRITE_COORD(0);
		WRITE_BYTE(0);
		WRITE_BYTE(0);
		WRITE_BYTE(0);
		WRITE_COORD(0);
		MESSAGE_END();
	}
}

void CEnvFog::SendInitMessages(CBaseEntity* pPlayer)
{
	if (m_bActive)
	{
		return;
	}

	UpdateFog(true, false, pPlayer);

	CEnvFog::SetCurrentEndDist(m_iEndDist, m_flBlendTime);
}

void CEnvFog::SetCurrentEndDist(int endDist, float BlendTime)
{
	if (!BlendTime || g_iCurrentEndDist < endDist || !g_iCurrentEndDist)
	{
		g_iCurrentEndDist = endDist;
		g_iIdealEndDist = 0;
		g_flBlendDoneTime = 0;
	}
	else
	{
		g_iIdealEndDist = endDist;
		g_flBlendDoneTime = gpGlobals->time + BlendTime;
	}
}

void CEnvFog::FogThink(void)
{
	if (!g_flBlendDoneTime || !g_iIdealEndDist)
	{
		return;
	}

	if (g_flBlendDoneTime <= gpGlobals->time)
	{
		g_iCurrentEndDist = g_iIdealEndDist;
		g_iIdealEndDist = 0;
		g_flBlendDoneTime = 0;
	}
}

bool CEnvFog::CheckBBox(edict_t* pPlayer, edict_t* pEdict)
{
	if (!g_iCurrentEndDist)
	{
		return false;
	}

	// This is to make sure fog doesn't cull underwater.
	if (pPlayer->v.waterlevel == 3)
	{
		return false;
	}

	// Calculate distance to edge.
	Vector boxTotal = Vector(g_iCurrentEndDist, g_iCurrentEndDist, g_iCurrentEndDist);
	float edgeLength = boxTotal.Length();

	// Set the fog BBox mins and maxs
	Vector viewOrigin = pPlayer->v.origin + pPlayer->v.view_ofs;
	Vector fogMins, fogMaxs;
	for (int i = 0; i < 3; i++)
	{
		fogMins[i] = viewOrigin[i] - edgeLength;
		fogMaxs[i] = viewOrigin[i] + edgeLength;
	}

	
    // Set the entity mins/maxs
	Vector entMins, entMaxs;
	entMins = pEdict->v.origin + pEdict->v.mins;
	entMaxs = pEdict->v.origin + pEdict->v.maxs;

	if (fogMins[0] > entMaxs[0])
	{
		return true;
	}
	if (fogMins[1] > entMaxs[1])
	{
		return true;
	}
	if (fogMins[2] > entMaxs[2])
	{
		return true;
	}
	if (fogMins[1] < entMaxs[1])
	{
		return true;
	}
	if (fogMins[2] < entMaxs[2])
	{
		return true;
	}
	return false;
}