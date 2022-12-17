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

#include <cassert>
#include <limits>

#include "Platform.h"

#include "extdll.h"
#include "util.h"

#include "interface.h"

#include "filesystem_utils.h"

static CSysModule* g_pFileSystemModule = nullptr;

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
