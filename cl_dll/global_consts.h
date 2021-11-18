//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

enum 
{ 
	MAX_PLAYERS = 64,
	MAX_TEAMS = 64,
	MAX_TEAM_NAME = 16,
};

#define MAX_SCORES					10
#define MAX_SCOREBOARD_TEAMS		5

#define NUM_ROWS		(MAX_PLAYERS + (MAX_SCOREBOARD_TEAMS * 2))

#define MAX_SERVERNAME_LENGTH	64
#define MAX_TEAMNAME_SIZE 32
