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
// Base class for flying monsters.  This overrides the movement test & execution code from CBaseMonster

#pragma once

class CFlyingMonster : public CBaseMonster
{
public:
	int CheckLocalMove(const Vector& vecStart, const Vector& vecEnd, CBaseEntity* pTarget, float* pflDist) override; // check validity of a straight move through space
	bool FTriangulate(const Vector& vecStart, const Vector& vecEnd, float flDist, CBaseEntity* pTargetEnt, Vector* pApex) override;
	Activity GetStoppedActivity() override;
	void Killed(entvars_t* pevAttacker, int iGib) override;
	void Stop() override;
	float ChangeYaw(int yawSpeed) override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	void MoveExecute(CBaseEntity* pTargetEnt, const Vector& vecDir, float flInterval) override;
	void Move(float flInterval = 0.1) override;
	bool ShouldAdvanceRoute(float flWaypointDist) override;

	inline void SetFlyingMomentum(float momentum) { m_momentum = momentum; }
	inline void SetFlyingFlapSound(const char* pFlapSound) { m_pFlapSound = pFlapSound; }
	inline void SetFlyingSpeed(float speed) { m_flightSpeed = speed; }
	float CeilingZ(const Vector& position);
	float FloorZ(const Vector& position);
	bool ProbeZ(const Vector& position, const Vector& probe, float* pFraction);


	// UNDONE:  Save/restore this stuff!!!
protected:
	Vector m_vecTravel;	 // Current direction
	float m_flightSpeed; // Current flight speed (decays when not flapping or gliding)
	float m_stopTime;	 // Last time we stopped (to avoid switching states too soon)
	float m_momentum;	 // Weight for desired vs. momentum velocity
	const char* m_pFlapSound;
	float m_flLastZYawTime;
};
