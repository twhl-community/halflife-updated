/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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
// Implementation in UTIL.CPP

#pragma once

class CBaseEntity;

class CSaveRestoreBuffer
{
public:
	CSaveRestoreBuffer(SAVERESTOREDATA& data);
	~CSaveRestoreBuffer();

	int EntityIndex(entvars_t* pevLookup);
	int EntityIndex(edict_t* pentLookup);
	int EntityIndex(EOFFSET eoLookup);
	int EntityIndex(CBaseEntity* pEntity);

	int EntityFlags(int entityIndex, int flags) { return EntityFlagsSet(entityIndex, 0); }
	int EntityFlagsSet(int entityIndex, int flags);

	edict_t* EntityFromIndex(int entityIndex);

	unsigned short TokenHash(const char* pszToken);

	const SAVERESTOREDATA& GetData() const { return m_data; }

	//Data is only valid if it's a valid pointer and if it has a token list
	[[nodiscard]] static bool IsValidSaveRestoreData(SAVERESTOREDATA* data)
	{
		const bool isValid = nullptr != data && nullptr != data->pTokens && data->tokenCount > 0;

		ASSERT(isValid);

		if (!isValid)
		{
			int x = 10;
		}

		return isValid;
	}

protected:
	SAVERESTOREDATA& m_data;
	void BufferRewind(int size);
	unsigned int HashString(const char* pszToken);
};


class CSave : public CSaveRestoreBuffer
{
public:
	using CSaveRestoreBuffer::CSaveRestoreBuffer;

	void WriteShort(const char* pname, const short* value, int count);
	void WriteInt(const char* pname, const int* value, int count);				// Save an int
	void WriteFloat(const char* pname, const float* value, int count);			// Save a float
	void WriteTime(const char* pname, const float* value, int count);			// Save a float (timevalue)
	void WriteData(const char* pname, int size, const char* pdata);				// Save a binary data block
	void WriteString(const char* pname, const char* pstring);					// Save a null-terminated string
	void WriteString(const char* pname, const int* stringId, int count);		// Save a null-terminated string (engine string)
	void WriteVector(const char* pname, const Vector& value);					// Save a vector
	void WriteVector(const char* pname, const float* value, int count);			// Save a vector
	void WritePositionVector(const char* pname, const Vector& value);			// Offset for landmark if necessary
	void WritePositionVector(const char* pname, const float* value, int count); // array of pos vectors
	void WriteFunction(const char* pname, void** value, int count);				// Save a function pointer
	bool WriteEntVars(const char* pname, entvars_t* pev);						// Save entvars_t (entvars_t)
	bool WriteFields(const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount);

private:
	bool DataEmpty(const char* pdata, int size);
	void BufferField(const char* pname, int size, const char* pdata);
	void BufferString(char* pdata, int len);
	void BufferData(const char* pdata, int size);
	void BufferHeader(const char* pname, int size);
};

typedef struct
{
	unsigned short size;
	unsigned short token;
	char* pData;
} HEADER;

class CRestore : public CSaveRestoreBuffer
{
public:
	using CSaveRestoreBuffer::CSaveRestoreBuffer;

	bool ReadEntVars(const char* pname, entvars_t* pev); // entvars_t
	bool ReadFields(const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount);
	int ReadField(void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount, int startField, int size, char* pName, void* pData);
	int ReadInt();
	short ReadShort();
	int ReadNamedInt(const char* pName);
	char* ReadNamedString(const char* pName);
	bool Empty() { return (m_data.pCurrentData - m_data.pBaseData) >= m_data.bufferSize; }
	void SetGlobalMode(bool global) { m_global = global; }
	void PrecacheMode(bool mode) { m_precache = mode; }

private:
	char* BufferPointer();
	void BufferReadBytes(char* pOutput, int size);
	void BufferSkipBytes(int bytes);
	int BufferSkipZString();
	bool BufferCheckZString(const char* string);

	void BufferReadHeader(HEADER* pheader);

	bool m_global = false; // Restoring a global entity?
	bool m_precache = true;
};

#define MAX_ENTITYARRAY 64

//#define ARRAYSIZE(p)		(sizeof(p)/sizeof(p[0]))

#define IMPLEMENT_SAVERESTORE(derivedClass, baseClass)                                     \
	bool derivedClass::Save(CSave& save)                                                   \
	{                                                                                      \
		if (!baseClass::Save(save))                                                        \
			return false;                                                                  \
		return save.WriteFields(#derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData));   \
	}                                                                                      \
	bool derivedClass::Restore(CRestore& restore)                                          \
	{                                                                                      \
		if (!baseClass::Restore(restore))                                                  \
			return false;                                                                  \
		return restore.ReadFields(#derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData)); \
	}


typedef enum
{
	GLOBAL_OFF = 0,
	GLOBAL_ON = 1,
	GLOBAL_DEAD = 2
} GLOBALESTATE;

typedef struct globalentity_s globalentity_t;

struct globalentity_s
{
	char name[64];
	char levelName[32];
	GLOBALESTATE state;
	globalentity_t* pNext;
};

class CGlobalState
{
public:
	CGlobalState();
	void Reset();
	void ClearStates();
	void EntityAdd(string_t globalname, string_t mapName, GLOBALESTATE state);
	void EntitySetState(string_t globalname, GLOBALESTATE state);
	void EntityUpdate(string_t globalname, string_t mapname);
	const globalentity_t* EntityFromTable(string_t globalname);
	GLOBALESTATE EntityGetState(string_t globalname);
	bool EntityInTable(string_t globalname) { return Find(globalname) != NULL; }
	bool Save(CSave& save);
	bool Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	//#ifdef _DEBUG
	void DumpGlobals();
	//#endif

private:
	globalentity_t* Find(string_t globalname);
	globalentity_t* m_pList;
	int m_listCount;
};

extern CGlobalState gGlobalState;
