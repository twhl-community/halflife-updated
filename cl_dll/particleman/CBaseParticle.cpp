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

#undef clamp

#include <algorithm>

#include "event_api.h"
#include "triangleapi.h"

#include "particleman.h"
#include "particleman_internal.h"
#include "CBaseParticle.h"

#include "pm_defs.h"
#include "pmtrace.h"

void CBaseParticle::InitializeSprite(Vector org, Vector normal, model_s* sprite, float size, float brightness)
{
	m_flSize = m_flOriginalSize = 10;

	m_flScaleSpeed = 0;
	m_flContractSpeed = 0;
	m_flStretchX = m_flStretchY = 1;

	m_pTexture = nullptr;
	m_flOriginalBrightness = 255;
	m_flBrightness = 255;

	strncpy(m_szClassname, "particle", sizeof(m_szClassname) - 1);
	m_szClassname[sizeof(m_szClassname) - 1] = '\0';

	m_flDieTime = 0;
	m_flDampingTime = 0;

	m_flNextPVSCheck = gEngfuncs.GetClientTime();

	m_flGravity = 0;
	m_flNextCollisionTime = 0;

	m_bInPVS = true;

	m_flFadeSpeed = -1;
	m_iFramerate = 0;
	m_iNumFrames = 0;
	m_iFrame = 0;
	m_iCollisionFlags = 0;

	m_iRendermode = kRenderNormal;
	m_iRenderFlags = 0;

	m_vVelocity.x = 0;
	m_vVelocity.y = 0;
	m_vVelocity.z = 0;

	m_flBounceFactor = 1;

	m_vOriginalAngles = g_vecZero;
	m_vAngles = g_vecZero;
	m_vAVelocity = g_vecZero;

	m_bInWater = false;
	m_bAffectedByForce = false;

	m_vColor.x = 255;
	m_vColor.y = 255;
	m_vColor.z = 255;

	m_flTimeCreated = gEngfuncs.GetClientTime();

	m_vPrevOrigin = m_vOrigin = org;
	m_vOriginalAngles = m_vAngles = normal;

	m_pTexture = sprite;
	m_flOriginalSize = m_flSize = size;
	m_flOriginalBrightness = m_flBrightness = brightness;

	Vector forward, right, up;
	gEngfuncs.pfnAngleVectors(normal, forward, right, up);

	const Vector scaledRight = right * size;
	const Vector scaledUp = up * size;

	m_vLowLeft = org - scaledRight * 0.5 - scaledUp * 0.5;

	//TODO: not sure if these are correct. If low left is half of the scaled directions then the full direction * 2 results in double size particles.
	m_vLowRight = m_vLowLeft + scaledRight + scaledRight;
	m_vTopLeft = m_vLowLeft + scaledUp + scaledUp;
}

bool CBaseParticle::CheckVisibility()
{
	const float radius = m_flSize / 5.0;

	if (gEngfuncs.GetClientTime() >= m_flNextPVSCheck)
	{
		const Vector radiusVector{radius, radius, radius};
		Vector mins = m_vOrigin - radiusVector;
		Vector maxs = m_vOrigin + radiusVector;

		m_bInPVS = gEngfuncs.pTriAPI->BoxInPVS(mins, maxs) != 0;

		m_flNextPVSCheck = gEngfuncs.GetClientTime() + 0.1;
	}

	if ((m_iRenderFlags & CULL_FRUSTUM_SPHERE) != 0)
	{
		if (!g_cFrustum.SphereInsideFrustum(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z, radius))
		{
			return false;
		}
	}
	else if ((m_iRenderFlags & CULL_FRUSTUM_PLANE) != 0)
	{
		if (!g_cFrustum.PlaneInsideFrustum(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z, radius))
		{
			return false;
		}
	}
	else if ((m_iRenderFlags & CULL_FRUSTUM_POINT) != 0)
	{
		if (!g_cFrustum.PointInsideFrustum(m_vOrigin.x, m_vOrigin.y, m_vOrigin.z))
		{
			return false;
		}
	}

	if (!m_bInPVS)
	{
		return (m_iRenderFlags & CULL_PVS) == 0;
	}

	return true;
}

