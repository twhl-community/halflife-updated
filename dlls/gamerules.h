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
// GameRules
//=========================================================

#pragma once

//#include "weapons.h"
//#include "items.h"
class CBasePlayerItem;
class CBasePlayer;
class CItem;
class CBasePlayerAmmo;

// weapon respawning return codes
enum
{
	GR_NONE = 0,

	GR_WEAPON_RESPAWN_YES,
	GR_WEAPON_RESPAWN_NO,

	GR_AMMO_RESPAWN_YES,
	GR_AMMO_RESPAWN_NO,

	GR_ITEM_RESPAWN_YES,
	GR_ITEM_RESPAWN_NO,

	GR_PLR_DROP_GUN_ALL,
	GR_PLR_DROP_GUN_ACTIVE,
	GR_PLR_DROP_GUN_NO,

	GR_PLR_DROP_AMMO_ALL,
	GR_PLR_DROP_AMMO_ACTIVE,
	GR_PLR_DROP_AMMO_NO,
};

// Player relationship return codes
enum
{
	GR_NOTTEAMMATE = 0,
	GR_TEAMMATE,
	GR_ENEMY,
	GR_ALLY,
	GR_NEUTRAL,
};

class CGameRules
{
public:
	virtual void RefreshSkillData();						 // fill skill data struct with proper values
	virtual void Think() = 0;								 // GR_Think - runs every server frame, should handle any timer tasks, periodic events, etc.
	virtual bool IsAllowedToSpawn(CBaseEntity* pEntity) = 0; // Can this item spawn (eg monsters don't spawn in deathmatch).

