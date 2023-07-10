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

// upper design bounds

enum MAX_MAP
{
	MAX_MAP_HULLS = 4,

	MAX_MAP_MODELS = 400,
	MAX_MAP_BRUSHES = 4096,
	MAX_MAP_ENTITIES = 1024,
	MAX_MAP_ENTSTRING = (128 * 1024),

	MAX_MAP_PLANES = 32767,
	MAX_MAP_NODES = 32767,	   // because negative shorts are contents
	MAX_MAP_CLIPNODES = 32767, //
	MAX_MAP_LEAFS = 8192,
	MAX_MAP_VERTS = 65535,
	MAX_MAP_FACES = 65535,
	MAX_MAP_MARKSURFACES = 65535,
	MAX_MAP_TEXINFO = 8192,
	MAX_MAP_EDGES = 256000,
	MAX_MAP_SURFEDGES = 512000,
	MAX_MAP_TEXTURES = 512,
	MAX_MAP_MIPTEX = 0x200000,
	MAX_MAP_LIGHTING = 0x200000,
	MAX_MAP_VISIBILITY = 0x200000,

	MAX_MAP_PORTALS = 65536
};


enum MAX_KEYVALUE
{
	// key / value pair sizes

	MAX_KEY = 32,
	MAX_VALUE = 1024
};

//=============================================================================

enum VERSION
{
	BSPVERSION = 30,
	TOOLVERSION = 2
};

typedef struct
{
	int fileofs, filelen;
} lump_t;

enum LUMP
{
	LUMP_ENTITIES = 0,
	LUMP_PLANES,
	LUMP_TEXTURES,
	LUMP_VERTEXES,
	LUMP_VISIBILITY,
	LUMP_NODES,
	LUMP_TEXINFO,
	LUMP_FACES,
	LUMP_LIGHTING,
	LUMP_CLIPNODES,
	LUMP_LEAFS,
	LUMP_MARKSURFACES,
	LUMP_EDGES,
	LUMP_SURFEDGES,
	LUMP_MODELS,

	HEADER_LUMPS
};

typedef struct
{
	float mins[3], maxs[3];
	float origin[3];
	int headnode[MAX_MAP_HULLS];
	int visleafs; // not including the solid leaf 0
	int firstface, numfaces;
} dmodel_t;

typedef struct
{
	int version;
	lump_t lumps[HEADER_LUMPS];
} dheader_t;

typedef struct
{
	int nummiptex;
	int dataofs[4]; // [nummiptex]
} dmiptexlump_t;

constexpr int MIPLEVELS = 4;
typedef struct miptex_s
{
	char name[16];
	unsigned width, height;
	unsigned offsets[MIPLEVELS]; // four mip maps stored
} miptex_t;


typedef struct
{
	float point[3];
} dvertex_t;

enum PLANE_POS
{ // 0-2 are axial planes
	PLANE_X = 0,
	PLANE_Y,
	PLANE_Z,

	// 3-5 are non-axial planes snapped to the nearest
	PLANE_ANYX,
	PLANE_ANYY,
	PLANE_ANYZ
};

typedef struct
{
	float normal[3];
	float dist;
	int type; // PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
} dplane_t;



enum CONTENTS
{
	CONTENTS_EMPTY = -1,
	CONTENTS_SOLID = -2,
	CONTENTS_WATER = -3,
	CONTENTS_SLIME = -4,
	CONTENTS_LAVA = -5,
	CONTENTS_SKY = -6,
	CONTENTS_ORIGIN = -7, // removed at csg time
	CONTENTS_CLIP = -8,	  // changed to contents_solid

	CONTENTS_CURRENT_0 = -9,
	CONTENTS_CURRENT_90 = -10,
	CONTENTS_CURRENT_180 = -11,
	CONTENTS_CURRENT_270 = -12,
	CONTENTS_CURRENT_UP = -13,
	CONTENTS_CURRENT_DOWN = -14,

	CONTENTS_TRANSLUCENT = -15
};

// !!! if this is changed, it must be changed in asm_i386.h too !!!
typedef struct
{
	int planenum;
	short children[2]; // negative numbers are -(leafs+1), not nodes
	short mins[3];	   // for sphere culling
	short maxs[3];
	unsigned short firstface;
	unsigned short numfaces; // counting both sides
} dnode_t;

typedef struct
{
	int planenum;
	short children[2]; // negative numbers are contents
} dclipnode_t;


