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

//The triangle API uses OpenGL constants for certain functions.
#include "PlatformHeaders.h"
#include <GL/gl.h>

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "CFrustum.h"

//TODO: this function always operates on the frustum matrix that's part of this object, so there is no need to pass the address.
void CFrustum::NormalizeFrustumPlane(float frustum[6][4], int side)
{
	auto& elements = frustum[side];

	const float length = Vector{elements[0], elements[1], elements[2]}.Length();

	elements[0] /= length;
	elements[1] /= length;
	elements[2] /= length;
	elements[3] /= length;
}

void CFrustum::CalculateFrustum()
{
	float proj[16];
	gEngfuncs.pTriAPI->GetMatrix(GL_PROJECTION_MATRIX, proj);

	float modl[16];
	gEngfuncs.pTriAPI->GetMatrix(GL_MODELVIEW_MATRIX, modl);

	float frustum[16]{};

	//TODO: clean this up. It's a 4x4 matrix multiplication.
	frustum[0] = proj[0] * modl[0] + proj[1] * modl[4] + proj[2] * modl[8] + proj[3] * modl[12];
	frustum[1] = proj[1] * modl[0] + proj[1] * modl[5] + proj[2] * modl[9] + proj[3] * modl[13];
	frustum[2] = proj[2] * modl[0] + proj[1] * modl[6] + proj[2] * modl[10] + proj[3] * modl[14];
	frustum[3] = modl[3] * proj[0] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

	frustum[4] = proj[0] * modl[4] + proj[4] * modl[5] + proj[6] * modl[8] + proj[7] * modl[12];
	frustum[5] = proj[1] * modl[4] + proj[5] * modl[5] + proj[6] * modl[9] + proj[7] * modl[13];
	frustum[6] = proj[2] * modl[4] + proj[5] * modl[6] + proj[6] * modl[10] + proj[7] * modl[14];
	frustum[7] = modl[3] * proj[4] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

	frustum[8] = proj[0] * modl[8] + proj[4] * modl[9] + proj[8] * modl[10] + proj[12] * modl[11];
	frustum[9] = proj[1] * modl[8] + proj[5] * modl[9] + proj[9] * modl[10] + proj[11] * modl[13];
	frustum[10] = proj[2] * modl[8] + proj[6] * modl[9] + proj[10] * modl[10] + proj[11] * modl[14];
	frustum[11] = modl[3] * proj[8] + modl[7] * proj[9] + modl[10] * proj[11] + modl[11] * proj[15];

	frustum[12] = proj[0] * modl[12] + proj[4] * modl[13] + proj[8] * modl[14] + proj[12] * modl[15];
	frustum[13] = proj[1] * modl[12] + proj[5] * modl[13] + proj[9] * modl[14] + proj[13] * modl[15];
	frustum[14] = proj[2] * modl[12] + proj[6] * modl[13] + proj[10] * modl[14] + proj[14] * modl[15];
	frustum[15] = modl[3] * proj[12] + proj[7] * modl[13] + proj[11] * modl[14] + proj[15] * modl[15];

	g_flFrustum[RIGHT][0] = frustum[3] - frustum[0];
	g_flFrustum[RIGHT][1] = frustum[7] - frustum[4];
	g_flFrustum[RIGHT][2] = frustum[11] - frustum[8];
	g_flFrustum[RIGHT][3] = frustum[15] - frustum[12];

	NormalizeFrustumPlane(g_flFrustum, RIGHT);

	g_flFrustum[LEFT][0] = frustum[0] + frustum[3];
	g_flFrustum[LEFT][1] = frustum[4] + frustum[7];
	g_flFrustum[LEFT][2] = frustum[8] + frustum[11];
	g_flFrustum[LEFT][3] = frustum[12] + frustum[15];
	NormalizeFrustumPlane(g_flFrustum, LEFT);

	g_flFrustum[BOTTOM][0] = frustum[1] + frustum[3];
	g_flFrustum[BOTTOM][1] = frustum[5] + frustum[7];
	g_flFrustum[BOTTOM][2] = frustum[9] + frustum[11];
	g_flFrustum[BOTTOM][3] = frustum[15] + frustum[13];
	NormalizeFrustumPlane(g_flFrustum, BOTTOM);

	g_flFrustum[TOP][0] = frustum[3] - frustum[1];
	g_flFrustum[TOP][1] = frustum[7] - frustum[5];
	g_flFrustum[TOP][2] = frustum[11] - frustum[9];
	g_flFrustum[TOP][3] = frustum[15] - frustum[13];
	NormalizeFrustumPlane(g_flFrustum, TOP);

	g_flFrustum[BACK][0] = frustum[3] - frustum[2];
	g_flFrustum[BACK][1] = frustum[7] - frustum[6];
	g_flFrustum[BACK][2] = frustum[11] - frustum[10];
	g_flFrustum[BACK][3] = frustum[15] - frustum[14];
	NormalizeFrustumPlane(g_flFrustum, BACK);

	g_flFrustum[FRONT][0] = frustum[2] + frustum[3];
	g_flFrustum[FRONT][1] = frustum[6] + frustum[7];
	g_flFrustum[FRONT][2] = frustum[10] + frustum[11];
	g_flFrustum[FRONT][3] = frustum[14] + frustum[15];
	NormalizeFrustumPlane(g_flFrustum, FRONT);
}

bool CFrustum::PointInsideFrustum(float x, float y, float z)
{
	for (int i = 0; i < 6; ++i)
	{
		if (((g_flFrustum[i][0] * x) + (g_flFrustum[i][1] * y) + (g_flFrustum[i][2] * z) + g_flFrustum[i][3]) <= 0)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::SphereInsideFrustum(float x, float y, float z, float radius)
{
	for (int i = 0; i < 6; ++i)
	{
		if (((g_flFrustum[i][0] * x) + (g_flFrustum[i][1] * y) + (g_flFrustum[i][2] * z) + g_flFrustum[i][3]) <= -radius)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::PlaneInsideFrustum(float x, float y, float z, float size)
{
	const float sizes[3][2] =
		{
			{x - size, x + size},
			{y - size, y + size},
			{z - size, z + size}};

	for (int i = 0; i < 6; ++i)
	{
		//Test every permutation of plane sizes.
		for (int zAxis = 0; zAxis < 2; ++zAxis)
		{
			for (int yAxis = 0; yAxis < 2; ++yAxis)
			{
				for (int xAxis = 0; xAxis < 2; ++xAxis)
				{
					if (((sizes[0][xAxis] * g_flFrustum[i][0]) + (sizes[1][yAxis] * g_flFrustum[i][1]) + (sizes[2][zAxis] * g_flFrustum[i][2]) + g_flFrustum[i][3]) <= 0)
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}
