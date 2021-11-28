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
//  parsemsg.h
//	MDC - copying from cstrike\cl_dll so career-mode stuff can catch messages
//  in this dll. (and C++ifying it)
//

#pragma once

#define ASSERT(x)
//--------------------------------------------------------------------------------------------------------------
void BEGIN_READ(void* buf, int size);
int READ_CHAR();
int READ_BYTE();
int READ_SHORT();
int READ_WORD();
int READ_LONG();
float READ_FLOAT();
char* READ_STRING();
float READ_COORD();
float READ_ANGLE();
float READ_HIRESANGLE();
bool READ_OK();

//--------------------------------------------------------------------------------------------------------------
class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(unsigned char* buffer, int bufferLen);
	void Init(unsigned char* buffer, int bufferLen);

	void WriteByte(unsigned char data);
	void WriteLong(int data);
	void WriteString(const char* str);

	bool HasOverflowed();
	int GetSpaceUsed();

protected:
	unsigned char* m_buffer;
	int m_remaining;
	bool m_overflow;
	int m_overallLength;
};

//--------------------------------------------------------------------------------------------------------------
