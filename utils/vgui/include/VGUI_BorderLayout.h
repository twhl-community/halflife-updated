//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include<VGUI.h>
#include<VGUI_Layout.h>

namespace vgui
{

class LayoutInfo;

class VGUIAPI BorderLayout : public Layout
{
public:
	enum Alignment
	{
		a_center=0,
		a_north,
		a_south,
		a_east,
		a_west,
	};
private:
	int _inset;
public:
	BorderLayout(int inset);
public:
	virtual void        performLayout(Panel* panel);
	virtual LayoutInfo* createLayoutInfo(Alignment alignment);
};

}
