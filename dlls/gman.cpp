/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// GMan - misunderstood servant of the people
//=========================================================
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "schedule.h"
#include "weapons.h"
#include "soundent.h"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================

class CGMan : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	int ISoundMask() override;

	bool Save(CSave& save) override;
	bool Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	void StartTask(Task_t* pTask) override;
	void RunTask(Task_t* pTask) override;
	bool TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;

	void PlayScriptedSentence(const char* pszSentence, float duration, float volume, float attenuation, bool bConcurrent, CBaseEntity* pListener) override;

	EHANDLE m_hPlayer;
	EHANDLE m_hTalkTarget;
	float m_flTalkTime;
};
LINK_ENTITY_TO_CLASS(monster_gman, CGMan);


TYPEDESCRIPTION CGMan::m_SaveData[] =
	{
		DEFINE_FIELD(CGMan, m_hTalkTarget, FIELD_EHANDLE),
		DEFINE_FIELD(CGMan, m_flTalkTime, FIELD_TIME),
};
IMPLEMENT_SAVERESTORE(CGMan, CBaseMonster);


//=========================================================
// Classify - indicates this monster's place in the
// relationship table.
//=========================================================
int CGMan::Classify()
{
	return CLASS_NONE;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CGMan::SetYawSpeed()
{
	int ys;

	switch (m_Activity)
	{
	case ACT_IDLE:
	default:
		ys = 90;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CGMan::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	switch (pEvent->event)
	{
	case 0:
	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// ISoundMask - generic monster can't hear.
//=========================================================
int CGMan::ISoundMask()
{
	return bits_SOUND_NONE;
}

//=========================================================
// Spawn
//=========================================================
void CGMan::Spawn()
{
	Precache();

	SET_MODEL(ENT(pev), "models/gman.mdl");
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = DONT_BLEED;
	pev->health = 100;
	m_flFieldOfView = 0.5; // indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;

	MonsterInit();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CGMan::Precache()
{
	PRECACHE_MODEL("models/gman.mdl");
}


//=========================================================
// AI Schedules Specific to this monster
//=========================================================


void CGMan::StartTask(Task_t* pTask)
{
	switch (pTask->iTask)
	{
	case TASK_WAIT:
		if (m_hPlayer == NULL)
		{
			m_hPlayer = UTIL_FindEntityByClassname(NULL, "player");
		}
		break;
	}
	CBaseMonster::StartTask(pTask);
}

void CGMan::RunTask(Task_t* pTask)
{
	switch (pTask->iTask)
	{
	case TASK_WAIT:
		// look at who I'm talking to
		if (m_flTalkTime > gpGlobals->time && m_hTalkTarget != NULL)
		{
			float yaw = VecToYaw(m_hTalkTarget->pev->origin - pev->origin) - pev->angles.y;

			if (yaw > 180)
				yaw -= 360;
			if (yaw < -180)
				yaw += 360;

			// turn towards vector
			SetBoneController(0, yaw);
		}
		// look at player, but only if playing a "safe" idle animation
		else if (m_hPlayer != NULL && pev->sequence == 0)
		{
			float yaw = VecToYaw(m_hPlayer->pev->origin - pev->origin) - pev->angles.y;

			if (yaw > 180)
				yaw -= 360;
			if (yaw < -180)
				yaw += 360;

			// turn towards vector
			SetBoneController(0, yaw);
		}
		else
		{
			SetBoneController(0, 0);
		}
		CBaseMonster::RunTask(pTask);
		break;
	default:
		SetBoneController(0, 0);
		CBaseMonster::RunTask(pTask);
		break;
	}
}


//=========================================================
// Override all damage
//=========================================================
bool CGMan::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	pev->health = pev->max_health / 2; // always trigger the 50% damage aitrigger

	if (flDamage > 0)
	{
		SetConditions(bits_COND_LIGHT_DAMAGE);
	}

	if (flDamage >= 20)
	{
		SetConditions(bits_COND_HEAVY_DAMAGE);
	}
	return true;
}


void CGMan::TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType)
{
	UTIL_Ricochet(ptr->vecEndPos, 1.0);
	AddMultiDamage(pevAttacker, this, flDamage, bitsDamageType);
}


void CGMan::PlayScriptedSentence(const char* pszSentence, float duration, float volume, float attenuation, bool bConcurrent, CBaseEntity* pListener)
{
	CBaseMonster::PlayScriptedSentence(pszSentence, duration, volume, attenuation, bConcurrent, pListener);

	m_flTalkTime = gpGlobals->time + duration;
	m_hTalkTarget = pListener;
}
