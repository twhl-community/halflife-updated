//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include<VGUI.h>
#include<VGUI_Label.h>

namespace vgui
{

class VGUIAPI MenuSeparator : public Label
{
public:
	MenuSeparator(const char* text);
protected:
	virtual void paintBackground();
};

}
