#pragma once

/**
*	@file
*
*	Header to include core platform-specific headers (e.g. Windows.h) without conflicting with SDK code
*	Include this header *after* Platform.h
*/

#ifdef WIN32

// Prevent tons of unused windows definitions
#define WIN32_LEAN_AND_MEAN

//Disable all Windows 10 and older APIs otherwise pulled in by Windows.h
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
//#define NOUSER //Need GetCursorPos in the mouse thread code
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

//Disable additional stuff not covered by the Windows.h list
#define NOWINRES
#define NOIME

//Fix problems with ARRAYSIZE and HSPRITE SDK definitions conflicting with Windows header definitions
//TODO: ARRAYSIZE should be replaced with std::size, which is a superior replacement
#pragma push_macro("ARRAYSIZE")
#ifdef ARRAYSIZE
#undef ARRAYSIZE
#endif
#define HSPRITE WINDOWS_HSPRITE

#include <Windows.h>

#undef HSPRITE
#pragma pop_macro("ARRAYSIZE")
#else
//Add core Linux headers here if you need them
#endif