typedef struct texinfo_s
{
	float vecs[2][4]; // [s/t][xyz offset]
	int miptex;
	int flags;
} texinfo_t;
constexpr int TEX_SPECIAL = 1; // sky or slime, no lightmap or 256 subdivision

// note that edge 0 is never used, because negative edge nums are used for
// counterclockwise use of the edge in a face
typedef struct
{
	unsigned short v[2]; // vertex numbers
} dedge_t;

constexpr int MAXLIGHTMAPS = 4;
typedef struct
{
	short planenum;
	short side;

	int firstedge; // we must support > 64k edges
	short numedges;
	short texinfo;

	// lighting info
	byte styles[MAXLIGHTMAPS];
	int lightofs; // start of [numstyles*surfsize] samples
} dface_t;


enum AMBIENT
{
	AMBIENT_WATER = 0,
	AMBIENT_SKY,
	AMBIENT_SLIME,
	AMBIENT_LAVA,

	NUM_AMBIENTS // automatic ambient sounds
};

// leaf 0 is the generic CONTENTS_SOLID leaf, used for all solid areas
// all other leafs need visibility info
typedef struct
{
	int contents;
	int visofs; // -1 = no visibility info

	short mins[3]; // for frustum culling
	short maxs[3];

	unsigned short firstmarksurface;
	unsigned short nummarksurfaces;

	byte ambient_level[NUM_AMBIENTS];
} dleaf_t;


//============================================================================

#ifndef QUAKE_GAME

enum QUAKE_ANGLE
{
	ANGLE_UP = -1,
	ANGLE_DOWN = -2
};


// the utilities get to be lazy and just use large static arrays

extern int nummodels;
extern dmodel_t dmodels[MAX_MAP_MODELS];
extern int dmodels_checksum;

extern int visdatasize;
extern byte dvisdata[MAX_MAP_VISIBILITY];
extern int dvisdata_checksum;

extern int lightdatasize;
extern byte dlightdata[MAX_MAP_LIGHTING];
extern int dlightdata_checksum;

extern int texdatasize;
extern byte dtexdata[MAX_MAP_MIPTEX]; // (dmiptexlump_t)
extern int dtexdata_checksum;

extern int entdatasize;
extern char dentdata[MAX_MAP_ENTSTRING];
extern int dentdata_checksum;

extern int numleafs;
extern dleaf_t dleafs[MAX_MAP_LEAFS];
extern int dleafs_checksum;

extern int numplanes;
extern dplane_t dplanes[MAX_MAP_PLANES];
extern int dplanes_checksum;

extern int numvertexes;
extern dvertex_t dvertexes[MAX_MAP_VERTS];
extern int dvertexes_checksum;

extern int numnodes;
extern dnode_t dnodes[MAX_MAP_NODES];
extern int dnodes_checksum;

extern int numtexinfo;
extern texinfo_t texinfo[MAX_MAP_TEXINFO];
extern int texinfo_checksum;

extern int numfaces;
extern dface_t dfaces[MAX_MAP_FACES];
extern int dfaces_checksum;

extern int numclipnodes;
extern dclipnode_t dclipnodes[MAX_MAP_CLIPNODES];
extern int dclipnodes_checksum;

extern int numedges;
extern dedge_t dedges[MAX_MAP_EDGES];
extern int dedges_checksum;

extern int nummarksurfaces;
extern unsigned short dmarksurfaces[MAX_MAP_MARKSURFACES];
extern int dmarksurfaces_checksum;

extern int numsurfedges;
extern int dsurfedges[MAX_MAP_SURFEDGES];
extern int dsurfedges_checksum;

int FastChecksum(void* buffer, int bytes);

void DecompressVis(byte* in, byte* decompressed);
int CompressVis(byte* vis, byte* dest);

void LoadBSPFile(char* filename);
void WriteBSPFile(char* filename);
void PrintBSPFileSizes(void);

//===============


typedef struct epair_s
{
	struct epair_s* next;
	char* key;
	char* value;
} epair_t;

typedef struct
{
	vec3_t origin;
	int firstbrush;
	int numbrushes;
	epair_t* epairs;
} entity_t;

extern int num_entities;
extern entity_t entities[MAX_MAP_ENTITIES];

void ParseEntities(void);
void UnparseEntities(void);

void SetKeyValue(entity_t* ent, const char* key, char* value);
char* ValueForKey(entity_t* ent, const char* key);
// will return "" if not present

vec_t FloatForKey(entity_t* ent, const char* key);
void GetVectorForKey(entity_t* ent, const char* key, vec3_t vec);

epair_t* ParseEpair(void);

#endif
