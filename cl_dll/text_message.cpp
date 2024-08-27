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
//
// text_message.cpp
//
// implementation of CHudTextMessage class
//
// this class routes messages through titles.txt for localisation
//

#include <algorithm>
#include <cassert>
#include <iterator>

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"

#include "vgui_TeamFortressViewport.h"

DECLARE_MESSAGE(m_TextMessage, TextMsg);

bool CHudTextMessage::Init()
{
	HOOK_MESSAGE(TextMsg);

	gHUD.AddHudElem(this);

	Reset();

	return true;
}

// Searches through the string for any msg names (indicated by a '#')
// any found are looked up in titles.txt and the new message substituted
// the new value is pushed into dst_buffer
char* CHudTextMessage::LocaliseTextString(const char* msg, char* dst_buffer, int buffer_size)
{
	assert(buffer_size > 0);

	char* dst = dst_buffer;

	// Subtract one so we have space for the null terminator no matter what.
	std::size_t remainingBufferSize = buffer_size - 1;

	for (const char* src = msg; *src != '\0' && remainingBufferSize > 0;)
	{
		if (*src == '#')
		{
			// cut msg name out of string
			static char word_buf[255];
			const char* word_start = src;

			++src;

			{
				const auto end = std::find_if_not(src, src + std::strlen(src), [](auto c)
					{ return (c >= 'A' && c <= 'z') || (c >= '0' && c <= '9'); });

				const std::size_t nameLength = end - src;

				const std::size_t count = std::min(std::size(word_buf) - 1, nameLength);

				if (count < nameLength)
				{
					gEngfuncs.Con_DPrintf(
						"CHudTextMessage::LocaliseTextString: Token name starting at index %d too long in message \"%s\"\n",
						static_cast<int>(src - msg), msg);
				}

				std::strncpy(word_buf, src, count);
				word_buf[count] = '\0';

				src += nameLength;
			}

			// lookup msg name in titles.txt
			client_textmessage_t* clmsg = TextMessageGet(word_buf);
			if (clmsg && clmsg->pMessage)
			{
				// copy string into message over the msg name
				const std::size_t count = std::min(remainingBufferSize, std::strlen(clmsg->pMessage));

				std::strncpy(dst, clmsg->pMessage, count);

				dst += count;
				remainingBufferSize -= count;
				continue;
			}

			src = word_start;
		}

		*dst = *src;
		dst++;
		src++;

		--remainingBufferSize;
	}

	*dst = '\0'; // ensure null termination

	return dst_buffer;
}

// As above, but with a local static buffer
char* CHudTextMessage::BufferedLocaliseTextString(const char* msg)
{
	static char dst_buffer[1024];
	LocaliseTextString(msg, dst_buffer, std::size(dst_buffer));
	return dst_buffer;
}

// Simplified version of LocaliseTextString;  assumes string is only one word
const char* CHudTextMessage::LookupString(const char* msg, int* msg_dest)
{
	if (!msg)
		return "";

	// '#' character indicates this is a reference to a string in titles.txt, and not the string itself
	if (msg[0] == '#')
	{
		// this is a message name, so look up the real message
		client_textmessage_t* clmsg = TextMessageGet(msg + 1);

		if (!clmsg || !(clmsg->pMessage))
			return msg; // lookup failed, so return the original string

		if (msg_dest)
		{
			// check to see if titles.txt info overrides msg destination
			// if clmsg->effect is less than 0, then clmsg->effect holds -1 * message_destination
			if (clmsg->effect < 0) //
				*msg_dest = -clmsg->effect;
		}

		return clmsg->pMessage;
	}
	else
	{ // nothing special about this message, so just return the same string
		return msg;
	}
}

void StripEndNewlineFromString(char* str)
{
	int s = strlen(str) - 1;
	if (str[s] == '\n' || str[s] == '\r')
		str[s] = 0;
}

// converts all '\r' characters to '\n', so that the engine can deal with the properly
// returns a pointer to str
char* ConvertCRtoNL(char* str)
{
	for (char* ch = str; *ch != 0; ch++)
		if (*ch == '\r')
			*ch = '\n';
	return str;
}

// Message handler for text messages
// displays a string, looking them up from the titles.txt file, which can be localised
// parameters:
//   byte:   message direction  ( HUD_PRINTCONSOLE, HUD_PRINTNOTIFY, HUD_PRINTCENTER, HUD_PRINTTALK )
//   string: message
// optional parameters:
//   string: message parameter 1
//   string: message parameter 2
//   string: message parameter 3
//   string: message parameter 4
// any string that starts with the character '#' is a message name, and is used to look up the real message in titles.txt
// the next (optional) one to four strings are parameters for that string (which can also be message names if they begin with '#')
bool CHudTextMessage::MsgFunc_TextMsg(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int msg_dest = READ_BYTE();

#define MSG_BUF_SIZE 128
	static char szBuf[6][MSG_BUF_SIZE];
	const char* msg_text = LookupString(READ_STRING(), &msg_dest);
	msg_text = safe_strcpy(szBuf[0], msg_text, MSG_BUF_SIZE);

	// keep reading strings and using C format strings for subsituting the strings into the localised text string
	const char* tempsstr1 = LookupString(READ_STRING());
	char* sstr1 = safe_strcpy(szBuf[1], tempsstr1, MSG_BUF_SIZE);
	StripEndNewlineFromString(sstr1); // these strings are meant for subsitution into the main strings, so cull the automatic end newlines
	const char* tempsstr2 = LookupString(READ_STRING());
	char* sstr2 = safe_strcpy(szBuf[2], tempsstr2, MSG_BUF_SIZE);
	StripEndNewlineFromString(sstr2);
	const char* tempsstr3 = LookupString(READ_STRING());
	char* sstr3 = safe_strcpy(szBuf[3], tempsstr3, MSG_BUF_SIZE);
	StripEndNewlineFromString(sstr3);
	const char* tempsstr4 = LookupString(READ_STRING());
	char* sstr4 = safe_strcpy(szBuf[4], tempsstr4, MSG_BUF_SIZE);
	StripEndNewlineFromString(sstr4);
	char* psz = szBuf[5];

	if (gViewPort && !gViewPort->AllowedToPrintText())
		return true;

	switch (msg_dest)
	{
	case HUD_PRINTCENTER:
		safe_sprintf(psz, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4);
		CenterPrint(ConvertCRtoNL(psz));
		break;

	case HUD_PRINTNOTIFY:
		psz[0] = 1; // mark this message to go into the notify buffer
		safe_sprintf(psz + 1, MSG_BUF_SIZE - 1, msg_text, sstr1, sstr2, sstr3, sstr4);
		ConsolePrint(ConvertCRtoNL(psz));
		break;

	case HUD_PRINTTALK:
		safe_sprintf(psz, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4);
		gHUD.m_SayText.SayTextPrint(ConvertCRtoNL(psz), 128);
		break;

	case HUD_PRINTCONSOLE:
		safe_sprintf(psz, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4);
		ConsolePrint(ConvertCRtoNL(psz));
		break;
	}

	return true;
}
