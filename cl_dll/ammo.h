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

#define MAX_WEAPON_NAME 128


#define WEAPON_FLAGS_SELECTONEMPTY 1

#define WEAPON_IS_ONTARGET 0x40

struct WEAPON
{
	char szName[MAX_WEAPON_NAME];
	int iAmmoType;
	int iAmmo2Type;
	int iMax1;
	int iMax2;
	int iSlot;
	int iSlotPos;
	int iFlags;
	int iId;
	int iClip;

	int iCount; // # of itesm in plist

	HSPRITE hActive;
	Rect rcActive;
	HSPRITE hInactive;
	Rect rcInactive;
	HSPRITE hAmmo;
	Rect rcAmmo;
	HSPRITE hAmmo2;
	Rect rcAmmo2;
	HSPRITE hCrosshair;
	Rect rcCrosshair;
	HSPRITE hAutoaim;
	Rect rcAutoaim;
	HSPRITE hZoomedCrosshair;
	Rect rcZoomedCrosshair;
	HSPRITE hZoomedAutoaim;
	Rect rcZoomedAutoaim;
};
