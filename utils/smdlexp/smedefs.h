/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
****/

//===================================================================
// Useful macros
//
#define CONSTRUCTOR
#define DESTRUCTOR

#define EXPORT_THIS __declspec(dllexport)

#define DEFAULT_EXT _T("smd")

#define FStrEq(sz1, sz2) (wcscmp((sz1), (sz2)) == 0)


//===================================================================
// Class that implements the scene-export.
//
class SmdExportClass : public SceneExport
{
	friend INT_PTR CALLBACK ExportOptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	friend class DumpModelTEP;
	friend class DumpDeformsTEP;

public:
	CONSTRUCTOR SmdExportClass(void);
	DESTRUCTOR ~SmdExportClass(void);

	// Required by classes derived from SceneExport
	int ExtCount(void) { return 1; }
	const TCHAR* Ext(int i) { return DEFAULT_EXT; }
	const TCHAR* LongDesc(void) { return _T("Valve Skeletal Model Exporter for 3D Studio Max"); }
	const TCHAR* ShortDesc(void) { return _T("Valve SMD"); }
	const TCHAR* AuthorName(void) { return _T("Valve, LLC"); }
	const TCHAR* CopyrightMessage(void) { return _T("Copyright (c) 1998, Valve LLC"); }
	const TCHAR* OtherMessage1(void) { return _T(""); }
	const TCHAR* OtherMessage2(void) { return _T(""); }
	unsigned int Version(void) { return 201; }
	void ShowAbout(HWND hWnd) { return; }
	// virtual int				DoExport		(const TCHAR *name, ExpInterface *ei, Interface *i);
	int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts = FALSE, DWORD options = 0); // Export file

	// Integer constants for this class
	enum
	{
		MAX_NAME_CHARS = 70,
		UNDESIRABLE_NODE_MARKER = -7777
	};

	// For keeping info about each (non-ignored) 3dsMax node in the tree
	typedef struct
	{
		wchar_t szNodeName[MAX_NAME_CHARS]; // usefull for lookups
		Matrix3 mat3NodeTM;				 // node's transformation matrix (at time zero)
		Matrix3 mat3ObjectTM;			 // object-offset transformation matrix (at time zero)
		int imaxnodeParent;				 // cached index of parent node
		float xRotFirstFrame;			 // 1st frame's X rotation
		float yRotFirstFrame;			 // 1st frame's Y rotation
		float zRotFirstFrame;			 // 1st frame's Z rotation
	} MaxNode;
	MaxNode* m_rgmaxnode; // array of nodes
	long m_imaxnodeMac;	  // # of nodes

	// Animation metrics (gleaned from 3dsMax and cached for convenience)
	Interval m_intervalOfAnimation;
	TimeValue m_tvStart;
	TimeValue m_tvEnd;
	int m_tpf; // ticks-per-frame

protected:
	// Property stuff
	bool hasStringPropertyValue(const wchar_t* propertyName, const wchar_t* propertyValue, Interface* ip);
	const PROPVARIANT* getPropertyVariant(const wchar_t* propertyName, Interface* ip);
	void VariantToString(const PROPVARIANT* propertyVariant, WCHAR* buffer, int bufferSize);

private:
	BOOL CollectNodes(ExpInterface* expiface);
	BOOL DumpBones(FILE* pFile, ExpInterface* pexpiface);
	BOOL DumpRotations(FILE* pFile, ExpInterface* pexpiface);
	BOOL DumpModel(FILE* pFile, ExpInterface* pexpiface);
	BOOL DumpDeforms(FILE* pFile, ExpInterface* pexpiface);


	// Is this MAX file just the reference frame, or an animation?
	// If TRUE, the "bones" and "mesh" files will be created.
	// If FALSE, the "rots" file will be created.
	BOOL m_fReferenceFrame;
};


//===================================================================
// Basically just a ClassFactory for communicating with 3DSMAX.
//
class SmdExportClassDesc : public ClassDesc
{
public:
	int IsPublic(void) { return TRUE; }
	void* Create(BOOL loading = FALSE) { return new SmdExportClass; }
	const TCHAR* ClassName(void) { return _T("SmdExport"); }
	const TCHAR* NonLocalizedClassName(void) { return _T("SmdExport"); }
	SClass_ID SuperClassID(void) { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID(void) { return Class_ID(0x774a43fd, 0x794d2210); }
	const TCHAR* Category(void) { return _T(""); }
};


//===================================================================
// Tree Enumeration Callback
//		Just counts the nodes in the node tree
//
class CountNodesTEP : public ITreeEnumProc
{
public:
	virtual int callback(INode* node);
	int m_cNodes; // running count of nodes
};


//===================================================================
// Tree Enumeration Callback
//		Collects the nodes in the tree into the global array
//
class CollectNodesTEP : public ITreeEnumProc
{
public:
	virtual int callback(INode* node);
	SmdExportClass* m_phec;
};


//===================================================================
// Tree Enumeration Callback
//		Dumps the bone offsets to a file.
//
class DumpNodesTEP : public ITreeEnumProc
{
public:
	virtual int callback(INode* node);
	FILE* m_pfile; // write to this file
	SmdExportClass* m_phec;
};


//===================================================================
// Tree Enumeration Callback
//		Dumps the per-frame bone rotations to a file.
//
class DumpFrameRotationsTEP : public ITreeEnumProc
{
public:
	virtual int callback(INode* node);
	void cleanup(void);
	FILE* m_pfile;		  // write to this file
	TimeValue m_tvToDump; // dump snapshot at this frame time
	SmdExportClass* m_phec;
};


//===================================================================
// Tree Enumeration Callback
//		Dumps the triangle meshes to a file.
//
class DumpModelTEP : public ITreeEnumProc
{
public:
	virtual int callback(INode* node);
	void cleanup(void);
	FILE* m_pfile;		  // write to this file
	TimeValue m_tvToDump; // dump snapshot at this frame time
	SmdExportClass* m_phec;
	IPhyContextExport* m_mcExport;
	IPhysiqueExport* m_phyExport;
	Modifier* m_phyMod;

private:
	int InodeOfPhyVectex(int iVertex0);
	Point3 Pt3GetRVertexNormal(RVertex* prvertex, DWORD smGroupFace);
};
