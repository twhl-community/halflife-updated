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
//=========================================================
// Hornets
//=========================================================

#pragma once

//=========================================================
// Hornet Defines
//=========================================================
#define HORNET_TYPE_RED 0
#define HORNET_TYPE_ORANGE 1
#define HORNET_RED_SPEED (float)600
#define HORNET_ORANGE_SPEED (float)800
#define HORNET_BUZZ_VOLUME (float)0.8

extern int iHornetPuff;

//=========================================================
// Hornet - this is the projectile that the Alien Grunt fires.
//=========================================================
class CHornet : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	int Classify() override;
	int IRelationship(CBaseEntity* pTarget) override;
	bool Save(CSave& save) override;
	bool Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	void IgniteTrail();
	void EXPORT StartTrack();
	void EXPORT StartDart();
	void EXPORT TrackTarget();
	void EXPORT TrackTouch(CBaseEntity* pOther);
	void EXPORT DartTouch(CBaseEntity* pOther);
	void EXPORT DieTouch(CBaseEntity* pOther);

	bool TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

	float m_flStopAttack;
	int m_iHornetType;
	float m_flFlySpeed;
};