	virtual bool FAllowFlashlight() = 0;																			  // Are players allowed to switch on their flashlight?
	virtual bool FShouldSwitchWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) = 0;							  // should the player switch to this weapon?
	virtual bool GetNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon, bool alwaysSearch = false); // I can't use this weapon anymore, get me the next best one.

	// Functions to verify the single/multiplayer status of a game
	virtual bool IsMultiplayer() = 0;								 // is this a multiplayer game? (either coop or deathmatch)
	virtual bool IsDeathmatch() = 0;								 //is this a deathmatch game?
	virtual bool IsTeamplay() { return false; }						 // is this deathmatch game being played with team rules?
	virtual bool IsCoOp() = 0;										 // is this a coop game?
	virtual const char* GetGameDescription() { return "Half-Life"; } // this is the game name that gets seen in the server browser

	// Client connection/disconnection
	virtual bool ClientConnected(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) = 0; // a client just connected to the server (player hasn't spawned yet)
	virtual void InitHUD(CBasePlayer* pl) = 0;																				   // the client dll is ready for updating
	virtual void ClientDisconnected(edict_t* pClient) = 0;																	   // a client just disconnected from the server
	virtual void UpdateGameMode(CBasePlayer* pPlayer) {}																	   // the client needs to be informed of the current game mode

	// Client damage rules
	virtual float FlPlayerFallDamage(CBasePlayer* pPlayer) = 0;										 // this client just hit the ground after a fall. How much damage?
	virtual bool FPlayerCanTakeDamage(CBasePlayer* pPlayer, CBaseEntity* pAttacker) { return true; } // can this player take damage from this attacker?
	virtual bool ShouldAutoAim(CBasePlayer* pPlayer, edict_t* target) { return true; }

	// Client spawn/respawn control
	virtual void PlayerSpawn(CBasePlayer* pPlayer) = 0;		   // called by CBasePlayer::Spawn just before releasing player into the game
	virtual void PlayerThink(CBasePlayer* pPlayer) = 0;		   // called by CBasePlayer::PreThink every frame, before physics are run and after keys are accepted
	virtual bool FPlayerCanRespawn(CBasePlayer* pPlayer) = 0;  // is this player allowed to respawn now?
	virtual float FlPlayerSpawnTime(CBasePlayer* pPlayer) = 0; // When in the future will this player be able to spawn?
	virtual edict_t* GetPlayerSpawnSpot(CBasePlayer* pPlayer); // Place this player on their spawnspot and face them the proper direction.

	virtual bool AllowAutoTargetCrosshair() { return true; }
	virtual bool ClientCommand(CBasePlayer* pPlayer, const char* pcmd) { return false; } // handles the user commands;  returns true if command handled properly
	virtual void ClientUserInfoChanged(CBasePlayer* pPlayer, char* infobuffer) {}		 // the player has changed userinfo;  can change it now

	// Client kills/scoring
	virtual int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) = 0;					// how many points do I award whoever kills this player?
	virtual void PlayerKilled(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) = 0; // Called each time a player dies
	virtual void DeathNotice(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) = 0;	// Call this from within a GameRules class to report an obituary.
																									// Weapon retrieval
	virtual bool CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon);					// The player is touching an CBasePlayerItem, do I give it to him?
	virtual void PlayerGotWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) = 0;				// Called each time a player picks up a weapon from the ground

	// Weapon spawn/respawn control
	virtual int WeaponShouldRespawn(CBasePlayerItem* pWeapon) = 0;	   // should this weapon respawn?
	virtual float FlWeaponRespawnTime(CBasePlayerItem* pWeapon) = 0;   // when may this weapon respawn?
	virtual float FlWeaponTryRespawn(CBasePlayerItem* pWeapon) = 0;	   // can i respawn now,  and if not, when should i try again?
	virtual Vector VecWeaponRespawnSpot(CBasePlayerItem* pWeapon) = 0; // where in the world should this weapon respawn?

	// Item retrieval
	virtual bool CanHaveItem(CBasePlayer* pPlayer, CItem* pItem) = 0;	// is this player allowed to take this item?
	virtual void PlayerGotItem(CBasePlayer* pPlayer, CItem* pItem) = 0; // call each time a player picks up an item (battery, healthkit, longjump)

	// Item spawn/respawn control
	virtual int ItemShouldRespawn(CItem* pItem) = 0;	 // Should this item respawn?
	virtual float FlItemRespawnTime(CItem* pItem) = 0;	 // when may this item respawn?
	virtual Vector VecItemRespawnSpot(CItem* pItem) = 0; // where in the world should this item respawn?

	// Ammo retrieval
	virtual bool CanHaveAmmo(CBasePlayer* pPlayer, const char* pszAmmoName, int iMaxCarry); // can this player take more of this ammo?
	virtual void PlayerGotAmmo(CBasePlayer* pPlayer, char* szName, int iCount) = 0;			// called each time a player picks up some ammo in the world

	// Ammo spawn/respawn control
	virtual int AmmoShouldRespawn(CBasePlayerAmmo* pAmmo) = 0;	   // should this ammo item respawn?
	virtual float FlAmmoRespawnTime(CBasePlayerAmmo* pAmmo) = 0;   // when should this ammo item respawn?
	virtual Vector VecAmmoRespawnSpot(CBasePlayerAmmo* pAmmo) = 0; // where in the world should this ammo item respawn?
																   // by default, everything spawns

	// Healthcharger respawn control
	virtual float FlHealthChargerRechargeTime() = 0;	   // how long until a depleted HealthCharger recharges itself?
	virtual float FlHEVChargerRechargeTime() { return 0; } // how long until a depleted HealthCharger recharges itself?

	// What happens to a dead player's weapons
	virtual int DeadPlayerWeapons(CBasePlayer* pPlayer) = 0; // what do I do with a player's weapons when he's killed?

	// What happens to a dead player's ammo
	virtual int DeadPlayerAmmo(CBasePlayer* pPlayer) = 0; // Do I drop ammo when the player dies? How much?

	// Teamplay stuff
	virtual const char* GetTeamID(CBaseEntity* pEntity) = 0;						// what team is this entity on?
	virtual int PlayerRelationship(CBaseEntity* pPlayer, CBaseEntity* pTarget) = 0; // What is the player's relationship with this entity?
	virtual int GetTeamIndex(const char* pTeamName) { return -1; }
	virtual const char* GetIndexedTeamName(int teamIndex) { return ""; }
	virtual bool IsValidTeam(const char* pTeamName) { return true; }
	virtual void ChangePlayerTeam(CBasePlayer* pPlayer, const char* pTeamName, bool bKill, bool bGib) {}
	virtual const char* SetDefaultPlayerTeam(CBasePlayer* pPlayer) { return ""; }

	// Sounds
	virtual bool PlayTextureSounds() { return true; }
	virtual bool PlayFootstepSounds(CBasePlayer* pl, float fvol) { return true; }

	// Monsters
	virtual bool FAllowMonsters() = 0; //are monsters allowed

	// Immediately end a multiplayer game
	virtual void EndMultiplayerGame() {}

protected:
	CBasePlayerItem* FindNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon);
};

extern CGameRules* InstallGameRules();


