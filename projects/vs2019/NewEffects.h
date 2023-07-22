/******************************
MOD			:	Beta 1999
FILE		:	NewEffects.h
FILE DESC	:	New Effects/Entities for the mod
CREDITS		:	sourcemodding.net
*******************************/

#ifndef NEW_EFFECTS
#define ENV_MODEL
 
#include "extdll.h"		// Required for KeyValueData & Export Parts.
#include "util.h"		// Required Consts & Macros.
#include "cbase.h"		// Required for inheriting from CBaseAnimating.

class CEnvModel : public CBaseAnimating
{
private:
	void Spawn(void);
	// This float controls the model's scale, default is 1.0;
	float m_fModelScale = 1.0f;

	// Lets the mapper decide if they want manual collisions, or sequence-based collisions.
	// The second unsigned short is an integer, to tell the game what animation to load.
	unsigned short m_iCollisionMode, m_iSequence;

	// The start of the "modular" BBox system.
	bool KeyValue(KeyValueData* pkvd) override;
	Vector mins = {0, 0, 0}, 
		   maxs = {0, 0, 0};

	// Toggles the BBox debug visibility, and if the user wants animations.
	bool m_bDebugBB = false, m_bAnimate = false;

	// Updates the model frame-by-frame (or custom timer).
	void EXPORT Animate(void);

	// This float controls the animation playback speed.
	float m_flAnimationSpeed = 0.0f;

	#define ENV_MODEL_IS_SOLID 1
	#define ENV_MODEL_DEBUG_BB 2
	#define ENV_MODEL_ANIMATED 4
};

#endif