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

#include <algorithm>

#include "hud.h"
#include "cl_util.h"
#include "particleman.h"
#include "particleman_internal.h"
#include "pman_particlemem.h"

static MemList m_FreeMem;
static MemList m_ActiveMem;

//TODO: convert these to inline
VectorOfMemoryBlocks CMiniMem::m_vecMemoryPool;

long CMiniMem::m_lMemoryBlockSize = 0;
long CMiniMem::m_lMaxBlocks = 0;
long CMiniMem::m_lMemoryPoolSize = 0;
CMiniMem* CMiniMem::_instance = nullptr;

CMiniMem::CMiniMem(long lMemoryPoolSize, long lMaxBlockSize)
{
	m_lMemoryPoolSize = lMemoryPoolSize;
	m_lMemoryBlockSize = lMaxBlockSize;
	m_lMaxBlocks = lMemoryPoolSize / 16;

	//Allocate all blocks.
	for (long i = 0; i < m_lMaxBlocks; ++i)
	{
		auto block = new MemoryBlock(m_lMemoryBlockSize);

		m_vecMemoryPool.push_back(block);

		m_FreeMem.Push(block);
	}

	m_pVisibleParticles = new visibleparticles_t[m_lMaxBlocks];
}

CMiniMem::~CMiniMem()
{
	for (MemoryBlock* block; (block = m_FreeMem.Pop()) != nullptr;)
	{
		delete block;
	}

	m_vecMemoryPool.clear();

	delete[] m_pVisibleParticles;
	m_pVisibleParticles = nullptr;
}

void CMiniMem::Shutdown()
{
	delete _instance;
	_instance = nullptr;
}

char* CMiniMem::AllocateFreeBlock()
{
	if (auto block = m_FreeMem.Pop(); nullptr != block)
	{
		m_ActiveMem.Push(block);

		return block->Memory();
	}

	return nullptr;
}

bool CMiniMem::CheckSize(int iSize)
{
	//Ensure instance exists
	//TODO: this is a non-static function so this is pointless
	Instance();

	return iSize > 0 && iSize <= m_lMemoryBlockSize;
}

char* CMiniMem::newBlock()
{
	if (auto memory = AllocateFreeBlock(); memory)
	{
		return memory;
	}

	gEngfuncs.Con_DPrintf("Particleman is out of memory, too many particles");

	return nullptr;
}

void CMiniMem::deleteBlock(MemoryBlock* p)
{
	m_ActiveMem.Delete(p);
	m_FreeMem.Push(p);
}

long CMiniMem::PercentUsed()
{
	long usedBlockCount = 0;

	if (nullptr != m_FreeMem.Front())
	{
		for (auto block = m_FreeMem.Front(); nullptr != block; block = block->next)
		{
			++usedBlockCount;
		}

		usedBlockCount *= 100;
	}

	return usedBlockCount / m_lMaxBlocks;
}

long CMiniMem::MaxBlockSize()
{
	return m_lMemoryBlockSize;
}

CMiniMem* CMiniMem::Instance()
{
	if (!_instance)
	{
		_instance = new CMiniMem(MemoryPoolSize, g_lMaxParticleClassSize);
	}

	return _instance;
}

int ParticleSortFn(const void* elem1, const void* elem2)
{
	auto lhs = reinterpret_cast<const visibleparticles_t*>(elem1);
	auto rhs = reinterpret_cast<const visibleparticles_t*>(elem2);

	const float lhsDistance = lhs->pVisibleParticle->GetPlayerDistance();
	const float rhsDistance = rhs->pVisibleParticle->GetPlayerDistance();

	if (lhsDistance < rhsDistance)
	{
		return 1;
	}
	else if (lhsDistance > rhsDistance)
	{
		return -1;
	}

	return 0;
}

