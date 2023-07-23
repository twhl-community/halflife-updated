/***+
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

#include "windows.h"
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "pm_shared.h"

#include <string.h>
#include <stdio.h>

#include "pmtrace.h"
#include "r_efx.h"
#include "event_api.h"
#include "event_args.h"
#include "in_defs.h"
#include "pm_defs.h"
#include "fog.h"

#include "gl/gl.h"

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"

// Class declaration
CFog gFog;

int __MsgFunc_Fog( const char *pszName, int iSize, void *pBuf )
{
	return gFog.MsgFunc_Fog( pszName, iSize, pBuf );
}

/*
====================
Init

====================
*/
void CFog::Init( void )
{
	HOOK_MESSAGE( Fog );
}

/*
====================
VidInit

====================
*/
void CFog::VidInit( void )
{
	memset(&m_fogParams, 0, sizeof(fog_params_t));
	memset(&m_fogBlend1, 0, sizeof(fog_params_t));
	memset(&m_fogBlend2, 0, sizeof(fog_params_t));
}

/*
====================
SetGLFog

====================
*/
void CFog::SetGLFog(Vector& color)
{
	if(!m_fogParams.enddist && !m_fogParams.startdist)
	{
		gEngfuncs.pTriAPI->Fog( Vector(0, 0, 0), 0, 0, FALSE );
		glDisable(GL_FOG);
		return;
	}

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.5);

	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_START, m_fogParams.startdist);
	glFogf(GL_FOG_END, m_fogParams.enddist);

	// Tell the engine too
	gEngfuncs.pTriAPI->Fog ( color, m_fogParams.startdist, m_fogParams.enddist, TRUE );	
}
/*
====================
HUD_CreateEntities

====================
*/
void CFog::HUD_CreateEntities( void )
{
	// Do any blending
	BlendFog();
}

/*
====================
V_CalcRefDef

====================
*/
void CFog::V_CalcRefDef( const ref_params_t* pparams )
{
	// Calculate distance to edge
	Vector boxTotal = Vector(m_fogParams.enddist, m_fogParams.enddist, m_fogParams.enddist);
	float edgeLength = boxTotal.Length();

	// Set mins/maxs box
	for(int i = 0; i < 3; i++)
	{
		m_vFogBBoxMin[i] = pparams->vieworg[i] - edgeLength;
		m_vFogBBoxMax[i] = pparams->vieworg[i] + edgeLength;
	}

	// Remember this
	m_clientWaterLevel = pparams->waterlevel;

	// Set the normal fog color
	SetGLFog(m_fogParams.color);
}

/*
====================
HUD_DrawNormalTriangles

====================
*/
void CFog::HUD_DrawNormalTriangles( void )
{
	SetGLFog(m_fogParams.color);
}

/*
====================
HUD_DrawTransparentTriangles

====================
*/
void CFog::HUD_DrawTransparentTriangles( void )
{
	// At this stage we want to set black fog, so beams are faded out
	SetGLFog(Vector(0, 0, 0));
}

/*
====================
BlendFog

====================
*/
void CFog::BlendFog( void )
{
	if(!m_fogChangeTime && !m_fogBlendTime)
		return;

	// Clear if blend is over
	if((m_fogChangeTime+m_fogBlendTime) < gEngfuncs.GetClientTime())
	{
		memcpy(&m_fogParams, &m_fogBlend2, sizeof(fog_params_t));
		memset(&m_fogBlend1, 0, sizeof(fog_params_t));
		memset(&m_fogBlend2, 0, sizeof(fog_params_t));

		m_fogChangeTime = 0;
		m_fogBlendTime = 0;
		return;
	}

	// Perform linear blending
	float interp = (gEngfuncs.GetClientTime() - m_fogChangeTime)/m_fogBlendTime;
	VectorScale(m_fogBlend1.color, (1.0 - interp), m_fogParams.color);
	VectorMA(m_fogParams.color, interp, m_fogBlend2.color, m_fogParams.color);
	m_fogParams.enddist = (m_fogBlend1.enddist * (1.0 - interp)) + (m_fogBlend2.enddist * interp);
	m_fogParams.startdist = (m_fogBlend1.startdist * (1.0 - interp)) + (m_fogBlend2.startdist * interp);
}

/*
====================
CullFogBBox

====================
*/
bool CFog::CullFogBBox(const Vector& mins, const Vector& maxs)
{
	if(!m_fogParams.enddist && !m_fogParams.startdist || m_clientWaterLevel == 3)
		return false;

	if (mins[0] > m_vFogBBoxMax[0]) 
		return true;

	if (mins[1] > m_vFogBBoxMax[1]) 
		return true;

	if (mins[2] > m_vFogBBoxMax[2]) 
		return true;

	if (maxs[0] < m_vFogBBoxMin[0]) 
		return true;

	if (maxs[1] < m_vFogBBoxMin[1]) 
		return true;

	if (maxs[2] < m_vFogBBoxMin[2]) 
		return true;

	return false;
}

/*
====================
MsgFunc_Fog

====================
*/
int CFog::MsgFunc_Fog( const char *pszName, int iSize, void *pBuf )
{
	BEGIN_READ(pBuf, iSize);

	int startdist = READ_COORD();
	int enddist = READ_COORD();

	Vector fogcolor;
	for(int i = 0; i < 3; i++)
		fogcolor[i] = (float)READ_BYTE()/255.0f;

	float blendtime = READ_COORD();

	// If blending, copy current fog params to the blend state if we had any active
	if(blendtime > 0 && m_fogParams.enddist > 0 && m_fogParams.startdist > 0)
	{
		memcpy(&m_fogBlend1, &m_fogParams, sizeof(fog_params_t));

		VectorCopy(fogcolor, m_fogBlend2.color);
		m_fogBlend2.startdist = startdist;
		m_fogBlend2.enddist = enddist;

		// Set times
		m_fogChangeTime = gEngfuncs.GetClientTime();
		m_fogBlendTime = blendtime;
	}
	else
	{
		// No blending, just set
		memset(&m_fogBlend1, 0, sizeof(fog_params_t));
		memset(&m_fogBlend2, 0, sizeof(fog_params_t));

		VectorCopy(fogcolor, m_fogParams.color);
		m_fogParams.startdist = startdist;
		m_fogParams.enddist = enddist;

		m_fogChangeTime = 0;
		m_fogBlendTime = 0;
	}

	return 1;
}