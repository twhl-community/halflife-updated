/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
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
//
// death notice
//
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>

#include "vgui_TeamFortressViewport.h"

DECLARE_MESSAGE(m_DeathNotice, DeathMsg);

struct DeathNoticeItem
{
	char szKiller[MAX_PLAYER_NAME_LENGTH * 2];
	char szVictim[MAX_PLAYER_NAME_LENGTH * 2];
	int iId; // the index number of the associated sprite
	bool iSuicide;
	bool iTeamKill;
	bool iNonPlayerKill;
	float flDisplayTime;
	float* KillerColor;
	float* VictimColor;
};

#define MAX_DEATHNOTICES 4
static int DEATHNOTICE_DISPLAY_TIME = 6;

#define DEATHNOTICE_TOP 64

DeathNoticeItem rgDeathNoticeList[MAX_DEATHNOTICES + 1];

float g_ColorBlue[3] = {0.6, 0.8, 1.0};
float g_ColorRed[3] = {1.0, 0.25, 0.25};
float g_ColorGreen[3] = {0.6, 1.0, 0.6};
float g_ColorYellow[3] = {1.0, 0.7, 0.0};
float g_ColorGrey[3] = {0.8, 0.8, 0.8};

float* GetClientColor(int clientIndex)
{
	switch (g_PlayerExtraInfo[clientIndex].teamnumber)
	{
	case 1:
		return g_ColorBlue;
	case 2:
		return g_ColorRed;
	case 3:
		return g_ColorYellow;
	case 4:
		return g_ColorGreen;
	case 0:
		return g_ColorYellow;

	default:
		return g_ColorGrey;
	}

	return NULL;
}

bool CHudDeathNotice::Init()
{
	gHUD.AddHudElem(this);

	HOOK_MESSAGE(DeathMsg);

	CVAR_CREATE("hud_deathnotice_time", "6", FCVAR_ARCHIVE);

	return true;
}


void CHudDeathNotice::InitHUDData()
{
	memset(rgDeathNoticeList, 0, sizeof(rgDeathNoticeList));
}


bool CHudDeathNotice::VidInit()
{
	m_HUD_d_skull = gHUD.GetSpriteIndex("d_skull");

	return true;
}

bool CHudDeathNotice::Draw(float flTime)
{
	int x, y, r, g, b, texty;

	int gap = 20;

	Rect sprite = gHUD.GetSpriteRect(m_HUD_d_skull);
	gap = sprite.bottom - sprite.top;

	SCREENINFO screenInfo;
	screenInfo.iSize = sizeof(SCREENINFO);
	gEngfuncs.pfnGetScreenInfo(&screenInfo);
	gap = V_max(gap, screenInfo.iCharHeight);

	for (int i = 0; i < MAX_DEATHNOTICES; i++)
	{
		if (rgDeathNoticeList[i].iId == 0)
			break; // we've gone through them all

		if (rgDeathNoticeList[i].flDisplayTime < flTime)
		{ // display time has expired
			// remove the current item from the list
			memmove(&rgDeathNoticeList[i], &rgDeathNoticeList[i + 1], sizeof(DeathNoticeItem) * (MAX_DEATHNOTICES - i));
			i--; // continue on the next item;  stop the counter getting incremented
			continue;
		}

		rgDeathNoticeList[i].flDisplayTime = V_min(rgDeathNoticeList[i].flDisplayTime, gHUD.m_flTime + DEATHNOTICE_DISPLAY_TIME);

		// Only draw if the viewport will let me
		if (gViewPort && gViewPort->AllowedToPrintText())
		{
			// Draw the death notice
			y = DEATHNOTICE_TOP + 2 + (gap * i);

			texty = y + 4;

			int id = (rgDeathNoticeList[i].iId == -1) ? m_HUD_d_skull : rgDeathNoticeList[i].iId;
			x = gHUD.GetWidth() - ConsoleStringLen(rgDeathNoticeList[i].szVictim) - (gHUD.GetSpriteRect(id).right - gHUD.GetSpriteRect(id).left) - 4;

			if (!rgDeathNoticeList[i].iSuicide)
			{
				x -= (5 + ConsoleStringLen(rgDeathNoticeList[i].szKiller));

				// Draw killers name
				if (rgDeathNoticeList[i].KillerColor)
					gEngfuncs.pfnDrawSetTextColor(rgDeathNoticeList[i].KillerColor[0], rgDeathNoticeList[i].KillerColor[1], rgDeathNoticeList[i].KillerColor[2]);
				x = 5 + DrawConsoleString(x, texty, rgDeathNoticeList[i].szKiller);
			}

			r = 255;
			g = 80;
			b = 0;
			if (rgDeathNoticeList[i].iTeamKill)
			{
				r = 10;
				g = 240;
				b = 10; // display it in sickly green
			}

			// Draw death weapon
			SPR_Set(gHUD.GetSprite(id), r, g, b);
			SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(id));

			x += (gHUD.GetSpriteRect(id).right - gHUD.GetSpriteRect(id).left);

			// Draw victims name (if it was a player that was killed)
			if (!rgDeathNoticeList[i].iNonPlayerKill)
			{
				if (rgDeathNoticeList[i].VictimColor)
					gEngfuncs.pfnDrawSetTextColor(rgDeathNoticeList[i].VictimColor[0], rgDeathNoticeList[i].VictimColor[1], rgDeathNoticeList[i].VictimColor[2]);
				x = DrawConsoleString(x, texty, rgDeathNoticeList[i].szVictim);
			}
		}
	}

	return true;
}

