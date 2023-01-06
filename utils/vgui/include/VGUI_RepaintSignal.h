//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

namespace vgui
{
class Panel;
	
class RepaintSignal
{
public:
	virtual void panelRepainted(Panel* panel)=0;
};

}
