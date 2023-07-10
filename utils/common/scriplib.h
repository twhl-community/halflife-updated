/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
****/

// scriplib.h

#pragma once

#include "cmdlib.h"

constexpr int MAXTOKEN = 512;

extern char token[MAXTOKEN];
extern char *scriptbuffer, *script_p, *scriptend_p;
extern int grabbed;
extern int scriptline;
extern bool endofscript;


void LoadScriptFile(char* filename);
void ParseFromMemory(char* buffer, int size);

bool GetToken(bool crossline);
void UnGetToken(void);
bool TokenAvailable(void);
