/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
****/

#pragma once

/*
	movie.h

	definitions and such for dumping screen shots to make a movie
*/

typedef struct
{
	unsigned long tag;
	unsigned long size;
} movieblockheader_t;


typedef struct
{
	short width;
	short height;
	short depth;
} movieframe_t;