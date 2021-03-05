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
#ifndef TRAINS_H
#define TRAINS_H

// Tracktrain spawn flags
#define SF_TRACKTRAIN_NOPITCH		0x0001
#define SF_TRACKTRAIN_NOCONTROL		0x0002
#define SF_TRACKTRAIN_FORWARDONLY	0x0004
#define SF_TRACKTRAIN_PASSABLE		0x0008

// Spawnflag for CPathTrack
#define SF_PATH_DISABLED		0x00000001
#define SF_PATH_FIREONCE		0x00000002
#define SF_PATH_ALTREVERSE		0x00000004
#define SF_PATH_DISABLE_TRAIN	0x00000008
#define SF_PATH_ALTERNATE		0x00008000

// Spawnflags of CPathCorner
#define SF_CORNER_WAITFORTRIG	0x001
#define SF_CORNER_TELEPORT		0x002
#define SF_CORNER_FIREONCE		0x004

//#define PATH_SPARKLE_DEBUG		1	// This makes a particle effect around path_track entities for debugging
class CPathTrack : public CPointEntity
{
public:
	void		Spawn() override;
	void		Activate() override;
	void		KeyValue( KeyValueData* pkvd) override;
	
	void		SetPrevious( CPathTrack *pprevious );
	void		Link();
	void		Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	CPathTrack	*ValidPath( CPathTrack *ppath, int testFlag );		// Returns ppath if enabled, NULL otherwise
	void		Project( CPathTrack *pstart, CPathTrack *pend, Vector *origin, float dist );

	static CPathTrack *Instance( edict_t *pent );

	CPathTrack	*LookAhead( Vector *origin, float dist, int move );
	CPathTrack	*Nearest( Vector origin );

	CPathTrack	*GetNext();
	CPathTrack	*GetPrevious();

	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	
	static	TYPEDESCRIPTION m_SaveData[];
#if PATH_SPARKLE_DEBUG
	void EXPORT Sparkle();
#endif

	float		m_length;
	string_t	m_altName;
	CPathTrack	*m_pnext;
	CPathTrack	*m_pprevious;
	CPathTrack	*m_paltpath;
};


class CFuncTrackTrain : public CBaseEntity
{
public:
	void Spawn() override;
	void Precache() override;

	void Blocked( CBaseEntity *pOther ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void KeyValue( KeyValueData* pkvd ) override;

	void EXPORT Next();
	void EXPORT Find();
	void EXPORT NearestPath();
	void EXPORT DeadEnd();

	void		NextThink( float thinkTime, BOOL alwaysThink );

	void SetTrack( CPathTrack *track ) { m_ppath = track->Nearest(pev->origin); }
	void SetControls( entvars_t *pevControls );
	BOOL OnControls( entvars_t *pev ) override;

	void StopSound ();
	void UpdateSound ();
	
	static CFuncTrackTrain *Instance( edict_t *pent );

	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	
	static	TYPEDESCRIPTION m_SaveData[];
	int	ObjectCaps() override { return (CBaseEntity :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_DIRECTIONAL_USE; }

	void	OverrideReset() override;

	CPathTrack	*m_ppath;
	float		m_length;
	float		m_height;
	float		m_speed;
	float		m_dir;
	float		m_startSpeed;
	Vector		m_controlMins;
	Vector		m_controlMaxs;
	int			m_soundPlaying;
	int			m_sounds;
	float		m_flVolume;
	float		m_flBank;
	float		m_oldSpeed;

private:
	unsigned short m_usAdjustPitch;
};

#endif
