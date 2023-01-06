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

#pragma once

#include "scriptevent.h"

#define SF_SCRIPT_WAITTILLSEEN 1
#define SF_SCRIPT_EXITAGITATED 2
#define SF_SCRIPT_REPEATABLE 4
#define SF_SCRIPT_LEAVECORPSE 8
//#define SF_SCRIPT_INTERPOLATE		16 // don't use, old bug
#define SF_SCRIPT_NOINTERRUPT 32
#define SF_SCRIPT_OVERRIDESTATE 64
#define SF_SCRIPT_NOSCRIPTMOVEMENT 128

#define SCRIPT_BREAK_CONDITIONS (bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE)

enum SS_INTERRUPT
{
	SS_INTERRUPT_IDLE = 0,
	SS_INTERRUPT_BY_NAME,
	SS_INTERRUPT_AI,
};

// when a monster finishes an AI scripted sequence, we can choose
// a schedule to place them in. These defines are the aliases to
// resolve worldcraft input to real schedules (sjb)
#define SCRIPT_FINISHSCHED_DEFAULT 0
#define SCRIPT_FINISHSCHED_AMBUSH 1

class CCineMonster : public CBaseMonster
{
public:
	void Spawn() override;
	bool KeyValue(KeyValueData* pkvd) override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	void Blocked(CBaseEntity* pOther) override;
	void Touch(CBaseEntity* pOther) override;
	int ObjectCaps() override { return (CBaseMonster::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }
	void Activate() override;

	bool Save(CSave& save) override;
	bool Restore(CRestore& restore) override;

	static TYPEDESCRIPTION m_SaveData[];

	// void EXPORT CineSpawnThink();
	void EXPORT CineThink();
	void Pain();
	void Die();
	void DelayStart(bool state);
	bool FindEntity();
	virtual void PossessEntity();

	void ReleaseEntity(CBaseMonster* pEntity);
	void CancelScript();
	virtual bool StartSequence(CBaseMonster* pTarget, int iszSeq, bool completeOnEmpty);
	virtual bool FCanOverrideState();
	void SequenceDone(CBaseMonster* pMonster);
	virtual void FixScriptMonsterSchedule(CBaseMonster* pMonster);
	bool CanInterrupt();
	void AllowInterrupt(bool fAllow);
	int IgnoreConditions() override;

	int m_iszIdle;	 // string index for idle animation
	int m_iszPlay;	 // string index for scripted animation
	int m_iszEntity; // entity that is wanted for this script
	int m_fMoveTo;
	int m_iFinishSchedule;
	float m_flRadius; // range to search
	float m_flRepeat; // repeat rate

	int m_iDelay;
	float m_startTime;

	int m_saved_movetype;
	int m_saved_solid;
	int m_saved_effects;
	//	Vector m_vecOrigOrigin;
	bool m_interruptable;
};

class CCineAI : public CCineMonster
{
	bool StartSequence(CBaseMonster* pTarget, int iszSeq, bool completeOnEmpty) override;
	void PossessEntity() override;
	bool FCanOverrideState() override;
	void FixScriptMonsterSchedule(CBaseMonster* pMonster) override;
};
