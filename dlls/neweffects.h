/******************************
MOD			:	Cookie's Half-Life: Updated
FILE		:	neweffects.h
FILE DESC	:	New Effects/Entities for the mod
CREDITS		:	sourcemodding.net, twhl.info
*******************************/

#ifndef NEW_EFFECTS
 
#include "extdll.h"		// Required for KeyValueData & Export Parts.
#include "util.h"		// Required Consts & Macros.
#include "cbase.h"		// Required for inheriting from CBaseAnimating.

/*******************************

ENV_MODEL

*******************************/

#define ENV_MODEL

class CEnvModel : public CBaseAnimating
{
private:
	void Spawn(void);
	// This float controls the model's scale, default is 1.0;
	float m_fModelScale = 1.0f;

	// m_iCollisionMode sets the current collision type, manual or sequence.
	// m_iSequence is an animation loader, telling the game which animation to load.
	unsigned short m_iCollisionMode, m_iSequence;

	// KeyValues
	bool KeyValue(KeyValueData* pkvd);
	Vector mins = {0, 0, 0}, 
		   maxs = {0, 0, 0};

	// m_bDebugBB toggles whether or not the user wants to see the collision box for models.
	// m_bAnimate toggles whether or not the model is playing an animation.
	bool m_bDebugBB = false, m_bAnimate = false;

	// Updates the model frame-by-frame, or by a custom timer.
	void EXPORT Animate(void);

	// m_flAnimationSpeed sets the animation playback speed, from 0.0. Default is 1.0.
	float m_flAnimationSpeed = 0.0f;

	#define ENV_MODEL_IS_SOLID 1
	#define ENV_MODEL_DEBUG_BB 2
	#define ENV_MODEL_ANIMATED 4
};

/*******************************

ENV_FOG

*******************************/

#define ENV_FOG

class CEnvFog : public CBaseEntity
{
public:
	// Spawn the entity.
	void Spawn(void);

	// Sends a message on initialization.
	void SendInitMessages(CBaseEntity* pPlayer = NULL);
	
	// A boolean for key values.
	bool KeyValue(KeyValueData* pkvd) override;

	// An update to the fog, used by other entities and triggers.
	void UpdateFog(bool isOn, bool doBlend, CBaseEntity* pPlayer);

	// Makes Env_Fog usable by other entities.
	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value) override;


	// An integer for when the game is saved.
	bool Save(CSave& save);

	// An integer for when the game is loaded.
	bool Restore(CRestore& restore);

public: 
	// Sets the new end distance for Env_Fog.
	static void SetCurrentEndDist(int endDist, float blendTime);

	// Gets the current end distance for Env_Fog.
	static int GetCurrentEndDist(void)
	{
		return g_iCurrentEndDist;
	}

	static void FogThink(void);

	static bool CheckBBox(edict_t* pPlayer, edict_t* pEdict);

private:
	// m_SaveData saves the fog data.
	static TYPEDESCRIPTION m_SaveData[];

	// m_iStartDistance is the starting distance of the fog from the player.
	int m_iStartDist;

	// m_iEndDist is the end distance of the fog from the player.
	int m_iEndDist;

	// m_flBlendTime is the amount of fog to blend together.
	float m_flBlendTime;

	// m_bActive sets whether or not env_fog is active.
	bool m_bActive;

	#define ENV_FOG_ACTIVE 1
private:
	// The current end distance
	static int g_iCurrentEndDist;

	// The ideal end distance
	static int g_iIdealEndDist;

	// The blend time
	static float g_flBlendDoneTime;
};

#endif