//=========================================================
// CHalfLifeRules - rules for the single player Half-Life
// game.
//=========================================================
class CHalfLifeRules : public CGameRules
{
public:
	CHalfLifeRules();

	// GR_Think
	void Think() override;
	bool IsAllowedToSpawn(CBaseEntity* pEntity) override;
	bool FAllowFlashlight() override { return true; }

	bool FShouldSwitchWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override;
	bool GetNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon, bool alwaysSearch = false) override;

	// Functions to verify the single/multiplayer status of a game
	bool IsMultiplayer() override;
	bool IsDeathmatch() override;
	bool IsCoOp() override;

	// Client connection/disconnection
	bool ClientConnected(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) override;
	void InitHUD(CBasePlayer* pl) override; // the client dll is ready for updating
	void ClientDisconnected(edict_t* pClient) override;

	// Client damage rules
	float FlPlayerFallDamage(CBasePlayer* pPlayer) override;

	// Client spawn/respawn control
	void PlayerSpawn(CBasePlayer* pPlayer) override;
	void PlayerThink(CBasePlayer* pPlayer) override;
	bool FPlayerCanRespawn(CBasePlayer* pPlayer) override;
	float FlPlayerSpawnTime(CBasePlayer* pPlayer) override;

	bool AllowAutoTargetCrosshair() override;

	// Client kills/scoring
	int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) override;
	void PlayerKilled(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) override;
	void DeathNotice(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) override;

	// Weapon retrieval
	void PlayerGotWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override;

	// Weapon spawn/respawn control
	int WeaponShouldRespawn(CBasePlayerItem* pWeapon) override;
	float FlWeaponRespawnTime(CBasePlayerItem* pWeapon) override;
	float FlWeaponTryRespawn(CBasePlayerItem* pWeapon) override;
	Vector VecWeaponRespawnSpot(CBasePlayerItem* pWeapon) override;

	// Item retrieval
	bool CanHaveItem(CBasePlayer* pPlayer, CItem* pItem) override;
	void PlayerGotItem(CBasePlayer* pPlayer, CItem* pItem) override;

	// Item spawn/respawn control
	int ItemShouldRespawn(CItem* pItem) override;
	float FlItemRespawnTime(CItem* pItem) override;
	Vector VecItemRespawnSpot(CItem* pItem) override;

	// Ammo retrieval
	void PlayerGotAmmo(CBasePlayer* pPlayer, char* szName, int iCount) override;

	// Ammo spawn/respawn control
	int AmmoShouldRespawn(CBasePlayerAmmo* pAmmo) override;
	float FlAmmoRespawnTime(CBasePlayerAmmo* pAmmo) override;
	Vector VecAmmoRespawnSpot(CBasePlayerAmmo* pAmmo) override;

	// Healthcharger respawn control
	float FlHealthChargerRechargeTime() override;

	// What happens to a dead player's weapons
	int DeadPlayerWeapons(CBasePlayer* pPlayer) override;

	// What happens to a dead player's ammo
	int DeadPlayerAmmo(CBasePlayer* pPlayer) override;

	// Monsters
	bool FAllowMonsters() override;

	// Teamplay stuff
	const char* GetTeamID(CBaseEntity* pEntity) override { return ""; }
	int PlayerRelationship(CBaseEntity* pPlayer, CBaseEntity* pTarget) override;
};

//=========================================================
// CHalfLifeMultiplay - rules for the basic half life multiplayer
// competition
//=========================================================
class CHalfLifeMultiplay : public CGameRules
{
public:
	CHalfLifeMultiplay();

	// GR_Think
	void Think() override;
	void RefreshSkillData() override;
	bool IsAllowedToSpawn(CBaseEntity* pEntity) override;
	bool FAllowFlashlight() override;

	bool FShouldSwitchWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override;

	// Functions to verify the single/multiplayer status of a game
	bool IsMultiplayer() override;
	bool IsDeathmatch() override;
	bool IsCoOp() override;

	// Client connection/disconnection
	// If ClientConnected returns false, the connection is rejected and the user is provided the reason specified in
	//  svRejectReason
	// Only the client's name and remote address are provided to the dll for verification.
	bool ClientConnected(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) override;
	void InitHUD(CBasePlayer* pl) override; // the client dll is ready for updating
	void ClientDisconnected(edict_t* pClient) override;
	void UpdateGameMode(CBasePlayer* pPlayer) override; // the client needs to be informed of the current game mode

	// Client damage rules
	float FlPlayerFallDamage(CBasePlayer* pPlayer) override;
	bool FPlayerCanTakeDamage(CBasePlayer* pPlayer, CBaseEntity* pAttacker) override;

