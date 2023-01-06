/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#pragma once

void PM_ViewEntity();
void PM_DrawBBox(Vector mins, Vector maxs, Vector origin, int pcolor, float life);
void PM_ParticleLine(Vector start, Vector end, int pcolor, float life, float vert);
void PM_ShowClipBox();
