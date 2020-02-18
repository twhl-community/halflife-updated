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
    // This should match the classname - the HUD uses it to find the matching .txt file in the sprites/ folder
    p->pszName = STRING(pev->classname);

    // The "primary" ammo type for this weapon and the maximum ammo of that type that the player can hold
    p->pszAmmo1 = "357";
    p->iMaxAmmo1 = _357_MAX_CARRY;

    // Same as above, but for "secondary" ammo. This should be NULL and -1 for weapons with no secondary
    p->pszAmmo2 = NULL;
    p->iMaxAmmo2 = -1;

    // The size of a full clip
    p->iMaxClip = DESERT_EAGLE_MAX_CLIP;

    // Special weapon flags - leave this as 0 for now, this is covered in a different article
    p->iFlags = 0;

    // The "slot" in the HUD that the weapon appears in. This is a pistol, so it goes into slot 1 with the others
    p->iSlot = 1;

    // The "position" in the HUD that the weapon is added to. We'll put this after the magnum (which is in slot 1)
    p->iPosition = 2;

    // Set the ID and auto-switching weights of the weapon
    p->iId = m_iId = WEAPON_DESERT_EAGLE;
    p->iWeight = DESERT_EAGLE_WEIGHT;

    return TRUE;
}

int CDesertEagle::AddToPlayer(CBasePlayer* pPlayer)
{
    // AddToPlayer returns TRUE if the weapon was picked up
    if (CBasePlayerWeapon::AddToPlayer(pPlayer))
    {
        // Send a message to the client so the pickup icon can be shown
        MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
        WRITE_BYTE(m_iId);
        MESSAGE_END();
        return TRUE;
    }
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
    return DefaultDeploy(
        "models/v_desert_eagle.mdl", // Weapon view model
        "models/p_desert_eagle.mdl", // Weapon player model
        DESERT_EAGLE_DRAW, // "Draw" animation index for the view model
        "onehanded",       // Third person animation set for the weapon. We'll use the generic "onehanded" animation set
        UseDecrement(),    // Flag whether or not to do client prediction or not
        pev->body          // The weapon model's "body" pointer
);
    return FALSE;
}

void CDesertEagle::Holster(int skiplocal)
{
    // Cancel any reload in progress
    m_fInReload = FALSE;

    // Delay the next player's attack for about the same time as the holster animation takes
    m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;

    // Play the "holster" animation
    SendWeaponAnim(DESERT_EAGLE_HOLSTER);
}

void CDesertEagle::Reload()
{

}

void CDesertEagle::WeaponIdle()
{

}