void CBaseParticle::Draw()
{
	if (m_flDieTime == gEngfuncs.GetClientTime())
	{
		return;
	}

	Vector vColor;
	float intensity = 0.0;

	if ((m_iRenderFlags & LIGHT_NONE) == 0)
	{
		gEngfuncs.pTriAPI->LightAtPoint(m_vOrigin, vColor);

		intensity = (vColor.x + vColor.y + vColor.z) / 3.0;
	}

	if ((m_iRenderFlags & (RENDER_FACEPLAYER | RENDER_FACEPLAYER_ROTATEZ)) != 0)
	{
		m_vAngles.x = g_vViewAngles.x;
		m_vAngles.y = g_vViewAngles.y;

		if ((m_iRenderFlags & RENDER_FACEPLAYER_ROTATEZ) == 0)
		{
			m_vAngles.z = g_vViewAngles.z;
		}
	}

	Vector resultColor;

	if ((m_iRenderFlags & LIGHT_NONE) != 0)
	{
		resultColor = m_vColor;
	}
	else if ((m_iRenderFlags & LIGHT_COLOR) != 0)
	{
		resultColor.x = vColor.x / (m_vColor.x * 255);
		resultColor.y = vColor.y / (m_vColor.y * 255);
		resultColor.z = vColor.z / (m_vColor.z * 255);
	}
	else if ((m_iRenderFlags & LIGHT_INTENSITY) != 0)
	{
		resultColor.x = intensity / (m_vColor.x * 255);
		resultColor.y = intensity / (m_vColor.y * 255);
		resultColor.z = intensity / (m_vColor.z * 255);
	}

	resultColor.x = std::clamp(resultColor.x, 0.f, 255.f);
	resultColor.y = std::clamp(resultColor.y, 0.f, 255.f);
	resultColor.z = std::clamp(resultColor.z, 0.f, 255.f);

	Vector forward, right, up;
	gEngfuncs.pfnAngleVectors(m_vAngles, forward, right, up);

	const float radius = m_flSize;
	const Vector width = right * radius * m_flStretchX;
	const Vector height = up * radius * m_flStretchY;

	//TODO: shouldn't this be accounting for stretch Y?
	const Vector lowLeft = m_vOrigin - (width * 0.5) - (up * radius * 0.5);

	const Vector lowRight = lowLeft + width;
	const Vector topLeft = lowLeft + height;
	const Vector topRight = lowRight + height;

	gEngfuncs.pTriAPI->SpriteTexture(m_pTexture, m_iFrame);
	gEngfuncs.pTriAPI->RenderMode(m_iRendermode);
	gEngfuncs.pTriAPI->CullFace(TRI_NONE);

	gEngfuncs.pTriAPI->Begin(TRI_QUADS);
	gEngfuncs.pTriAPI->Color4f(resultColor.x / 255, resultColor.y / 255, resultColor.z / 255, m_flBrightness / 255);

	gEngfuncs.pTriAPI->TexCoord2f(0, 0);
	gEngfuncs.pTriAPI->Vertex3fv(topLeft);

	gEngfuncs.pTriAPI->TexCoord2f(0, 1);
	gEngfuncs.pTriAPI->Vertex3fv(lowLeft);

	gEngfuncs.pTriAPI->TexCoord2f(1, 1);
	gEngfuncs.pTriAPI->Vertex3fv(lowRight);

	gEngfuncs.pTriAPI->TexCoord2f(1, 0);
	gEngfuncs.pTriAPI->Vertex3fv(topRight);

	gEngfuncs.pTriAPI->End();

	gEngfuncs.pTriAPI->RenderMode(kRenderNormal);
	gEngfuncs.pTriAPI->CullFace(TRI_FRONT);
}

