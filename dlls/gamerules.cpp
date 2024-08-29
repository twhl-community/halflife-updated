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
// GameRules.cpp
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "skill.h"
#include "game.h"
#include "UserMessages.h"

extern edict_t* EntSelectSpawnPoint(CBaseEntity* pPlayer);

CBasePlayerItem* CGameRules::FindNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon)
{
	if (pCurrentWeapon != nullptr && !pCurrentWeapon->CanHolster())
	{
		// can't put this gun away right now, so can't switch.
		return nullptr;
	}

	const int currentWeight = pCurrentWeapon != nullptr ? pCurrentWeapon->iWeight() : -1;

	CBasePlayerItem* pBest = nullptr; // this will be used in the event that we don't find a weapon in the same category.

	int iBestWeight = -1; // no weapon lower than -1 can be autoswitched to

	for (int i = 0; i < MAX_ITEM_TYPES; i++)
	{
		for (auto pCheck = pPlayer->m_rgpPlayerItems[i]; pCheck; pCheck = pCheck->m_pNext)
		{
			// don't reselect the weapon we're trying to get rid of
			if (pCheck == pCurrentWeapon)
			{
				continue;
			}

			if ((pCheck->iFlags() & ITEM_FLAG_NOAUTOSWITCHTO) != 0)
			{
				continue;
			}

			if (pCheck->iWeight() > -1 && pCheck->iWeight() == currentWeight)
			{
				// this weapon is from the same category.
				if (pCheck->CanDeploy())
				{
					if (pPlayer->SwitchWeapon(pCheck))
					{
						return pCheck;
					}
				}
			}
			else if (pCheck->iWeight() > iBestWeight)
			{
				//ALERT ( at_console, "Considering %s\n", STRING( pCheck->pev->classname ) );
				// we keep updating the 'best' weapon just in case we can't find a weapon of the same weight
				// that the player was using. This will end up leaving the player with his heaviest-weighted
				// weapon.
				if (pCheck->CanDeploy())
				{
					// if this weapon is useable, flag it as the best
					iBestWeight = pCheck->iWeight();
					pBest = pCheck;
				}
			}
		}
	}

	// if we make it here, we've checked all the weapons and found no useable
	// weapon in the same catagory as the current weapon.

	// if pBest is nullptr, we didn't find ANYTHING. Shouldn't be possible- should always
	// at least get the crowbar, but ya never know.

	return pBest;
}

bool CGameRules::GetNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon, bool alwaysSearch)
{
	if (auto pBest = FindNextBestWeapon(pPlayer, pCurrentWeapon); pBest != nullptr)
	{
		pPlayer->SwitchWeapon(pBest);
		return true;
	}

	return false;
}

//=========================================================
//=========================================================
bool CGameRules::CanHaveAmmo(CBasePlayer* pPlayer, const char* pszAmmoName, int iMaxCarry)
{
	int iAmmoIndex;

	if (pszAmmoName)
	{
		iAmmoIndex = pPlayer->GetAmmoIndex(pszAmmoName);

		if (iAmmoIndex > -1)
		{
			if (pPlayer->AmmoInventory(iAmmoIndex) < iMaxCarry)
			{
				// player has room for more of this type of ammo
				return true;
			}
		}
	}

	return false;
}

//=========================================================
//=========================================================
edict_t* CGameRules::GetPlayerSpawnSpot(CBasePlayer* pPlayer)
{
	edict_t* pentSpawnSpot = EntSelectSpawnPoint(pPlayer);

	pPlayer->pev->origin = VARS(pentSpawnSpot)->origin + Vector(0, 0, 1);
	pPlayer->pev->v_angle = g_vecZero;
	pPlayer->pev->velocity = g_vecZero;
	pPlayer->pev->angles = VARS(pentSpawnSpot)->angles;
	pPlayer->pev->punchangle = g_vecZero;
	pPlayer->pev->fixangle = 1;

	return pentSpawnSpot;
}

//=========================================================
//=========================================================
bool CGameRules::CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon)
{
	// only living players can have items
	if (pPlayer->pev->deadflag != DEAD_NO)
		return false;

	if (pWeapon->pszAmmo1())
	{
		if (!CanHaveAmmo(pPlayer, pWeapon->pszAmmo1(), pWeapon->iMaxAmmo1()))
		{
			// we can't carry anymore ammo for this gun. We can only
			// have the gun if we aren't already carrying one of this type
			if (pPlayer->HasPlayerItem(pWeapon))
			{
				return false;
			}
		}
	}
	else
	{
		// weapon doesn't use ammo, don't take another if you already have it.
		if (pPlayer->HasPlayerItem(pWeapon))
		{
			return false;
		}
	}

	// note: will fall through to here if GetItemInfo doesn't fill the struct!
	return true;
}

