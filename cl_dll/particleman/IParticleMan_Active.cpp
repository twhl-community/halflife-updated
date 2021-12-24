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

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "particleman.h"
#include "particleman_internal.h"
#include "pman_particlemem.h"
#include "IParticleMan_Active.h"

static bool g_iRenderMode = true;

static cvar_t* cl_pmanstats = nullptr;

static ForceList g_pForceList;

EXPOSE_INTERFACE(IParticleMan_Active, IParticleMan, PARTICLEMAN_INTERFACE);

void IParticleMan_Active::SetRender(int iRender)
{
	g_iRenderMode = iRender != 0;
}

void IParticleMan_Active::ApplyForce(Vector vOrigin, Vector vDirection, float flRadius, float flStrength, float flDuration)
{
	if (g_pForceList.m_iElements >= MaxForceElements)
	{
		return;
	}

	++g_pForceList.m_iElements;

	auto member = new ForceMember();

	member->m_pNext = nullptr;
	member->m_pPrevious = nullptr;
	member->m_flRadius = 0;
	member->m_flStrength = 0;
	member->m_flDieTime = 0;
	member->m_iIndex = g_pForceList.m_iElements;

	if (nullptr != g_pForceList.pFirst)
	{
		member->m_pPrevious = g_pForceList.pCurrent;

		g_pForceList.pCurrent->m_pNext = member;

		g_pForceList.pLast = g_pForceList.pCurrent = member;
	}
	else
	{
		g_pForceList.pLast = g_pForceList.pCurrent = g_pForceList.pFirst = member;
	}

	if (auto current = g_pForceList.pCurrent; nullptr != current)
	{
		current->m_vOrigin = vOrigin;
		current->m_vDirection = vDirection;
		current->m_flRadius = flRadius;
		current->m_flStrength = flStrength;
		current->m_flDieTime = gEngfuncs.GetClientTime() + flDuration;
	}
}

char* IParticleMan_Active::RequestNewMemBlock(int iSize)
{
	if (!g_iRenderMode)
	{
		return nullptr;
	}

	return CMiniMem::Instance()->newBlock();
}

void IParticleMan_Active::AddCustomParticleClassSize(unsigned long lSize)
{
	MaxParticleClassSize(lSize);
}

void IParticleMan_Active::SetUp(cl_enginefunc_t* pEnginefuncs)
{
	//Note: disabled because we're in the client dll.
	//std::memcpy(&gEngfuncs, pEnginefuncs, sizeof(gEngfuncs));

	cl_pmanstats = gEngfuncs.pfnRegisterVariable("cl_pmanstats", "0", 0);
	g_lMaxParticleClassSize = 0;
	MaxParticleClassSize(0);
}

CBaseParticle* IParticleMan_Active::CreateParticle(Vector org, Vector normal, model_s* sprite, float size, float brightness, const char* classname)
{
	auto particle = new CBaseParticle();

	particle->InitializeSprite(org, normal, sprite, size, brightness);
	//TODO: unsafe
	strcpy(particle->m_szClassname, classname);

	return particle;
}

void IParticleMan_Active::ResetParticles()
{
	CMiniMem::Instance()->Reset();
	g_pForceList.Clear();
}

void IParticleMan_Active::SetVariables(float flGravity, Vector vViewAngles)
{
	g_flGravity = flGravity;

	if (gEngfuncs.GetClientTime() != g_flOldTime)
	{
		g_vViewAngles = vViewAngles;
	}
}

void IParticleMan_Active::Update()
{
	g_pParticleMan = this;

	const float time = gEngfuncs.GetClientTime();

	for (auto member = g_pForceList.pFirst; nullptr != member;)
	{
		auto next = member->m_pNext;

		if (member->m_flDieTime != 0 && member->m_flDieTime < time)
		{
			if (member == g_pForceList.pFirst)
			{
				g_pForceList.pFirst = member->m_pNext;
			}

			if (member == g_pForceList.pLast)
			{
				g_pForceList.pLast = g_pForceList.pCurrent = member->m_pPrevious;
			}

			delete member;

			--g_pForceList.m_iElements;
		}

		member = next;
	}

	for (auto member = g_pForceList.pFirst; nullptr != member; member = member->m_pNext)
	{
		CMiniMem::ApplyForce(member->m_vOrigin, member->m_vDirection, member->m_flRadius, member->m_flStrength);
	}

	g_cFrustum.CalculateFrustum();

	CMiniMem::Instance()->ProcessAll();

	if (nullptr != cl_pmanstats && cl_pmanstats->value == 1)
	{
		gEngfuncs.Con_NPrintf(15, "Number of Particles: %d", CMiniMem::Instance()->GetTotalParticles());
		gEngfuncs.Con_NPrintf(16, "Particles Drawn: %d", CMiniMem::Instance()->GetDrawnParticles());
		gEngfuncs.Con_NPrintf(17, "CMiniMem Free: %d%%", CMiniMem::Instance()->PercentUsed());
	}
}