void CBaseParticle::Animate(float time)
{
	if (0 != m_iFramerate && 0 != m_iNumFrames)
	{
		m_iFrame = static_cast<int>(m_iFramerate * (time - m_flTimeCreated)) % m_iNumFrames;
	}
}

void CBaseParticle::AnimateAndDie(float time)
{
	if (0 != m_iFramerate && 0 != m_iNumFrames)
	{
		const float frame = m_iFramerate * (time - m_flTimeCreated);

		if (frame > m_iNumFrames)
		{
			m_flDieTime = time;
		}
		else
		{
			m_iFrame = frame;
		}

		if (m_iFrame > m_iNumFrames)
		{
			m_iFrame = m_iNumFrames - 1;
		}
	}
}

void CBaseParticle::Expand(float time)
{
	if (m_flScaleSpeed != 0)
	{
		m_flSize = (m_flScaleSpeed * 30.0 * (time - m_flTimeCreated)) + m_flOriginalSize;

		if (m_flSize < 0.0001)
		{
			m_flDieTime = time;
		}
	}
}

void CBaseParticle::Contract(float time)
{
	if (m_flContractSpeed != 0)
	{
		m_flSize = m_flOriginalSize - (m_flContractSpeed * 30.0 * (time - m_flTimeCreated));

		if (m_flSize < 0.0001)
		{
			m_flDieTime = time;
		}
	}
}

void CBaseParticle::Fade(float time)
{
	if (m_flFadeSpeed >= -0.5)
	{
		if (m_flFadeSpeed == 0)
		{
			m_flBrightness = (1.0 - (time - m_flTimeCreated) / (m_flDieTime - m_flTimeCreated)) * m_flOriginalBrightness;
		}
		else
		{
			m_flBrightness = m_flOriginalBrightness - m_flFadeSpeed * 30.0 * (time - m_flTimeCreated);
		}

		if (m_flBrightness < 1)
		{
			m_flDieTime = time;
		}
	}
}

void CBaseParticle::Spin(float time)
{
	if (m_vAVelocity == g_vecZero)
	{
		return;
	}

	//Superfluous; overwritten below.
	//m_vAngles = m_vAngles + m_vAVelocity;

	Vector point = m_vAVelocity;

	const float length = VectorNormalize(point) * 30.0 * (time - m_flTimeCreated);

	m_vAngles = m_vOriginalAngles + m_vAVelocity * length;
}

void CBaseParticle::CalculateVelocity(float time)
{
	const float deltaTime = time - g_flOldTime;
	const float gravity = -deltaTime * g_flGravity * m_flGravity;

	if (m_vVelocity == g_vecZero && gravity == 0)
	{
		return;
	}

	if ((m_iCollisionFlags & TRI_SPIRAL) != 0)
	{
		m_vOrigin = m_vOrigin + m_vVelocity * deltaTime;

		m_vOrigin.x += sin(time * 5.0 + reinterpret_cast<std::intptr_t>(this)) * 2;
		m_vOrigin.y += sin(time * 7.5 + reinterpret_cast<std::intptr_t>(this));
	}
	else
	{
		m_vOrigin = m_vOrigin + m_vVelocity * deltaTime;
	}

	m_vVelocity.z = m_vVelocity.z + gravity;
}

