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
}