// This message handler may be better off elsewhere
bool CHudDeathNotice::MsgFunc_DeathMsg(const char* pszName, int iSize, void* pbuf)
{
	m_iFlags |= HUD_ACTIVE;

	BEGIN_READ(pbuf, iSize);

	int killer = READ_BYTE();
	int victim = READ_BYTE();

	char killedwith[32];
	strcpy(killedwith, "d_");
	strncat(killedwith, READ_STRING(), 32);

	if (gViewPort)
		gViewPort->DeathMsg(killer, victim);

	gHUD.m_Spectator.DeathMessage(victim);
	int i;
	for (i = 0; i < MAX_DEATHNOTICES; i++)
	{
		if (rgDeathNoticeList[i].iId == 0)
			break;
	}
	if (i == MAX_DEATHNOTICES)
	{ // move the rest of the list forward to make room for this item
		memmove(rgDeathNoticeList, rgDeathNoticeList + 1, sizeof(DeathNoticeItem) * MAX_DEATHNOTICES);
		i = MAX_DEATHNOTICES - 1;
	}

	if (gViewPort)
		gViewPort->GetAllPlayersInfo();

	// Get the Killer's name
	const char* killer_name = g_PlayerInfoList[killer].name;
	if (!killer_name)
	{
		killer_name = "";
		rgDeathNoticeList[i].szKiller[0] = 0;
	}
	else
	{
		rgDeathNoticeList[i].KillerColor = GetClientColor(killer);
		strncpy(rgDeathNoticeList[i].szKiller, killer_name, MAX_PLAYER_NAME_LENGTH);
		rgDeathNoticeList[i].szKiller[MAX_PLAYER_NAME_LENGTH - 1] = 0;
	}

	// Get the Victim's name
	const char* victim_name = NULL;
	// If victim is -1, the killer killed a specific, non-player object (like a sentrygun)
	if (((char)victim) != -1)
		victim_name = g_PlayerInfoList[victim].name;
	if (!victim_name)
	{
		victim_name = "";
		rgDeathNoticeList[i].szVictim[0] = 0;
	}
	else
	{
		rgDeathNoticeList[i].VictimColor = GetClientColor(victim);
		strncpy(rgDeathNoticeList[i].szVictim, victim_name, MAX_PLAYER_NAME_LENGTH);
		rgDeathNoticeList[i].szVictim[MAX_PLAYER_NAME_LENGTH - 1] = 0;
	}

	// Is it a non-player object kill?
	if (((char)victim) == -1)
	{
		rgDeathNoticeList[i].iNonPlayerKill = true;

		// Store the object's name in the Victim slot (skip the d_ bit)
		strcpy(rgDeathNoticeList[i].szVictim, killedwith + 2);
	}
	else
	{
		if (killer == victim || killer == 0)
			rgDeathNoticeList[i].iSuicide = true;

		if (0 == strcmp(killedwith, "d_teammate"))
			rgDeathNoticeList[i].iTeamKill = true;
	}

	// Find the sprite in the list
	int spr = gHUD.GetSpriteIndex(killedwith);

	rgDeathNoticeList[i].iId = spr;

	DEATHNOTICE_DISPLAY_TIME = CVAR_GET_FLOAT("hud_deathnotice_time");
	rgDeathNoticeList[i].flDisplayTime = gHUD.m_flTime + DEATHNOTICE_DISPLAY_TIME;

	if (rgDeathNoticeList[i].iNonPlayerKill)
	{
		ConsolePrint(rgDeathNoticeList[i].szKiller);
		ConsolePrint(" killed a ");
		ConsolePrint(rgDeathNoticeList[i].szVictim);
		ConsolePrint("\n");
	}
	else
	{
		// record the death notice in the console
		if (rgDeathNoticeList[i].iSuicide)
		{
			ConsolePrint(rgDeathNoticeList[i].szVictim);

			if (0 == strcmp(killedwith, "d_world"))
			{
				ConsolePrint(" died");
			}
			else
			{
				ConsolePrint(" killed self");
			}
		}
		else if (rgDeathNoticeList[i].iTeamKill)
		{
			ConsolePrint(rgDeathNoticeList[i].szKiller);
			ConsolePrint(" killed his teammate ");
			ConsolePrint(rgDeathNoticeList[i].szVictim);
		}
		else
		{
			ConsolePrint(rgDeathNoticeList[i].szKiller);
			ConsolePrint(" killed ");
			ConsolePrint(rgDeathNoticeList[i].szVictim);
		}

		if (killedwith && '\0' != *killedwith && (*killedwith > 13) && 0 != strcmp(killedwith, "d_world") && !rgDeathNoticeList[i].iTeamKill)
		{
			ConsolePrint(" with ");

			// replace the code names with the 'real' names
			if (0 == strcmp(killedwith + 2, "egon"))
				strcpy(killedwith, "d_gluon gun");
			if (0 == strcmp(killedwith + 2, "gauss"))
				strcpy(killedwith, "d_tau cannon");

			ConsolePrint(killedwith + 2); // skip over the "d_" part
		}

		ConsolePrint("\n");
	}

	return true;
}
