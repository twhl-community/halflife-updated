//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "Exports.h"

#include "particleman.h"
#include "tri.h"
#include "eventscripts.h"

struct Quad
{
	float left;
	float right;
	float top;
	float bottom;
};

extern IParticleMan* g_pParticleMan;

extern ref_params_t gRefParams;

extern cvar_t* crosshair;

static float flScaleX;
static float flScaleY;
static float flOffsetX;
static float flOffsetY;

static HSPRITE sPic;
static color24 sColor;

static HSPRITE sCrosshair;
static Rect sCrosshairRc;
static color24 sCrosshairColor;

// Shrink sprite quads a tad to prevent bleeding
static constexpr float kTexCoordOffset = 1.0F / 16.0F;


/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles()
{
	//	RecClDrawNormalTriangles();

	gHUD.m_Spectator.DrawOverview();
}


/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void DLLEXPORT HUD_DrawTransparentTriangles()
{
	//	RecClDrawTransparentTriangles();


	if (g_pParticleMan)
		g_pParticleMan->Update();
}


void SPR_Set(HSPRITE hPic, int r, int g, int b)
{
	// defer to engine for software rendering
	if (!gHUD.ShouldScale())
	{
		gEngfuncs.pfnSPR_Set(hPic, r, g, b);
	}

	sPic = hPic;

	sColor.r = r;
	sColor.g = g;
	sColor.b = b;
}


static bool SPR_Setup(const HSPRITE pic, const int frame, const int rendermode, const color24 color)
{
	if (pic == 0)
	{
		return false;
	}

	const model_s* const pSpriteModel = gEngfuncs.GetSpritePointer(pic);

	if (pSpriteModel == nullptr)
	{
		return false;
	}

	if (gEngfuncs.pTriAPI->SpriteTexture((model_s*)pSpriteModel, frame) == 0)
	{
		return false;
	}

	gEngfuncs.pTriAPI->CullFace(TRI_FRONT);

	gEngfuncs.pTriAPI->RenderMode(rendermode);
	gEngfuncs.pTriAPI->Color4ub(color.r, color.g, color.b, 255);

	return true;
}


static inline void SPR_DrawQuad(const Quad& verts, const Quad& uv)
{
	gEngfuncs.pTriAPI->Begin(TRI_QUADS);

	gEngfuncs.pTriAPI->TexCoord2f(uv.left, uv.top);
	gEngfuncs.pTriAPI->Vertex3f(verts.left, verts.top, 0.0F);

	gEngfuncs.pTriAPI->TexCoord2f(uv.right, uv.top);
	gEngfuncs.pTriAPI->Vertex3f(verts.right, verts.top, 0.0F);

	gEngfuncs.pTriAPI->TexCoord2f(uv.right, uv.bottom);
	gEngfuncs.pTriAPI->Vertex3f(verts.right, verts.bottom, 0.0F);

	gEngfuncs.pTriAPI->TexCoord2f(uv.left, uv.bottom);
	gEngfuncs.pTriAPI->Vertex3f(verts.left, verts.bottom, 0.0F);

	gEngfuncs.pTriAPI->End();
}


static inline void SPR_Restore()
{
	gEngfuncs.pTriAPI->RenderMode(kRenderNormal);
	gEngfuncs.pTriAPI->Color4ub(255, 255, 255, 255);
}


