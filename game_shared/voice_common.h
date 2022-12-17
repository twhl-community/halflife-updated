//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "cdll_dll.h"
#include "bitvec.h"

constexpr int VOICE_MAX_PLAYERS_DW = (MAX_PLAYERS / 32) + ((MAX_PLAYERS & 31) != 0 ? 1 : 0);

typedef CBitVec<MAX_PLAYERS> CPlayerBitVec;
