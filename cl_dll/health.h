/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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

#pragma once

#define DMG_IMAGE_LIFE 2 // seconds that image is up

enum
{
	DMG_IMAGE_POISON = 0,
	DMG_IMAGE_ACID,
	DMG_IMAGE_COLD,
	DMG_IMAGE_DROWN,
	DMG_IMAGE_BURN,
	DMG_IMAGE_NERVE,
	DMG_IMAGE_RAD,
	DMG_IMAGE_SHOCK,
	NUM_DMG_TYPES,
};
// instant damage

typedef struct
{
	float fExpire;
	float fBaseline;
	int x, y;
} DAMAGE_IMAGE;

//
//-----------------------------------------------------
//
class CHudHealth : public CHudBase
{
public:
	bool Init() override;
	bool VidInit() override;
	bool Draw(float fTime) override;
	void Reset() override;
	bool MsgFunc_Health(const char* pszName, int iSize, void* pbuf);
	bool MsgFunc_Damage(const char* pszName, int iSize, void* pbuf);
	int m_iHealth;
	int m_HUD_dmg_bio;
	int m_HUD_cross;
	float m_fAttackFront, m_fAttackRear, m_fAttackLeft, m_fAttackRight;
	void GetPainColor(int& r, int& g, int& b);
	float m_fFade;

private:
	HSPRITE m_hSprite;
	HSPRITE m_hDamage;

	DAMAGE_IMAGE m_dmg[NUM_DMG_TYPES];
	int m_bitsDamage;
	bool DrawPain(float fTime);
	bool DrawDamage(float fTime);
	void CalcDamageDirection(Vector vecFrom);
	void UpdateTiles(float fTime, long bits);
};
