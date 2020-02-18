// Desert Eagle implemator
// written by Sabian Roberts for The Phoenix Project @ Heatwave Studios
// based on code written by Penguinboy @ The Whole Half-Life
// Contact at https://github.com/sabian-heatwave

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"

// These correspond directly to the sequences in the weapon's view model
enum desert_eagle_e {
    DESERT_EAGLE_IDLE1 = 0,
    DESERT_EAGLE_IDLE2,
    DESERT_EAGLE_IDLE3,
    DESERT_EAGLE_IDLE4,
    DESERT_EAGLE_IDLE5,
    DESERT_EAGLE_SHOOT,
    DESERT_EAGLE_SHOOT_EMPTY,
    DESERT_EAGLE_RELOAD,
    DESERT_EAGLE_RELOAD_NOT_EMPTY,
    DESERT_EAGLE_DRAW,
    DESERT_EAGLE_HOLSTER,
};

LINK_ENTITY_TO_CLASS(weapon_eagle, CDesertEagle)

void CDesertEagle::Spawn()
{
    // Define the classname of the entity
    // This is the name you should use to reference this entity name in your code base.
    pev->classname = MAKE_STRING("weapon_eagle");

    // Precache the weapon models and sounds
    // This might be called by the engine separately, but it's best to call it here as well just in case.
    Precache();

    // Set the weapon ID
    m_iId = WEAPON_DESERT_EAGLE;

    // Tell the engine about the weapon's world model
    SET_MODEL(ENT(pev), "models/w_desert_eagle.mdl");

    // Set the default ammo value for the weapon
    m_iDefaultAmmo = DESERT_EAGLE_DEFAULT_GIVE;

    // Set up some default behaviour for the weapon
    // This will tell the engine that the weapon should "fall" to the ground when it spawns.
    // It also sets up the behaviour so that the weapon is equipped when the player touches it.
    FallInit();
}

void CDesertEagle::Precache()
{
    // Precache models
    PRECACHE_MODEL("models/v_desert_eagle.mdl");
    PRECACHE_MODEL("models/w_desert_eagle.mdl");
    PRECACHE_MODEL("models/p_desert_eagle.mdl");
    PRECACHE_MODEL("models/shell.mdl");

    // Precache sounds
    PRECACHE_SOUND("weapons/desert_eagle_fire.wav");
    PRECACHE_SOUND("weapons/desert_eagle_sight.wav");
    PRECACHE_SOUND("weapons/desert_eagle_sight2.wav");
}

int CDesertEagle::GetItemInfo(ItemInfo* p)
{
    return 0;
}

int CDesertEagle::AddToPlayer(CBasePlayer* pPlayer)
{
    return FALSE;
}

void CDesertEagle::SecondaryAttack()
{

}

void CDesertEagle::PrimaryAttack()
{

}

BOOL CDesertEagle::Deploy()
{
    return FALSE;
}

void CDesertEagle::Holster(int skiplocal)
{

}

void CDesertEagle::Reload()
{

}

void CDesertEagle::WeaponIdle()
{

}