static void SPR_DrawInternal(const int frame, const int x, const int y, const Rect* prc, const int rendermode)
{
	if (!SPR_Setup(sPic, frame, rendermode, sColor))
	{
		return;
	}

	const int sprWidth = SPR_Width(sPic, frame);
	const int sprHeight = SPR_Height(sPic, frame);

	const Rect nullrc = {
		0,
		sprWidth,
		0,
		sprHeight,
	};

	if (prc == nullptr)
	{
		prc = &nullrc;
	}

	// vertices

	const int width = prc->right - prc->left;
	const int height = prc->bottom - prc->top;

	const Quad verts = {
		flOffsetX + flScaleX * x,
		flOffsetX + flScaleX * (x + width),
		flOffsetY + flScaleY * y,
		flOffsetY + flScaleY * (y + height),
	};

	// texcoords

	const Quad uv = {
		(prc->left   + kTexCoordOffset) / (float)sprWidth,
		(prc->right  - kTexCoordOffset) / (float)sprWidth,
		(prc->top    + kTexCoordOffset) / (float)sprHeight,
		(prc->bottom - kTexCoordOffset) / (float)sprHeight,
	};
	
	SPR_DrawQuad(verts, uv);

	SPR_Restore();
}


void SPR_Draw(int frame, int x, int y, const Rect* prc)
{
	// defer to engine for software rendering
	if (!gHUD.ShouldScale())
	{
		gEngfuncs.pfnSPR_Draw(frame, x, y, prc);
		return;
	}

	SPR_DrawInternal(frame, x, y, prc, kRenderNormal);
}


void SPR_DrawHoles(int frame, int x, int y, const Rect* prc)
{
	// defer to engine for software rendering
	if (!gHUD.ShouldScale())
	{
		gEngfuncs.pfnSPR_DrawHoles(frame, x, y, prc);
		return;
	}

	SPR_DrawInternal(frame, x, y, prc, kRenderTransAlpha);
}


void SPR_DrawAdditive(int frame, int x, int y, const Rect* prc)
{
	// defer to engine for software rendering
	if (!gHUD.ShouldScale())
	{
		gEngfuncs.pfnSPR_DrawAdditive(frame, x, y, prc);
		return;
	}

	SPR_DrawInternal(frame, x, y, prc, kRenderTransAdd);
}


void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a)
{
	gEngfuncs.pfnFillRGBA(
		flOffsetX + flScaleX * x,
		flOffsetY + flScaleY * y,
		flScaleX * width,
		flScaleY * height,
		r, g, b, a);
}


void SetCrosshair(HSPRITE hspr, Rect rc, int r, int g, int b)
{
	// defer to engine for software rendering
	if (!gHUD.ShouldScale())
	{
		gEngfuncs.pfnSetCrosshair(hspr, rc, r, g, b);
	}
	else
	{
		gEngfuncs.pfnSetCrosshair(0, rc, 0, 0, 0);
	}

	sCrosshair = hspr;
	sCrosshairRc = rc;
	sCrosshairColor.r = r;
	sCrosshairColor.g = g;
	sCrosshairColor.b = b;
}


int DrawConsoleString(int x, int y, const char* string)
{
	const int width = gEngfuncs.pfnDrawConsoleString(
		flOffsetX + flScaleX * x,
		flOffsetY + flScaleY * y,
		(char*)string);

	return (width - flOffsetX) / flScaleX;
}


void GetConsoleStringSize(const char* string, int* width, int* height)
{
	gEngfuncs.pfnDrawConsoleStringLen(string, width, height);

	*width = *width / flScaleX;
	*height = *height / flScaleY;
}


int DrawString(int x, int y, const char* str, int r, int g, int b)
{
	const int width = gEngfuncs.pfnDrawString(
		flOffsetX + flScaleX * x, 
		flOffsetY + flScaleY * y,
		str, r, g, b);

	return x + (width - flOffsetX) / flScaleX;
}


int DrawStringReverse(int x, int y, const char* str, int r, int g, int b)
{
	const int width = gEngfuncs.pfnDrawStringReverse(
		flOffsetX + flScaleX * x, 
		flOffsetY + flScaleY * y,
		str, r, g, b);

	return x + (width - flOffsetX) / flScaleX;
}


