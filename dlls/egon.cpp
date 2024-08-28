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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "monsters.h"
#include "weapons.h"
#include "effects.h"
#include "customentity.h"
#include "gamerules.h"
#include "UserMessages.h"

#ifdef CLIENT_DLL
#include "hud.h"
#include "com_weapons.h"
#else
extern bool IsBustingGame();
#endif

#define EGON_SWITCH_NARROW_TIME 0.75 // Time it takes to switch fire modes
#define EGON_SWITCH_WIDE_TIME 1.5

LINK_ENTITY_TO_CLASS(weapon_egon, CEgon);

void CEgon::Spawn()
{
	Precache();
	m_iId = WEAPON_EGON;
	SET_MODEL(ENT(pev), "models/w_egon.mdl");

	m_iDefaultAmmo = EGON_DEFAULT_GIVE;

	FallInit(); // get ready to fall down.
}


void CEgon::Precache()
{
	PRECACHE_MODEL("models/w_egon.mdl");
	PRECACHE_MODEL("models/v_egon.mdl");
	PRECACHE_MODEL("models/p_egon.mdl");

	PRECACHE_MODEL("models/w_9mmclip.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND(EGON_SOUND_OFF);
	PRECACHE_SOUND(EGON_SOUND_RUN);
	PRECACHE_SOUND(EGON_SOUND_STARTUP);

	PRECACHE_MODEL(EGON_BEAM_SPRITE);
	PRECACHE_MODEL(EGON_FLARE_SPRITE);

	PRECACHE_SOUND("weapons/357_cock1.wav");

	m_usEgonFire = PRECACHE_EVENT(1, "events/egon_fire.sc");
	m_usEgonStop = PRECACHE_EVENT(1, "events/egon_stop.sc");
}


bool CEgon::Deploy()
{
	m_deployed = false;
	m_fireState = FIRE_OFF;
	return DefaultDeploy("models/v_egon.mdl", "models/p_egon.mdl", EGON_DRAW, "egon");
}

void CEgon::Holster()
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	SendWeaponAnim(EGON_HOLSTER);

	EndAttack();
}

bool CEgon::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "uranium";
	p->iMaxAmmo1 = URANIUM_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	p->iPosition = 2;
	p->iId = m_iId = WEAPON_EGON;
	p->iFlags = 0;
	p->iWeight = EGON_WEIGHT;

	return true;
}

#define EGON_PULSE_INTERVAL 0.1
#define EGON_DISCHARGE_INTERVAL 0.1

float CEgon::GetPulseInterval()
{
	return EGON_PULSE_INTERVAL;
}

float CEgon::GetDischargeInterval()
{
	return EGON_DISCHARGE_INTERVAL;
}

bool CEgon::HasAmmo()
{
	if (m_pPlayer->ammo_uranium <= 0)
		return false;

	return true;
}

void CEgon::UseAmmo(int count)
{
#ifndef CLIENT_DLL
	if (IsBustingGame())
		return;
#endif

	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] >= count)
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= count;
	else
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 0;
}

void CEgon::Attack()
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{

		if (m_fireState != FIRE_OFF || m_pBeam)
		{
			EndAttack();
		}
		else
		{
			PlayEmptySound();
		}
		return;
	}

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);
	Vector vecAiming = gpGlobals->v_forward;
	Vector vecSrc = m_pPlayer->GetGunPosition();

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	switch (m_fireState)
	{
	case FIRE_OFF:
	{
		if (!HasAmmo())
		{
			m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.25;
			PlayEmptySound();
			return;
		}

		m_flAmmoUseTime = gpGlobals->time; // start using ammo ASAP.

		PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), m_usEgonFire, 0.0, g_vecZero, g_vecZero, 0.0, 0.0, 0, m_fireMode, 1, 0);

		m_shakeTime = 0;

		m_pPlayer->m_iWeaponVolume = EGON_PRIMARY_VOLUME;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1;
		pev->fuser1 = UTIL_WeaponTimeBase() + 2;

		pev->dmgtime = gpGlobals->time + GetPulseInterval();
		m_fireState = FIRE_CHARGE;
	}
	break;

	case FIRE_CHARGE:
	{
		Fire(vecSrc, vecAiming);
		m_pPlayer->m_iWeaponVolume = EGON_PRIMARY_VOLUME;

		if (pev->fuser1 <= UTIL_WeaponTimeBase())
		{
			PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), m_usEgonFire, 0, g_vecZero, g_vecZero, 0.0, 0.0, 0, m_fireMode, 0, 0);
			pev->fuser1 = 1000;
		}

		if (!HasAmmo())
		{
			EndAttack();
			m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.0;
		}
	}
	break;
	}
}

void CEgon::PrimaryAttack()
{
	m_fireMode = FIRE_WIDE;
	Attack();
}

