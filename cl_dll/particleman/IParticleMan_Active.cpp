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

#include <vector>

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "particleman.h"
#include "particleman_internal.h"
#include "CMiniMem.h"
#include "IParticleMan_Active.h"

static bool g_iRenderMode = true;

static cvar_t* cl_pmanstats = nullptr;

static std::vector<ForceMember> g_pForceList;

EXPOSE_INTERFACE(IParticleMan_Active, IParticleMan, PARTICLEMAN_INTERFACE);

IParticleMan_Active::IParticleMan_Active()
{
	g_pForceList.reserve(MaxForceElements);
}

void IParticleMan_Active::SetRender(int iRender)
{
	g_iRenderMode = iRender != 0;
}

void IParticleMan_Active::ApplyForce(Vector vOrigin, Vector vDirection, float flRadius, float flStrength, float flDuration)
{
	if (g_pForceList.size() >= MaxForceElements)
	{
		return;
	}

	ForceMember member;

	member.m_vOrigin = vOrigin;
	member.m_vDirection = vDirection;
	member.m_flRadius = flRadius;
	member.m_flStrength = flStrength;
	member.m_flDieTime = gEngfuncs.GetClientTime() + flDuration;

	g_pForceList.push_back(member);
}

void IParticleMan_Active::SetUp(cl_enginefunc_t* pEnginefuncs)
{
	//Note: disabled because we're in the client dll.
	//std::memcpy(&gEngfuncs, pEnginefuncs, sizeof(gEngfuncs));

	cl_pmanstats = gEngfuncs.pfnRegisterVariable("cl_pmanstats", "0", 0);
}

CBaseParticle* IParticleMan_Active::CreateParticle(Vector org, Vector normal, model_s* sprite, float size, float brightness, const char* classname)
{
	auto particle = new CBaseParticle();

	particle->InitializeSprite(org, normal, sprite, size, brightness);
	strncpy(particle->m_szClassname, classname, sizeof(particle->m_szClassname) - 1);
	particle->m_szClassname[sizeof(particle->m_szClassname) - 1] = '\0';

	return particle;
}

void IParticleMan_Active::ResetParticles()
{
	CMiniMem::Instance()->Reset();
	g_pForceList.clear();
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

	for (std::size_t i = 0; i < g_pForceList.size();)
	{
		if (auto& member = g_pForceList[i]; member.m_flDieTime != 0 && member.m_flDieTime < time)
		{
			//Always swap to last before erasing to make it cheaper to do.
			if (i + 1 < g_pForceList.size())
			{
				std::swap(member, g_pForceList[g_pForceList.size() - 1]);
			}

			g_pForceList.erase(g_pForceList.begin() + (g_pForceList.size() - 1));
		}
		else
		{
			++i;
		}
	}

	auto memory = CMiniMem::Instance();

	for (const auto& member : g_pForceList)
	{
		memory->ApplyForce(member.m_vOrigin, member.m_vDirection, member.m_flRadius, member.m_flStrength);
	}

	g_cFrustum.CalculateFrustum();

	memory->ProcessAll();

	if (nullptr != cl_pmanstats && cl_pmanstats->value == 1)
	{
		//TODO: engine doesn't support printing size_t, use local printf
		gEngfuncs.Con_NPrintf(15, "Number of Particles: %d", static_cast<int>(CMiniMem::Instance()->GetTotalParticles()));
		gEngfuncs.Con_NPrintf(16, "Particles Drawn: %d", static_cast<int>(CMiniMem::Instance()->GetDrawnParticles()));
	}
}