void CMiniMem::ProcessAll()
{
	const float time = gEngfuncs.GetClientTime();

	//Clear list of visible particles.
	m_iTotalParticles = 0;
	m_iParticlesDrawn = 0;

	std::memset(m_pVisibleParticles, 0, sizeof(m_pVisibleParticles[0]) * m_lMaxBlocks);

	for (auto particle = m_ActiveMem.Front(); nullptr != particle;)
	{
		auto effect = reinterpret_cast<CCoreTriangleEffect*>(particle->Memory());

		if (!effect)
		{
			//TODO: bad error handling?
			while (true)
			{
			}
		}

		++m_iTotalParticles;

		if (effect->CheckVisibility())
		{
			auto player = gEngfuncs.GetLocalPlayer();
			effect->SetPlayerDistance((player->origin - effect->m_vOrigin).LengthSquared());

			m_pVisibleParticles[m_iParticlesDrawn++].pVisibleParticle = effect;
		}

		if (!IsGamePaused())
		{
			effect->Think(time);
		}

		auto next = particle->next;

		if (0 != effect->m_flDieTime && time >= effect->m_flDieTime)
		{
			effect->Die();

			//TODO: call destructor
			//TODO: remove from visible particles list (or remove before adding it)

			deleteBlock(particle);
		}

		particle = next;
	}

	//TODO: replace with std::sort
	qsort(m_pVisibleParticles, m_iParticlesDrawn, sizeof(m_pVisibleParticles[0]), &ParticleSortFn);

	/*
	std::sort(m_pVisibleParticles, m_pVisibleParticles + m_iParticlesDrawn, [](const visibleparticles_t& lhs, const visibleparticles_t& rhs)
		{
			//Particles are ordered farthest to nearest so they can be drawn in order.
			const float lhsDistance = lhs.pVisibleParticle->GetPlayerDistance();
			const float rhsDistance = rhs.pVisibleParticle->GetPlayerDistance();

			return lhsDistance > rhsDistance;
		});
		*/


	for (int i = 0; i < m_iParticlesDrawn; ++i)
	{
		m_pVisibleParticles[i].pVisibleParticle->Draw();
	}

	g_flOldTime = time;
}

int CMiniMem::ApplyForce(Vector vOrigin, Vector vDirection, float flRadius, float flStrength)
{
	const float radiusSquared = flRadius * flRadius;
	
	for (auto particle = m_ActiveMem.Front(); particle; particle = particle->next)
	{
		auto effect = reinterpret_cast<CCoreTriangleEffect*>(particle->Memory());

		if (!effect)
		{
			//TODO: bad error handling?
			while (true)
			{
			}
		}

		if (!effect->m_bAffectedByForce)
		{
			continue;
		}

		const float size = effect->m_flSize / 5;

		const Vector mins = effect->m_vOrigin - Vector{size, size, size};
		const Vector maxs = effect->m_vOrigin + Vector{size, size, size};

		//If the force origin lies outside the effect's bounding box, calculate the distance from the box.
		float totalDistanceSquared = 0;

		for (int i = 0; i < 3; ++i)
		{
			float boundingValue;

			if (vOrigin[i] < mins[i])
			{
				boundingValue = mins[i];
			}
			else if (vOrigin[i] > maxs[i])
			{
				boundingValue = maxs[i];
			}
			else
			{
				continue;
			}

			totalDistanceSquared += (vOrigin[i] - boundingValue) * (vOrigin[i] - boundingValue);
		}

		//Effect is further away from position than force radius, don't apply force.
		if (totalDistanceSquared > radiusSquared)
		{
			continue;
		}

		const float strength = std::max(0.f, flStrength - (vOrigin - effect->m_vOrigin).Length() * (flStrength / (0.5f * radiusSquared)));

		if (vDirection == g_vecZero)
		{
			const float acceleration = -(strength / effect->m_flMass);

			const Vector direction = (vOrigin - effect->m_vOrigin).Normalize();
			const Vector velocity = effect->m_vVelocity.Normalize();

			effect->m_vVelocity = acceleration * (direction + velocity);
		}
		else
		{
			const float acceleration = strength / effect->m_flMass;

			const Vector direction = vDirection.Normalize();
			const Vector velocity = effect->m_vVelocity.Normalize();

			effect->m_vVelocity = acceleration * (direction + velocity);
		}

		effect->Force();
	}

	return 1;
}

void CMiniMem::Reset()
{
	m_ActiveMem.Reset();
	m_FreeMem.Reset();

	//Initialize the free list.
	for (auto block : m_vecMemoryPool)
	{
		m_FreeMem.Push(block);
	}
}