static Vector2D GetCrosshairPosition(const ref_params_t& params)
{
	Vector2D crosshair;

	// Place the crosshair at the center of the viewport
	crosshair.x = params.viewport[0] + params.viewport[2] / 2;
	crosshair.y = params.viewport[1] + params.viewport[3] / 2;

	// Adjust for auto-aim
	if (params.crosshairangle[0] != 0.0F || params.crosshairangle[1] != 0.0F)
	{
		Vector angles = (float*)params.viewangles;
		angles = angles + (float*)params.crosshairangle;

		Vector forward;
		AngleVectors(angles, forward, nullptr, nullptr);

		Vector world = (float*)params.vieworg;
		world = world + forward;

		Vector screen;

		gEngfuncs.pTriAPI->WorldToScreen(world, screen);

		crosshair.x += params.viewport[2] / 2 * screen.x + 0.5F;
		crosshair.y += params.viewport[3] / 2 * screen.y + 0.5F;
	}

	return crosshair;
}


void CL_DrawCrosshair()
{
	// defer to engine for software rendering
	if (!gHUD.ShouldScale())
	{
		return;
	}

	if (crosshair->value == 0.0F)
	{
		return;
	}

	// Don't draw if the player is dead or looking through a camera
	if (gHUD.m_fPlayerDead || !EV_IsLocal(gRefParams.viewentity))
	{
		return;
	}

	if (!SPR_Setup(sCrosshair, 0, kRenderTransAlpha, sCrosshairColor))
	{
		return;
	}

	const int sprWidth = SPR_Width(sCrosshair, 0);
	const int sprHeight = SPR_Height(sCrosshair, 0);

	// vertices

	const int width = sCrosshairRc.right - sCrosshairRc.left;
	const int height = sCrosshairRc.bottom - sCrosshairRc.top;

	const Vector2D crosshair = GetCrosshairPosition(gRefParams);

	const Quad verts = {
		crosshair.x + flScaleX * width  / 2.0F,
		crosshair.x - flScaleX * width  / 2.0F,
		crosshair.y + flScaleY * height / 2.0F,
		crosshair.y - flScaleY * height / 2.0F,
	};

	// texcoords

	const Quad uv = {
		(sCrosshairRc.left   + kTexCoordOffset) / (float)sprWidth,
		(sCrosshairRc.right  - kTexCoordOffset) / (float)sprWidth,
		(sCrosshairRc.top    + kTexCoordOffset) / (float)sprHeight,
		(sCrosshairRc.bottom - kTexCoordOffset) / (float)sprHeight,
	};

	SPR_DrawQuad(verts, uv);

	SPR_Restore();
}


void CHud::UpdateScalingInfo()
{
	if (!ShouldScale())
	{
		// default values for functions that may still reference them

		m_iWidth = ScreenWidth;
		m_iHeight = ScreenHeight;

		flScaleX = flScaleY = 1.0F;
		flOffsetX = flOffsetY = 0.0F;

		return;
	}

	// hud reference size
	m_iWidth = V_min(640, ScreenWidth);
	m_iHeight = V_min(480, ScreenHeight);

	// set the scaling factor by using the minor axis

	if (ScreenWidth >= ScreenHeight)
	{
		// letterbox or widescreen

		flScaleX = flScaleY = ScreenHeight / (float)gHUD.GetHeight();

		// adapt to the real aspect ratio
		m_iWidth = m_iHeight * (ScreenWidth / (float)ScreenHeight);
	}
	else
	{
		// portrait

		flScaleX = flScaleY = ScreenWidth / (float)gHUD.GetWidth();

		// adapt to the real aspect ratio
		m_iHeight = m_iWidth * (ScreenHeight / (float)ScreenWidth);
	}

	// keep the reference sizes even
	m_iWidth = (int)(m_iWidth / 2) * 2;
	m_iHeight = (int)(m_iHeight / 2) * 2;

	// set the scaling offsets
	flOffsetX = (ScreenWidth - gHUD.GetWidth() * flScaleX) / 2.0;
	flOffsetY = (ScreenHeight - gHUD.GetHeight() * flScaleY) / 2.0;
}

