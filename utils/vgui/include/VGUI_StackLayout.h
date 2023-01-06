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

class VGUIAPI StackLayout : public Layout
{
private:
	int  _vgap;
	bool _fitWide;
public:
	StackLayout(int vgap,bool fitWide);
public:
	virtual void performLayout(Panel* panel);
};

}
