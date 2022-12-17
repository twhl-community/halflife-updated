//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

void VGui_Startup();
void VGui_Shutdown();

//Only safe to call from inside subclass of Panel::paintBackground
void VGui_ViewportPaintBackground(int extents[4]);
