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

#undef clamp

#include <algorithm>

#include "hud.h"
#include "cl_util.h"
#include "particleman.h"
#include "particleman_internal.h"
#include "CMiniMem.h"

void* CMiniMem::Allocate(std::size_t sizeInBytes, std::size_t alignment)
{
	auto particle = reinterpret_cast<CBaseParticle*>(_pool.allocate(sizeInBytes, alignment));

	if (nullptr != particle)
	{
		_particles.push_back(particle);
	}

	return particle;
}

void CMiniMem::Deallocate(void* memory, std::size_t sizeInBytes, std::size_t alignment)
{
	if (!memory)
	{
		return;
	}

	_particles.erase(std::find(_particles.begin(), _particles.end(), memory));

	_pool.deallocate(memory, sizeInBytes, alignment);
}

void CMiniMem::Shutdown()
{
	delete _instance;
	_instance = nullptr;
}

CMiniMem* CMiniMem::Instance()
{
	if (!_instance)
	{
		_instance = new CMiniMem();
	}

	return _instance;
}

void CMiniMem::ProcessAll()
{
	const float time = gEngfuncs.GetClientTime();

	//Clear list of visible particles.
	_visibleParticles = 0;

	//Divide the particle list in two: the list of visible particles and the list of invisible particles.
	//Remove any particles that have died.
	std::size_t invisibleCount = 0;

	for (std::size_t i = 0; i < (_particles.size() - invisibleCount);)
	{
		auto effect = _particles[i];

		if (!IsGamePaused())
		{
			effect->Think(time);
		}

		if (0 != effect->m_flDieTime && time >= effect->m_flDieTime)
		{
			effect->Die();
			delete effect;

			//Don't do this! operator delete removes the effect from the list.
			//_particles.erase(_particles.begin() + i);
			continue;
		}

		if (effect->CheckVisibility())
		{
			auto player = gEngfuncs.GetLocalPlayer();
			effect->SetPlayerDistance((player->origin - effect->m_vOrigin).LengthSquared());

			++_visibleParticles;
		}
		else
		{
			if (i + invisibleCount < _particles.size())
			{
				//There is an effect we haven't checked yet.
				//Put the invisible effect at the end of the list and check the other effect next.
				std::swap(_particles[i], _particles[_particles.size() - 1 - invisibleCount]);
				++invisibleCount;
				continue;
			}
			//No more unchecked effects in the list.
		}

		++i;
	}

	std::sort(_particles.begin(), _particles.begin() + _visibleParticles, [](const auto& lhs, const auto& rhs)
		{
			//Particles are ordered farthest to nearest so they can be drawn in order.
			const float lhsDistance = lhs->GetPlayerDistance();
			const float rhsDistance = rhs->GetPlayerDistance();

			return lhsDistance > rhsDistance;
		});

	for (std::size_t i = 0; i < _visibleParticles; ++i)
	{
		auto effect = _particles[i];
		effect->Draw();
	}

	g_flOldTime = time;
}

int CMiniMem::ApplyForce(Vector vOrigin, Vector vDirection, float flRadius, float flStrength)
{
	const float radiusSquared = flRadius * flRadius;

	for (auto effect : _particles)
	{
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
	_visibleParticles = 0;

	for (auto particle : _particles)
	{
		particle->Die();
		delete particle;
	}

	//Cleared by CBaseParticle::operator delete
	//_particles.clear();

	//Wipe away previously allocated memory so maps with loads of particles don't eat up memory forever.
	_pool.release();
	_particles.shrink_to_fit();
}