//=========================================================
// load the SkillData struct with the proper values based on the skill level.
//=========================================================
void CGameRules::RefreshSkillData()
{
	int iSkill;

	iSkill = (int)CVAR_GET_FLOAT("skill");
	g_iSkillLevel = iSkill;

	if (iSkill < 1)
	{
		iSkill = 1;
	}
	else if (iSkill > 3)
	{
		iSkill = 3;
	}

	gSkillData.iSkillLevel = iSkill;

	ALERT(at_console, "\nGAME SKILL LEVEL:%d\n", iSkill);

	//Agrunt
	gSkillData.agruntHealth = GetSkillCvar("sk_agrunt_health");
	gSkillData.agruntDmgPunch = GetSkillCvar("sk_agrunt_dmg_punch");

	// Apache
	gSkillData.apacheHealth = GetSkillCvar("sk_apache_health");

	// Barney
	gSkillData.barneyHealth = GetSkillCvar("sk_barney_health");

	// Big Momma
	gSkillData.bigmommaHealthFactor = GetSkillCvar("sk_bigmomma_health_factor");
	gSkillData.bigmommaDmgSlash = GetSkillCvar("sk_bigmomma_dmg_slash");
	gSkillData.bigmommaDmgBlast = GetSkillCvar("sk_bigmomma_dmg_blast");
	gSkillData.bigmommaRadiusBlast = GetSkillCvar("sk_bigmomma_radius_blast");

	// Bullsquid
	gSkillData.bullsquidHealth = GetSkillCvar("sk_bullsquid_health");
	gSkillData.bullsquidDmgBite = GetSkillCvar("sk_bullsquid_dmg_bite");
	gSkillData.bullsquidDmgWhip = GetSkillCvar("sk_bullsquid_dmg_whip");
	gSkillData.bullsquidDmgSpit = GetSkillCvar("sk_bullsquid_dmg_spit");

	// Gargantua
	gSkillData.gargantuaHealth = GetSkillCvar("sk_gargantua_health");
	gSkillData.gargantuaDmgSlash = GetSkillCvar("sk_gargantua_dmg_slash");
	gSkillData.gargantuaDmgFire = GetSkillCvar("sk_gargantua_dmg_fire");
	gSkillData.gargantuaDmgStomp = GetSkillCvar("sk_gargantua_dmg_stomp");

	// Hassassin
	gSkillData.hassassinHealth = GetSkillCvar("sk_hassassin_health");

	// Headcrab
	gSkillData.headcrabHealth = GetSkillCvar("sk_headcrab_health");
	gSkillData.headcrabDmgBite = GetSkillCvar("sk_headcrab_dmg_bite");

	// Hgrunt
	gSkillData.hgruntHealth = GetSkillCvar("sk_hgrunt_health");
	gSkillData.hgruntDmgKick = GetSkillCvar("sk_hgrunt_kick");
	gSkillData.hgruntShotgunPellets = GetSkillCvar("sk_hgrunt_pellets");
	gSkillData.hgruntGrenadeSpeed = GetSkillCvar("sk_hgrunt_gspeed");

	// Houndeye
	gSkillData.houndeyeHealth = GetSkillCvar("sk_houndeye_health");
	gSkillData.houndeyeDmgBlast = GetSkillCvar("sk_houndeye_dmg_blast");

	// ISlave
	gSkillData.slaveHealth = GetSkillCvar("sk_islave_health");
	gSkillData.slaveDmgClaw = GetSkillCvar("sk_islave_dmg_claw");
	gSkillData.slaveDmgClawrake = GetSkillCvar("sk_islave_dmg_clawrake");
	gSkillData.slaveDmgZap = GetSkillCvar("sk_islave_dmg_zap");

	// Icthyosaur
	gSkillData.ichthyosaurHealth = GetSkillCvar("sk_ichthyosaur_health");
	gSkillData.ichthyosaurDmgShake = GetSkillCvar("sk_ichthyosaur_shake");

	// Leech
	gSkillData.leechHealth = GetSkillCvar("sk_leech_health");

	gSkillData.leechDmgBite = GetSkillCvar("sk_leech_dmg_bite");

	// Controller
	gSkillData.controllerHealth = GetSkillCvar("sk_controller_health");
	gSkillData.controllerDmgZap = GetSkillCvar("sk_controller_dmgzap");
	gSkillData.controllerSpeedBall = GetSkillCvar("sk_controller_speedball");
	gSkillData.controllerDmgBall = GetSkillCvar("sk_controller_dmgball");

	// Nihilanth
	gSkillData.nihilanthHealth = GetSkillCvar("sk_nihilanth_health");
	gSkillData.nihilanthZap = GetSkillCvar("sk_nihilanth_zap");

	// Scientist
	gSkillData.scientistHealth = GetSkillCvar("sk_scientist_health");

	// Snark
	gSkillData.snarkHealth = GetSkillCvar("sk_snark_health");
	gSkillData.snarkDmgBite = GetSkillCvar("sk_snark_dmg_bite");
	gSkillData.snarkDmgPop = GetSkillCvar("sk_snark_dmg_pop");

	// Zombie
	gSkillData.zombieHealth = GetSkillCvar("sk_zombie_health");
	gSkillData.zombieDmgOneSlash = GetSkillCvar("sk_zombie_dmg_one_slash");
	gSkillData.zombieDmgBothSlash = GetSkillCvar("sk_zombie_dmg_both_slash");

	//Turret
	gSkillData.turretHealth = GetSkillCvar("sk_turret_health");

	// MiniTurret
	gSkillData.miniturretHealth = GetSkillCvar("sk_miniturret_health");

	// Sentry Turret
	gSkillData.sentryHealth = GetSkillCvar("sk_sentry_health");

	// PLAYER WEAPONS

	// Crowbar whack
	gSkillData.plrDmgCrowbar = GetSkillCvar("sk_plr_crowbar");

	// Glock Round
	gSkillData.plrDmg9MM = GetSkillCvar("sk_plr_9mm_bullet");

	// 357 Round
	gSkillData.plrDmg357 = GetSkillCvar("sk_plr_357_bullet");

	// MP5 Round
	gSkillData.plrDmgMP5 = GetSkillCvar("sk_plr_9mmAR_bullet");

	// M203 grenade
	gSkillData.plrDmgM203Grenade = GetSkillCvar("sk_plr_9mmAR_grenade");

	// Shotgun buckshot
	gSkillData.plrDmgBuckshot = GetSkillCvar("sk_plr_buckshot");

	// Crossbow
	gSkillData.plrDmgCrossbowClient = GetSkillCvar("sk_plr_xbow_bolt_client");
	gSkillData.plrDmgCrossbowMonster = GetSkillCvar("sk_plr_xbow_bolt_monster");

	// RPG
	gSkillData.plrDmgRPG = GetSkillCvar("sk_plr_rpg");

	// Gauss gun
	gSkillData.plrDmgGauss = GetSkillCvar("sk_plr_gauss");

	// Egon Gun
	gSkillData.plrDmgEgonNarrow = GetSkillCvar("sk_plr_egon_narrow");
	gSkillData.plrDmgEgonWide = GetSkillCvar("sk_plr_egon_wide");

	// Hand Grendade
	gSkillData.plrDmgHandGrenade = GetSkillCvar("sk_plr_hand_grenade");

	// Satchel Charge
	gSkillData.plrDmgSatchel = GetSkillCvar("sk_plr_satchel");

	// Tripmine
	gSkillData.plrDmgTripmine = GetSkillCvar("sk_plr_tripmine");

	// MONSTER WEAPONS
	gSkillData.monDmg12MM = GetSkillCvar("sk_12mm_bullet");
	gSkillData.monDmgMP5 = GetSkillCvar("sk_9mmAR_bullet");
	gSkillData.monDmg9MM = GetSkillCvar("sk_9mm_bullet");

	// MONSTER HORNET
	gSkillData.monDmgHornet = GetSkillCvar("sk_hornet_dmg");

	// PLAYER HORNET
	gSkillData.plrDmgHornet = GetSkillCvar("sk_plr_hornet_dmg");


	// HEALTH/CHARGE
	gSkillData.suitchargerCapacity = GetSkillCvar("sk_suitcharger");
	gSkillData.batteryCapacity = GetSkillCvar("sk_battery");
	gSkillData.healthchargerCapacity = GetSkillCvar("sk_healthcharger");
	gSkillData.healthkitCapacity = GetSkillCvar("sk_healthkit");
	gSkillData.scientistHeal = GetSkillCvar("sk_scientist_heal");

	// monster damage adj
	gSkillData.monHead = GetSkillCvar("sk_monster_head");
	gSkillData.monChest = GetSkillCvar("sk_monster_chest");
	gSkillData.monStomach = GetSkillCvar("sk_monster_stomach");
	gSkillData.monLeg = GetSkillCvar("sk_monster_leg");
	gSkillData.monArm = GetSkillCvar("sk_monster_arm");

	// player damage adj
	gSkillData.plrHead = GetSkillCvar("sk_player_head");
	gSkillData.plrChest = GetSkillCvar("sk_player_chest");
	gSkillData.plrStomach = GetSkillCvar("sk_player_stomach");
	gSkillData.plrLeg = GetSkillCvar("sk_player_leg");
	gSkillData.plrArm = GetSkillCvar("sk_player_arm");
}

//=========================================================
// instantiate the proper game rules object
//=========================================================

CGameRules* InstallGameRules()
{
	SERVER_COMMAND("exec game.cfg\n");
	SERVER_EXECUTE();

	if (1 == sv_busters.value)
	{
		g_teamplay = false;
		return new CMultiplayBusters;
	}
	else if (0 == gpGlobals->deathmatch)
	{
		// generic half-life
		g_teamplay = false;
		return new CHalfLifeRules;
	}
	else
	{
		if (teamplay.value > 0)
		{
			// teamplay

			g_teamplay = true;
			return new CHalfLifeTeamplay;
		}
		if ((int)gpGlobals->deathmatch == 1)
		{
			// vanilla deathmatch
			g_teamplay = false;
			return new CHalfLifeMultiplay;
		}
		else
		{
			// vanilla deathmatch??
			g_teamplay = false;
			return new CHalfLifeMultiplay;
		}
	}
}
