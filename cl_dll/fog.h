//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined ( FOG_H )
#define FOG_H
#if defined( _WIN32 )
#pragma once
#endif

#include "ref_params.h"

/*
====================
CFog

====================
*/
class CFog
{
public:
	struct fog_params_t
	{
		int enddist;
		int startdist;
		Vector color;
	};

public:
	void Init( void );
	void VidInit( void );

	int MsgFunc_Fog( const char *pszName, int iSize, void *pBuf );

	void SetGLFog(Vector& color);
	void BlendFog( void );

	bool CullFogBBox(const Vector& mins, const Vector& maxs);

	void HUD_CreateEntities( void );
	void V_CalcRefDef( const ref_params_t* pparams );
	void HUD_DrawNormalTriangles( void );
	void HUD_DrawTransparentTriangles( void );

private:
	fog_params_t m_fogParams;
	fog_params_t m_fogBlend1;
	fog_params_t m_fogBlend2;

	float m_fogChangeTime;
	float m_fogBlendTime;

	Vector m_vFogBBoxMin;
	Vector m_vFogBBoxMax;

	int m_clientWaterLevel;
};

extern CFog gFog;
#endif // FOG_H