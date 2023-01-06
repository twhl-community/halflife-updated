//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include<VGUI.h>

namespace vgui
{

class Bitmap;

class VGUIAPI Cursor
{
public:
	enum DefaultCursor
	{
		dc_user,
		dc_none,
		dc_arrow,
		dc_ibeam,
		dc_hourglass,
		dc_crosshair,
		dc_up,
		dc_sizenwse,
		dc_sizenesw,
		dc_sizewe,
		dc_sizens,
		dc_sizeall,
		dc_no,
		dc_hand,
		dc_last,
	};
private:
	int           _hotspot[2];
	Bitmap*       _bitmap;
	DefaultCursor _dc;
public:
	Cursor(DefaultCursor dc);
	Cursor(Bitmap* bitmap,int hotspotX,int hotspotY);
public:
	virtual void getHotspot(int& x,int& y);
private:
	virtual void privateInit(Bitmap* bitmap,int hotspotX,int hotspotY);
public:
	virtual Bitmap*       getBitmap();
	virtual DefaultCursor getDefaultCursor();
};

}
