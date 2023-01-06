/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
****/

// cmdlib.h

#ifndef __CMDLIB__
#define __CMDLIB__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#ifndef __CMDUTIL__
#define __CMDUTIL__
typedef int qboolean;
#undef true
#undef false
#define true 1
#define false 0

typedef unsigned char byte;
#endif

// the dec offsetof macro doesn't work very well...
#define myoffsetof(type, identifier) ((size_t) & ((type*)0)->identifier)


// set these before calling CheckParm
extern int myargc;
extern char** myargv;

void COM_FixSlashes(char* pname);

#ifndef WIN32
char* strupr(char* in);
char* strlwr(char* in);
#endif
int Q_strncasecmp(const char* s1, const char* s2, int n);
int Q_strcasecmp(const char* s1, const char* s2);
void Q_getwd(char* out);

int filelength(FILE* f);
int FileTime(const char* path);

void Q_mkdir(const char* path);

extern char qdir[1024];
extern char gamedir[1024];
void SetQdirFromPath();
char* ExpandArg(char* path);  // from cmd line
char* ExpandPath(char* path); // from scripts
char* ExpandPathAndArchive(char* path);


double I_FloatTime(void);

[[noreturn]] void Error(const char* error, ...);
int CheckParm(const char* check);

FILE* SafeOpenWrite(const char* filename);
FILE* SafeOpenRead(const char* filename);
void SafeRead(FILE* f, void* buffer, int count);
void SafeWrite(FILE* f, void* buffer, int count);

int LoadFile(const char* filename, void** bufferptr);
void SaveFile(const char* filename, void* buffer, int count);

void DefaultExtension(char* path, const char* extension);
void DefaultPath(char* path, char* basepath);
void StripFilename(char* path);
void StripExtension(char* path);

void ExtractFilePath(const char* path, char* dest);
void ExtractFileBase(const char* path, char* dest);
void ExtractFileExtension(const char* path, char* dest);

int ParseNum(const char* str);

short BigShort(short l);
short LittleShort(short l);
int BigLong(int l);
int LittleLong(int l);
float BigFloat(float l);
float LittleFloat(float l);

long flen(FILE* f);



char* COM_Parse(char* data);

extern char com_token[1024];
extern qboolean com_eof;

char* copystring(const char* s);


void CRC_Init(unsigned short* crcvalue);
void CRC_ProcessByte(unsigned short* crcvalue, byte data);
unsigned short CRC_Value(unsigned short crcvalue);

void CreatePath(char* path);
void QCopyFile(char* from, char* to);

extern qboolean archive;
extern char archivedir[1024];


extern qboolean verbose;
void qprintf(const char* format, ...);


typedef struct
{
	char name[56];
	int filepos, filelen;
} packfile_t;

typedef struct
{
	char id[4];
	int dirofs;
	int dirlen;
} packheader_t;


void ListPak(const char* pakname);

#endif
