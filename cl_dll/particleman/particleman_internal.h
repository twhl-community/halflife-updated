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

/**
*	@file
*
*	Internal header for particle manager
*/

//TODO: remove this once the clamp macro has been removed
#undef clamp

#include <algorithm>

#include "CFrustum.h"

constexpr long MemoryPoolSize = 30000;
constexpr unsigned long MinimumParticleClassSize = 276;
constexpr int MaxForceElements = 128;

inline unsigned long g_lMaxParticleClassSize;

inline CFrustum g_cFrustum;
inline float g_flGravity;
inline float g_flOldTime;
inline Vector g_vViewAngles;

/**
*	@brief Updates the maximum particle class size as needed.
*	@param lSize Size of a particle class type, in bytes.
*/
constexpr long MaxParticleClassSize(unsigned long lSize)
{
	if (lSize > 0)
	{
		g_lMaxParticleClassSize = std::max({MinimumParticleClassSize, g_lMaxParticleClassSize, lSize});
	}

	return g_lMaxParticleClassSize;
}

inline bool IsGamePaused()
{
	return gEngfuncs.GetClientTime() == g_flOldTime;
}

struct ForceMember
{
	int m_iIndex;
	ForceMember* m_pNext;
	ForceMember* m_pPrevious;
	Vector m_vOrigin;
	Vector m_vDirection;
	float m_flRadius;
	float m_flStrength;
	float m_flDieTime;
};

/**
*	@brief List of physics forces to apply every frame.
*/
struct ForceList
{
	~ForceList()
	{
		Clear();
	}

	void Clear()
	{
		while (nullptr != pFirst)
		{
			auto next = pFirst->m_pNext;
			delete pFirst;
			pFirst = next;
		}

		pCurrent = nullptr;
		pLast = nullptr;
		m_iElements = 0;
	}

	int m_iElements = 0;
	ForceMember* pFirst = nullptr;
	ForceMember* pCurrent = nullptr;
	ForceMember* pLast = nullptr;
};
