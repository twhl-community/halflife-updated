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

#include <algorithm>
#include <cassert>
#include <limits>
#include <string>

#include "Platform.h"
#include "PlatformHeaders.h"

#ifdef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef LINUX
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#include "extdll.h"
#include "util.h"

#ifdef CLIENT_DLL
#include "hud.h"
#endif

#include "interface.h"

#include "filesystem_utils.h"

static CSysModule* g_pFileSystemModule = nullptr;

// Some methods used to launch the game don't set the working directory.
// This makes using relative paths that point to the game and/or mod directory difficult
// since C and C++ runtime APIs don't know to use the game directory as a base.
// As a workaround we look up the executable directory and use that as a base.
// See https://stackoverflow.com/a/1024937/1306648 for more platform-specific workarounds.
// The engine's filesystem doesn't provide functions to do this so we have to work around it.
static std::string g_GameDirectory;
static std::string g_ModDirectory;
static std::string g_ModDirectoryName;

static bool FileSystem_InitializeGameDirectory()
{
	std::string gameDirectory;

#ifdef WIN32
	const std::size_t BufferSize = MAX_PATH + 1;
	gameDirectory.resize(BufferSize);

	const DWORD charactersWritten = GetModuleFileNameA(NULL, gameDirectory.data(), BufferSize);

	if (charactersWritten == BufferSize)
	{
		// Path was truncated. Game is installed in the wrong location (Steam shouldn't allow this).
		return false;
	}
#else
	const std::size_t BufferSize = PATH_MAX + 1;
	gameDirectory.resize(BufferSize);

	const ssize_t charactersWritten = readlink("/proc/self/exe", gameDirectory.data(), BufferSize);

	if (charactersWritten < 0 || charactersWritten == BufferSize)
	{
		// Path was truncated. Game is installed in the wrong location (Steam shouldn't allow this).
		return false;
	}
#endif

	// Resize buffer to actual size.
	gameDirectory.resize(std::strlen(gameDirectory.c_str()));

	// Truncate to directory name.
	const std::size_t directoryEnd = gameDirectory.find_last_of(DefaultPathSeparatorChar);

	if (directoryEnd == std::string::npos)
	{
		return false;
	}

	gameDirectory.resize(directoryEnd);

	gameDirectory.shrink_to_fit();

	g_ModDirectoryName.resize(BufferSize);

#ifdef CLIENT_DLL
	g_ModDirectoryName = gEngfuncs.pfnGetGameDirectory();
#else
	g_engfuncs.pfnGetGameDir(g_ModDirectoryName.data());
	g_ModDirectoryName.resize(std::strlen(g_ModDirectoryName.c_str()));
#endif

	g_GameDirectory = std::move(gameDirectory);
	g_ModDirectory = g_GameDirectory + DefaultPathSeparatorChar + g_ModDirectoryName;

	return true;
}

bool FileSystem_LoadFileSystem()
{
	if (nullptr != g_pFileSystem)
	{
		//Already loaded.
		return true;
	}

	// Determine which filesystem to use.
#if defined(_WIN32)
	const char* szFsModule = "filesystem_stdio.dll";
#elif defined(OSX)
	const char* szFsModule = "filesystem_stdio.dylib";
#elif defined(LINUX)
	const char* szFsModule = "filesystem_stdio.so";
#else
#error
#endif

	// Get filesystem interface.
	// The library is located next to the game exe, so there is no need to resolve the path first.
	g_pFileSystemModule = Sys_LoadModule(szFsModule);

	assert(nullptr != g_pFileSystemModule);

	if (nullptr == g_pFileSystemModule)
	{
		return false;
	}

	CreateInterfaceFn fileSystemFactory = Sys_GetFactory(g_pFileSystemModule);

	if (nullptr == fileSystemFactory)
	{
		return false;
	}

	g_pFileSystem = reinterpret_cast<IFileSystem*>(fileSystemFactory(FILESYSTEM_INTERFACE_VERSION, nullptr));

	assert(nullptr != g_pFileSystem);

	if (nullptr == g_pFileSystem)
	{
		return false;
	}

	if (!FileSystem_InitializeGameDirectory())
	{
		return false;
	}

	return true;
}

