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

class Panel;

class VGUIAPI Layout
{
//private:
//	Panel* _panel;
public:
	Layout();
public:
	//virtual void setPanel(Panel* panel); //called by Panel::setLayout
	virtual void performLayout(Panel* panel);
};

}
