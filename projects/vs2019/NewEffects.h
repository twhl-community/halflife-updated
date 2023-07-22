/******************************
MOD			:	Beta 1999
FILE		:	NewEffects.h
FILE DESC	:	New Effects/Entities for the mod
CREDITS		:	sourcemodding.net
*******************************/

#ifndef NEW_EFFECTS
#define ENV_MODEL
 
#include "extdll.h"		// Required for KeyValueData & Export Parts
#include "util.h"		// Required Consts & Macros
#include "cbase.h"		// Required for inheriting from CBaseAnimating

class CEnvModel : public CBaseAnimating
{
private:
	void Spawn(void);
};

#endif