	// Client spawn/respawn control
	void PlayerSpawn(CBasePlayer* pPlayer) override;
	void PlayerThink(CBasePlayer* pPlayer) override;
	bool FPlayerCanRespawn(CBasePlayer* pPlayer) override;
	float FlPlayerSpawnTime(CBasePlayer* pPlayer) override;
	edict_t* GetPlayerSpawnSpot(CBasePlayer* pPlayer) override;

	bool AllowAutoTargetCrosshair() override;
	bool ClientCommand(CBasePlayer* pPlayer, const char* pcmd) override;
	void ClientUserInfoChanged(CBasePlayer* pPlayer, char* infobuffer) override;

	// Client kills/scoring
	int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) override;
	void PlayerKilled(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) override;
	void DeathNotice(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) override;

	// Weapon retrieval
	void PlayerGotWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override;
	bool CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override; // The player is touching an CBasePlayerItem, do I give it to him?

	// Weapon spawn/respawn control
	int WeaponShouldRespawn(CBasePlayerItem* pWeapon) override;
	float FlWeaponRespawnTime(CBasePlayerItem* pWeapon) override;
	float FlWeaponTryRespawn(CBasePlayerItem* pWeapon) override;
	Vector VecWeaponRespawnSpot(CBasePlayerItem* pWeapon) override;

	// Item retrieval
	bool CanHaveItem(CBasePlayer* pPlayer, CItem* pItem) override;
	void PlayerGotItem(CBasePlayer* pPlayer, CItem* pItem) override;

	// Item spawn/respawn control
	int ItemShouldRespawn(CItem* pItem) override;
	float FlItemRespawnTime(CItem* pItem) override;
	Vector VecItemRespawnSpot(CItem* pItem) override;

	// Ammo retrieval
	void PlayerGotAmmo(CBasePlayer* pPlayer, char* szName, int iCount) override;

	// Ammo spawn/respawn control
	int AmmoShouldRespawn(CBasePlayerAmmo* pAmmo) override;
	float FlAmmoRespawnTime(CBasePlayerAmmo* pAmmo) override;
	Vector VecAmmoRespawnSpot(CBasePlayerAmmo* pAmmo) override;

	// Healthcharger respawn control
	float FlHealthChargerRechargeTime() override;
	float FlHEVChargerRechargeTime() override;

	// What happens to a dead player's weapons
	int DeadPlayerWeapons(CBasePlayer* pPlayer) override;

	// What happens to a dead player's ammo
	int DeadPlayerAmmo(CBasePlayer* pPlayer) override;

	// Teamplay stuff
	const char* GetTeamID(CBaseEntity* pEntity) override { return ""; }
	int PlayerRelationship(CBaseEntity* pPlayer, CBaseEntity* pTarget) override;

	bool PlayTextureSounds() override { return false; }
	bool PlayFootstepSounds(CBasePlayer* pl, float fvol) override;

	// Monsters
	bool FAllowMonsters() override;

	// Immediately end a multiplayer game
	void EndMultiplayerGame() override { GoToIntermission(); }

protected:
	virtual void ChangeLevel();
	virtual void GoToIntermission();
	float m_flIntermissionStartTime = 0;
	float m_flIntermissionEndTime = 0;
	bool m_iEndIntermissionButtonHit;
	void SendMOTDToClient(edict_t* client);
};

//=========================================================
// CMultiplayBusters
// Rules for a multiplayer mode that makes you feel good
//=========================================================
class CMultiplayBusters : public CHalfLifeMultiplay
{
public:
	CMultiplayBusters();

	void Think();
	int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled);
	void PlayerKilled(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor);
	void DeathNotice(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor);
	int WeaponShouldRespawn(CBasePlayerItem* pWeapon);
	bool CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon);
	bool CanHaveItem(CBasePlayer* pPlayer, CItem* pItem);
	void PlayerGotWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon);
	void ClientUserInfoChanged(CBasePlayer* pPlayer, char* infobuffer);
	void PlayerSpawn(CBasePlayer* pPlayer);

	void SetPlayerModel(CBasePlayer* pPlayer);

protected:
	float m_flEgonBustingCheckTime = -1.0f;
	void CheckForEgons();
};

inline DLL_GLOBAL CGameRules* g_pGameRules = nullptr;
inline DLL_GLOBAL bool g_fGameOver;
inline bool g_teamplay = false;
