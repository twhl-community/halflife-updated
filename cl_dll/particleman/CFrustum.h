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

enum FrustumSide
{
	RIGHT = 0,
	LEFT = 1,
	BOTTOM = 2,
	TOP = 3,
	BACK = 4,
	FRONT = 5,
};

struct CFrustum
{
	void CalculateFrustum();

	bool PointInsideFrustum(float x, float y, float z);

	bool SphereInsideFrustum(float x, float y, float z, float radius);

	bool PlaneInsideFrustum(float x, float y, float z, float size);

private:
	void NormalizeFrustumPlane(float frustum[6][4], int side);

public:
	float g_flFrustum[6][4];
};