void FileSystem_FreeFileSystem()
{
	if (nullptr != g_pFileSystem)
	{
		g_pFileSystem = nullptr;
	}

	if (nullptr != g_pFileSystemModule)
	{
		Sys_UnloadModule(g_pFileSystemModule);
		g_pFileSystemModule = nullptr;
	}
}

const std::string& FileSystem_GetModDirectoryName()
{
	return g_ModDirectoryName;
}

void FileSystem_FixSlashes(std::string& fileName)
{
	std::replace(fileName.begin(), fileName.end(), AlternatePathSeparatorChar, DefaultPathSeparatorChar);
}

time_t FileSystem_GetFileTime(const char* fileName)
{
	if (nullptr == fileName)
	{
		return 0;
	}

	std::string absoluteFileName = g_ModDirectory + DefaultPathSeparatorChar + fileName;

	FileSystem_FixSlashes(absoluteFileName);

#ifdef WIN32
	struct _stat64i32 buf;

	const int result = _stat64i32(absoluteFileName.c_str(), &buf);

	if (result != 0)
	{
		return 0;
	}

	const time_t value = std::max(buf.st_ctime, buf.st_mtime);

	return value;
#else
	struct stat buf;

	const int result = stat(absoluteFileName.c_str(), &buf);

	if (result != 0)
	{
		return 0;
	}

	const time_t value = std::max(buf.st_ctim.tv_sec, buf.st_mtim.tv_sec);

	return value;
#endif
}

bool FileSystem_CompareFileTime(const char* filename1, const char* filename2, int* iCompare)
{
	*iCompare = 0;

	if (!filename1 || !filename2)
	{
		return false;
	}

	const time_t time1 = FileSystem_GetFileTime(filename1);
	const time_t time2 = FileSystem_GetFileTime(filename2);

	if (time1 < time2)
	{
		*iCompare = -1;
	}
	else if (time1 > time2)
	{
		*iCompare = 1;
	}

	return true;
}

std::vector<std::byte> FileSystem_LoadFileIntoBuffer(const char* fileName, FileContentFormat format, const char* pathID)
{
	assert(nullptr != g_pFileSystem);

	if (nullptr == fileName)
	{
		return {};
	}

	if (FSFile file{fileName, "rb", pathID}; file)
	{
		const auto size = file.Size();

		std::vector<std::byte> buffer;

		buffer.resize(size + (format == FileContentFormat::Text ? 1 : 0));

		file.Read(buffer.data(), size);

		if (format == FileContentFormat::Text)
		{
			//Null terminate it in case it's actually text.
			buffer[size] = std::byte{'\0'};
		}

		return buffer;
	}

	ALERT(at_console, "FileSystem_LoadFileIntoBuffer: couldn't open file \"%s\" for reading\n", fileName);
	return {};
}

bool FileSystem_WriteTextToFile(const char* fileName, const char* text, const char* pathID)
{
	assert(nullptr != g_pFileSystem);

	if (nullptr == fileName || nullptr == text)
	{
		return false;
	}

	const std::size_t length = std::strlen(text);

	if (length > static_cast<std::size_t>(std::numeric_limits<int>::max()))
	{
		ALERT(at_console, "FileSystem_WriteTextToFile: text too long\n");
		return false;
	}

	if (FSFile file{fileName, "w", pathID}; file)
	{
		file.Write(text, length);

		return true;
	}

	ALERT(at_console, "FileSystem_WriteTextToFile: couldn't open file \"%s\" for writing\n", fileName);

	return false;
}

constexpr const char* ValveGameDirectoryPrefixes[] =
	{
		"valve",
		"gearbox",
		"bshift",
		"ricochet",
		"dmc",
		"cstrike",
		"czero", // Also covers Deleted Scenes (czeror)
		"dod",
		"tfc"};

bool UTIL_IsValveGameDirectory()
{
	const std::string& modDirectoryName = FileSystem_GetModDirectoryName();

	for (const auto prefix : ValveGameDirectoryPrefixes)
	{
		if (strnicmp(modDirectoryName.c_str(), prefix, strlen(prefix)) == 0)
		{
			return true;
		}
	}

	return false;
}