void CBaseParticle::CheckCollision(float time)
{
	if (m_iCollisionFlags == 0)
	{
		return;
	}

	m_flNextCollisionTime = time;

	if ((m_iCollisionFlags & (TRI_WATERTRACE | TRI_COLLIDEALL | TRI_COLLIDEWORLD)) == 0)
	{
		return;
	}

	pmtrace_t trace;

	bool collided = false;

	if ((m_iCollisionFlags & TRI_COLLIDEALL) != 0)
	{
		gEngfuncs.pEventAPI->EV_SetTraceHull(2);
		gEngfuncs.pEventAPI->EV_PlayerTrace(m_vPrevOrigin, m_vOrigin, PM_STUDIO_BOX, -1, &trace);

		if (trace.fraction != 1.0)
		{
			//Called but never used, probably unfinished code for colliding with other entities.
			//auto entity = gEngfuncs.pEventAPI->EV_GetPhysent(trace.ent);

			//Collided with something other than world, ignore.
			if (0 == trace.ent)
			{
				collided = true;
			}
		}
	}
	else if ((m_iCollisionFlags & TRI_COLLIDEWORLD) != 0)
	{
		gEngfuncs.pEventAPI->EV_SetTraceHull(2);
		gEngfuncs.pEventAPI->EV_PlayerTrace(m_vPrevOrigin, m_vOrigin, PM_WORLD_ONLY | PM_STUDIO_BOX, -1, &trace);

		if (trace.fraction != 1.0)
		{
			m_vVelocity = m_vVelocity * 0.6;

			if (m_vVelocity.Length() < 10)
			{
				m_iCollisionFlags = 0;
				m_vVelocity = g_vecZero;
				m_vAVelocity = g_vecZero;
				m_vOrigin = trace.endpos;
			}

			collided = true;
		}
	}

	if (collided)
	{
		const float frametime = time - g_flOldTime;

		m_vOrigin = m_vPrevOrigin + m_vVelocity * (trace.fraction * frametime);

		float bounce;

		bool dead = false;

		if (trace.plane.normal.z <= 0.9 || m_vVelocity.z > 0 || (m_vVelocity.z < -frametime * g_flGravity * m_flGravity * 3.0))
		{
			if ((m_iCollisionFlags & TRI_COLLIDEKILL) != 0)
			{
				m_flDieTime = gEngfuncs.GetClientTime();
				dead = true;
			}
			else
			{
				bounce = m_flBounceFactor * 0.5;

				if (bounce != 0)
				{
					const float dot = DotProduct(trace.plane.normal, m_vVelocity) * -2;
					m_vVelocity = m_vVelocity + trace.plane.normal * dot;
					m_vAngles.y = -m_vAngles.y;
				}
			}
		}
		else
		{
			//Particle fell on an (almost) flat surface and has no velocity to bounce back up; disable collisions from now on.
			bounce = 0;
			m_vVelocity = g_vecZero;
			m_vAVelocity = g_vecZero;
			m_iCollisionFlags = 0;
		}

		if (!dead && bounce != 1)
		{
			m_vVelocity = m_vVelocity * bounce;
			m_vAngles = m_vAngles * 0.9;
		}

		Touch(trace.endpos, trace.plane.normal, trace.ent);
	}
	else if ((m_iCollisionFlags & TRI_WATERTRACE) != 0)
	{
		if (gEngfuncs.PM_PointContents(m_vOrigin, nullptr) == CONTENTS_WATER && !m_bInWater)
		{
			Touch(m_vOrigin, {0, 0, 1}, 0);

			m_bInWater = true;

			if ((m_iCollisionFlags & TRI_COLLIDEKILL) != 0)
			{
				m_flDieTime = gEngfuncs.GetClientTime();
			}
		}
	}

	m_vPrevOrigin = m_vOrigin;
}

void CBaseParticle::Touch(Vector pos, Vector normal, int index)
{
	//Nothing.
}

void CBaseParticle::Die()
{
	//Nothing.
}

void CBaseParticle::Force()
{
	//Nothing.
}

void CBaseParticle::Think(float time)
{
	if ((m_iCollisionFlags & TRI_ANIMATEDIE) != 0)
	{
		AnimateAndDie(time);
	}
	else
	{
		Animate(time);
	}

	Contract(time);
	Expand(time);
	Fade(time);
	Spin(time);
	CalculateVelocity(time);
	CheckCollision(time);
}
