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

#pragma once

/**
*	@file
*
*	Functions, types and globals to load and use the GoldSource engine filesystem interface to read and write files.
*	See the VDC for information on which search paths exist to be used as path IDs:
*	https://developer.valvesoftware.com/wiki/GoldSource_SteamPipe_Directories
*/

#include <cstddef>
#include <ctime>
#include <string>
#include <vector>

#include "Platform.h"
#include "FileSystem.h"

#ifdef WIN32
constexpr char DefaultPathSeparatorChar = '\\';
constexpr char AlternatePathSeparatorChar = '/';
#else
constexpr char DefaultPathSeparatorChar = '/';
constexpr char AlternatePathSeparatorChar = '\\';
#endif

inline IFileSystem* g_pFileSystem = nullptr;

bool FileSystem_LoadFileSystem();
void FileSystem_FreeFileSystem();

/**
*	@brief Returns the mod directory name. Only valid to call after calling FileSystem_LoadFileSystem.
*/
const std::string& FileSystem_GetModDirectoryName();

/**
*	@brief Replaces occurrences of ::AlternatePathSeparatorChar with ::DefaultPathSeparatorChar.
*/
void FileSystem_FixSlashes(std::string& fileName);

/**
*	@brief Returns the last modification time of the given file.
*	Filenames are relative to the game directory.
*/
time_t FileSystem_GetFileTime(const char* fileName);

/**
*	@brief Compares the file time of the given files located in the mod directory.
*	@details Needed because IFileSystem::GetFileTime() does not provide a path ID parameter.
*	@param filename1 First file to compare.
*	@param filename2 Second file to compare.
*	@param[out] iCompare Stores the result of the comparison.
*		-@c 0 if equal
*		-@c -1 if @p filename2 is newer than @p filename1
*		-@c 1 if @p filename1 is newer than @p filename2
*	@return @c true if filetimes were retrieved, false otherwise.
*/
bool FileSystem_CompareFileTime(const char* filename1, const char* filename2, int* iCompare);

enum class FileContentFormat
{
	Binary = 0,
	Text = 1
};

/**
*	@brief Loads a file from disk into a buffer.
*
*	@details If the returned buffer contains text data and @p format is @c FileContentFormat::Text it is safe to cast the data pointer to char*:
*	@code{.cpp}
*	auto text = reinterpret_cast<char*>(buffer.data());
*	@endcode
*
*	@param fileName Name of the file to load.
*	@param format If @c FileContentFormat::Text, a null terminator will be appended.
*	@param pathID If not null, only looks for the file in this search path.
*	@return If the file was successfully loaded the contents of the buffer,
*		with a zero byte (null terminator) appended to it if @p format is @c FileContentFormat::Text.
*		If the file could not be loaded an empty buffer is returned.
*/
std::vector<std::byte> FileSystem_LoadFileIntoBuffer(const char* fileName, FileContentFormat format, const char* pathID = nullptr);

/**
*	@brief Writes a text file to disk.
*	@param fileName Name of the file to write to.
*	@param text Null-terminated text to write. The null terminator is not written to disk.
*	@param pathID If not null, writes to a writable location assigned to the given search path.
*		Otherwise the first writable location will be used (in practice this will be the mod directory).
*		If no writable location exists no file will be written to.
*	@return True if the file was written, false if an error occurred.
*/
bool FileSystem_WriteTextToFile(const char* fileName, const char* text, const char* pathID = nullptr);

/**
*	@brief Returns @c true if the current game directory is that of a Valve game.
*	Any directory whose name starts with that of a Valve game's directory name is considered to be one, matching Steam's behavior.
*/
bool UTIL_IsValveGameDirectory();

/**
*	@brief Helper class to automatically close the file handle associated with a file.
*/
class FSFile
{
public:
	FSFile() noexcept = default;
	FSFile(const char* fileName, const char* options, const char* pathID = nullptr);

	FSFile(FSFile&& other) noexcept
		: _handle(other._handle)
	{
		other._handle = FILESYSTEM_INVALID_HANDLE;
	}

	FSFile& operator=(FSFile&& other) noexcept
	{
		if (this != &other)
		{
			Close();
			_handle = other._handle;
			other._handle = FILESYSTEM_INVALID_HANDLE;
		}

		return *this;
	}

	FSFile(const FSFile&) = delete;
	FSFile& operator=(const FSFile&) = delete;

	~FSFile();

	constexpr bool IsOpen() const { return _handle != FILESYSTEM_INVALID_HANDLE; }

	std::size_t Size() const { return static_cast<std::size_t>(g_pFileSystem->Size(_handle)); }

	bool Open(const char* filename, const char* options, const char* pathID = nullptr);
	void Close();

	void Seek(int pos, FileSystemSeek_t seekType);

	int Read(void* dest, int size);

	int Write(const void* input, int size);

	template <typename... Args>
	int Printf(const char* format, Args&&... args)
	{
		return g_pFileSystem->FPrintf(_handle, format, std::forward<Args>(args)...);
	}

	constexpr operator bool() const { return IsOpen(); }

private:
	FileHandle_t _handle = FILESYSTEM_INVALID_HANDLE;
};

inline FSFile::FSFile(const char* filename, const char* options, const char* pathID)
{
	Open(filename, options, pathID);
}

inline FSFile::~FSFile()
{
	Close();
}

inline bool FSFile::Open(const char* filename, const char* options, const char* pathID)
{
	Close();

	_handle = g_pFileSystem->Open(filename, options, pathID);

	return IsOpen();
}

inline void FSFile::Close()
{
	if (IsOpen())
	{
		g_pFileSystem->Close(_handle);
		_handle = FILESYSTEM_INVALID_HANDLE;
	}
}

inline void FSFile::Seek(int pos, FileSystemSeek_t seekType)
{
	if (IsOpen())
	{
		g_pFileSystem->Seek(_handle, pos, seekType);
	}
}

inline int FSFile::Read(void* dest, int size)
{
	return g_pFileSystem->Read(dest, size, _handle);
}

inline int FSFile::Write(const void* input, int size)
{
	return g_pFileSystem->Write(input, size, _handle);
}
