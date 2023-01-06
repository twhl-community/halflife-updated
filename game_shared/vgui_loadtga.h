//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "VGUI_BitmapTGA.h"

vgui::BitmapTGA* vgui_LoadTGA(char const *pFilename);
vgui::BitmapTGA* vgui_LoadTGANoInvertAlpha(char const *pFilename);