void CEgon::Fire(const Vector& vecOrigSrc, const Vector& vecDir)
{
	Vector vecDest = vecOrigSrc + vecDir * 2048;
	edict_t* pentIgnore;
	TraceResult tr;

	pentIgnore = m_pPlayer->edict();
	Vector tmpSrc = vecOrigSrc + gpGlobals->v_up * -8 + gpGlobals->v_right * 3;

	// ALERT( at_console, "." );

	UTIL_TraceLine(vecOrigSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr);

	if (0 != tr.fAllSolid)
		return;

#ifndef CLIENT_DLL
	CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);

	if (pEntity == NULL)
		return;

	if (g_pGameRules->IsMultiplayer())
	{
		if (m_pSprite && 0 != pEntity->pev->takedamage)
		{
			m_pSprite->pev->effects &= ~EF_NODRAW;
		}
		else if (m_pSprite)
		{
			m_pSprite->pev->effects |= EF_NODRAW;
		}
	}


#endif

	float timedist;

	switch (m_fireMode)
	{
	default:
	case FIRE_NARROW:
#ifndef CLIENT_DLL
		if (pev->dmgtime < gpGlobals->time)
		{
			// Narrow mode only does damage to the entity it hits
			ClearMultiDamage();
			if (0 != pEntity->pev->takedamage)
			{
				pEntity->TraceAttack(m_pPlayer->pev, gSkillData.plrDmgEgonNarrow, vecDir, &tr, DMG_ENERGYBEAM);
			}
			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);

			if (g_pGameRules->IsMultiplayer())
			{
				// multiplayer uses 1 ammo every 1/10th second
				if (gpGlobals->time >= m_flAmmoUseTime)
				{
					UseAmmo(1);
					m_flAmmoUseTime = gpGlobals->time + 0.1;
				}
			}
			else
			{
				// single player, use 3 ammo/second
				if (gpGlobals->time >= m_flAmmoUseTime)
				{
					UseAmmo(1);
					m_flAmmoUseTime = gpGlobals->time + 0.166;
				}
			}

			pev->dmgtime = gpGlobals->time + GetPulseInterval();
		}
#endif
		timedist = (pev->dmgtime - gpGlobals->time) / GetPulseInterval();
		break;

	case FIRE_WIDE:
#ifndef CLIENT_DLL
		if (pev->dmgtime < gpGlobals->time)
		{
			// wide mode does damage to the ent, and radius damage
			ClearMultiDamage();
			if (0 != pEntity->pev->takedamage)
			{
				pEntity->TraceAttack(m_pPlayer->pev, gSkillData.plrDmgEgonWide, vecDir, &tr, DMG_ENERGYBEAM | DMG_ALWAYSGIB);
			}
			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);

			if (g_pGameRules->IsMultiplayer())
			{
				// radius damage a little more potent in multiplayer.
				::RadiusDamage(tr.vecEndPos, pev, m_pPlayer->pev, gSkillData.plrDmgEgonWide / 4, 128, CLASS_NONE, DMG_ENERGYBEAM | DMG_BLAST | DMG_ALWAYSGIB);
			}

			if (!m_pPlayer->IsAlive())
				return;

			if (g_pGameRules->IsMultiplayer())
			{
				//multiplayer uses 5 ammo/second
				if (gpGlobals->time >= m_flAmmoUseTime)
				{
					UseAmmo(1);
					m_flAmmoUseTime = gpGlobals->time + 0.2;
				}
			}
			else
			{
				// Wide mode uses 10 charges per second in single player
				if (gpGlobals->time >= m_flAmmoUseTime)
				{
					UseAmmo(1);
					m_flAmmoUseTime = gpGlobals->time + 0.1;
				}
			}

			pev->dmgtime = gpGlobals->time + GetDischargeInterval();
			if (m_shakeTime < gpGlobals->time)
			{
				UTIL_ScreenShake(tr.vecEndPos, 5.0, 150.0, 0.75, 250.0);
				m_shakeTime = gpGlobals->time + 1.5;
			}
		}
#endif
		timedist = (pev->dmgtime - gpGlobals->time) / GetDischargeInterval();
		break;
	}

	if (timedist < 0)
		timedist = 0;
	else if (timedist > 1)
		timedist = 1;
	timedist = 1 - timedist;

	UpdateEffect(tmpSrc, tr.vecEndPos, timedist);
}


void CEgon::UpdateEffect(const Vector& startPoint, const Vector& endPoint, float timeBlend)
{
#ifndef CLIENT_DLL
	if (!m_pBeam)
	{
		CreateEffect();
	}

	m_pBeam->SetStartPos(endPoint);
	m_pBeam->SetBrightness(255 - (timeBlend * 180));
	m_pBeam->SetWidth(40 - (timeBlend * 20));

	if (m_fireMode == FIRE_WIDE)
		m_pBeam->SetColor(30 + (25 * timeBlend), 30 + (30 * timeBlend), 64 + 80 * fabs(sin(gpGlobals->time * 10)));
	else
		m_pBeam->SetColor(60 + (25 * timeBlend), 120 + (30 * timeBlend), 64 + 80 * fabs(sin(gpGlobals->time * 10)));


	UTIL_SetOrigin(m_pSprite->pev, endPoint);
	m_pSprite->pev->frame += 8 * gpGlobals->frametime;
	if (m_pSprite->pev->frame > m_pSprite->Frames())
		m_pSprite->pev->frame = 0;

	m_pNoise->SetStartPos(endPoint);

#endif
}

