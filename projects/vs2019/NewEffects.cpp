/******************************
MOD			:	Beta 1999
FILE		:	NewEffects.cpp
FILE DESC	:	New Effects/Entity functions
CREDITS		:	sourcemodding.net
*******************************/

#include "NewEffects.h"

// Links the entity name in FGD to the class file, located in NewEffects.h
LINK_ENTITY_TO_CLASS(env_model, CEnvModel);

// Spawn function from NewEffects.h/CEnvModel
void CEnvModel::Spawn(void)
{
	PRECACHE_MODEL((char*)STRING(pev->model));
	SET_MODEL(ENT(pev), STRING(pev->model));

	// Basic collision, can't yet define it through in-game (will this even work?)
	// MIN X Y Z, MAX X Y Z
	pev->solid = SOLID_BBOX;
	UTIL_SetSize(pev, Vector(-32, -32, 0), Vector(32, 32, 32));

	// This enables the "think" function
	SetThink(&CEnvModel::Animate);
	pev->nextthink = gpGlobals->time + 0.01;
}

void CEnvModel::Animate(void)
{
	pev->nextthink = gpGlobals->time + 0.01;
	UTIL_RenderBBox(pev->origin, pev->mins, pev->maxs, 1, 0, 255, 0);
}