void CEgon::CreateEffect()
{

#ifndef CLIENT_DLL
	DestroyEffect();

	m_pBeam = CBeam::BeamCreate(EGON_BEAM_SPRITE, 40);
	m_pBeam->PointEntInit(pev->origin, m_pPlayer->entindex());
	m_pBeam->SetFlags(BEAM_FSINE);
	m_pBeam->SetEndAttachment(1);
	m_pBeam->pev->spawnflags |= SF_BEAM_TEMPORARY; // Flag these to be destroyed on save/restore or level transition
	m_pBeam->pev->flags |= FL_SKIPLOCALHOST;
	m_pBeam->pev->owner = m_pPlayer->edict();

	m_pNoise = CBeam::BeamCreate(EGON_BEAM_SPRITE, 55);
	m_pNoise->PointEntInit(pev->origin, m_pPlayer->entindex());
	m_pNoise->SetScrollRate(25);
	m_pNoise->SetBrightness(100);
	m_pNoise->SetEndAttachment(1);
	m_pNoise->pev->spawnflags |= SF_BEAM_TEMPORARY;
	m_pNoise->pev->flags |= FL_SKIPLOCALHOST;
	m_pNoise->pev->owner = m_pPlayer->edict();

	m_pSprite = CSprite::SpriteCreate(EGON_FLARE_SPRITE, pev->origin, false);
	m_pSprite->pev->scale = 1.0;
	m_pSprite->SetTransparency(kRenderGlow, 255, 255, 255, 255, kRenderFxNoDissipation);
	m_pSprite->pev->spawnflags |= SF_SPRITE_TEMPORARY;
	// Josh: This sprite is not predicted on the client, so was missing
	// for many years after it got broken in an update.
	// m_pSprite->pev->flags |= FL_SKIPLOCALHOST;
	m_pSprite->pev->owner = m_pPlayer->edict();

	if (m_fireMode == FIRE_WIDE)
	{
		m_pBeam->SetScrollRate(50);
		m_pBeam->SetNoise(20);
		m_pNoise->SetColor(50, 50, 255);
		m_pNoise->SetNoise(8);
	}
	else
	{
		m_pBeam->SetScrollRate(110);
		m_pBeam->SetNoise(5);
		m_pNoise->SetColor(80, 120, 255);
		m_pNoise->SetNoise(2);
	}
#endif
}


void CEgon::DestroyEffect()
{

#ifndef CLIENT_DLL
	if (m_pBeam)
	{
		UTIL_Remove(m_pBeam);
		m_pBeam = NULL;
	}
	if (m_pNoise)
	{
		UTIL_Remove(m_pNoise);
		m_pNoise = NULL;
	}
	if (m_pSprite)
	{
		if (m_fireMode == FIRE_WIDE)
			m_pSprite->Expand(10, 500);
		else
			UTIL_Remove(m_pSprite);
		m_pSprite = NULL;
	}
#endif
}



void CEgon::WeaponIdle()
{
	if ((m_pPlayer->m_afButtonPressed & IN_ATTACK2) == 0 && (m_pPlayer->pev->button & IN_ATTACK) != 0)
	{
		return;
	}

	ResetEmptySound();

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	if (m_fireState != FIRE_OFF)
		EndAttack();

	int iAnim;

	float flRand = RANDOM_FLOAT(0, 1);

	if (flRand <= 0.5)
	{
		iAnim = EGON_IDLE1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
	}
	else
	{
		iAnim = EGON_FIDGET1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
	}

	SendWeaponAnim(iAnim);
	m_deployed = true;
}

bool CEgon::CanHolster()
{
#ifndef CLIENT_DLL
	if (IsBustingGame())
	{
		return false;
	}
#endif

	return true;
}

void CEgon::EndAttack()
{
	bool bMakeNoise = false;

	if (m_fireState != FIRE_OFF) //Checking the button just in case!.
		bMakeNoise = true;

	PLAYBACK_EVENT_FULL(FEV_GLOBAL | FEV_RELIABLE, m_pPlayer->edict(), m_usEgonStop, 0, m_pPlayer->pev->origin, m_pPlayer->pev->angles, 0.0, 0.0,
		static_cast<int>(bMakeNoise), 0, 0, 0);

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0;

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;

	m_fireState = FIRE_OFF;

	DestroyEffect();
}



class CEgonAmmo : public CBasePlayerAmmo
{
	void Spawn() override
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_chainammo.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache() override
	{
		PRECACHE_MODEL("models/w_chainammo.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	bool AddAmmo(CBaseEntity* pOther) override
	{
		if (pOther->GiveAmmo(AMMO_URANIUMBOX_GIVE, "uranium", URANIUM_MAX_CARRY) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return true;
		}
		return false;
	}
};
LINK_ENTITY_TO_CLASS(ammo_egonclip, CEgonAmmo);
