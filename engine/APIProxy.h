#pragma once

#include "Platform.h"
#include "netadr.h"
#include "Sequence.h"
#include "common_types.h"
#include "enums.h"

#define MAX_ALIAS_NAME 32

typedef struct cmdalias_s
{
	struct cmdalias_s* next;
	char name[MAX_ALIAS_NAME];
	char* value;
} cmdalias_t;


// ********************************************************
// Functions exported by the client .dll
// ********************************************************

// Function type declarations for client exports
typedef int (*INITIALIZE_FUNC)(struct cl_enginefuncs_s*, int);
typedef void (*HUD_INIT_FUNC)(void);
typedef int (*HUD_VIDINIT_FUNC)(void);
typedef int (*HUD_REDRAW_FUNC)(float, int);
typedef int (*HUD_UPDATECLIENTDATA_FUNC)(struct client_data_s*, float);
typedef void (*HUD_RESET_FUNC)(void);
typedef void (*HUD_CLIENTMOVE_FUNC)(struct playermove_s* ppmove, qboolean server);
typedef void (*HUD_CLIENTMOVEINIT_FUNC)(struct playermove_s* ppmove);
typedef char (*HUD_TEXTURETYPE_FUNC)(char* name);
typedef void (*HUD_IN_ACTIVATEMOUSE_FUNC)(void);
typedef void (*HUD_IN_DEACTIVATEMOUSE_FUNC)(void);
typedef void (*HUD_IN_MOUSEEVENT_FUNC)(int mstate);
typedef void (*HUD_IN_CLEARSTATES_FUNC)(void);
typedef void (*HUD_IN_ACCUMULATE_FUNC)(void);
typedef void (*HUD_CL_CREATEMOVE_FUNC)(float frametime, struct usercmd_s* cmd, int active);
typedef int (*HUD_CL_ISTHIRDPERSON_FUNC)(void);
typedef void (*HUD_CL_GETCAMERAOFFSETS_FUNC)(float* ofs);
typedef struct kbutton_s* (*HUD_KB_FIND_FUNC)(const char* name);
typedef void (*HUD_CAMTHINK_FUNC)(void);
typedef void (*HUD_CALCREF_FUNC)(struct ref_params_s* pparams);
typedef int (*HUD_ADDENTITY_FUNC)(int type, struct cl_entity_s* ent, const char* modelname);
typedef void (*HUD_CREATEENTITIES_FUNC)(void);
typedef void (*HUD_DRAWNORMALTRIS_FUNC)(void);
typedef void (*HUD_DRAWTRANSTRIS_FUNC)(void);
typedef void (*HUD_STUDIOEVENT_FUNC)(const struct mstudioevent_s* event, const struct cl_entity_s* entity);
typedef void (*HUD_POSTRUNCMD_FUNC)(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
typedef void (*HUD_SHUTDOWN_FUNC)(void);
typedef void (*HUD_TXFERLOCALOVERRIDES_FUNC)(struct entity_state_s* state, const struct clientdata_s* client);
typedef void (*HUD_PROCESSPLAYERSTATE_FUNC)(struct entity_state_s* dst, const struct entity_state_s* src);
typedef void (*HUD_TXFERPREDICTIONDATA_FUNC)(struct entity_state_s* ps, const struct entity_state_s* pps, struct clientdata_s* pcd, const struct clientdata_s* ppcd, struct weapon_data_s* wd, const struct weapon_data_s* pwd);
typedef void (*HUD_DEMOREAD_FUNC)(int size, unsigned char* buffer);
typedef int (*HUD_CONNECTIONLESS_FUNC)(const struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
typedef int (*HUD_GETHULLBOUNDS_FUNC)(int hullnumber, float* mins, float* maxs);
typedef void (*HUD_FRAME_FUNC)(double);
typedef int (*HUD_KEY_EVENT_FUNC)(int eventcode, int keynum, const char* pszCurrentBinding);
typedef void (*HUD_TEMPENTUPDATE_FUNC)(double frametime, double client_time, double cl_gravity, struct tempent_s** ppTempEntFree, struct tempent_s** ppTempEntActive, int (*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void (*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
typedef struct cl_entity_s* (*HUD_GETUSERENTITY_FUNC)(int index);
typedef void (*HUD_VOICESTATUS_FUNC)(int entindex, qboolean bTalking);
typedef void (*HUD_DIRECTORMESSAGE_FUNC)(int iSize, void* pbuf);
typedef int (*HUD_STUDIO_INTERFACE_FUNC)(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
typedef void (*HUD_CHATINPUTPOSITION_FUNC)(int* x, int* y);
typedef int (*HUD_GETPLAYERTEAM)(int iplayer);
typedef void* (*CLIENTFACTORY)(); // this should be CreateInterfaceFn but that means including interface.h
								  // which is a C++ file and some of the client files a C only...
								  // so we return a void * which we then do a typecast on later.


// Pointers to the exported client functions themselves
typedef struct
{
	INITIALIZE_FUNC pInitFunc;
	HUD_INIT_FUNC pHudInitFunc;
	HUD_VIDINIT_FUNC pHudVidInitFunc;
	HUD_REDRAW_FUNC pHudRedrawFunc;
	HUD_UPDATECLIENTDATA_FUNC pHudUpdateClientDataFunc;
	HUD_RESET_FUNC pHudResetFunc;
	HUD_CLIENTMOVE_FUNC pClientMove;
	HUD_CLIENTMOVEINIT_FUNC pClientMoveInit;
	HUD_TEXTURETYPE_FUNC pClientTextureType;
	HUD_IN_ACTIVATEMOUSE_FUNC pIN_ActivateMouse;
	HUD_IN_DEACTIVATEMOUSE_FUNC pIN_DeactivateMouse;
	HUD_IN_MOUSEEVENT_FUNC pIN_MouseEvent;
	HUD_IN_CLEARSTATES_FUNC pIN_ClearStates;
	HUD_IN_ACCUMULATE_FUNC pIN_Accumulate;
	HUD_CL_CREATEMOVE_FUNC pCL_CreateMove;
	HUD_CL_ISTHIRDPERSON_FUNC pCL_IsThirdPerson;
	HUD_CL_GETCAMERAOFFSETS_FUNC pCL_GetCameraOffsets;
	HUD_KB_FIND_FUNC pFindKey;
	HUD_CAMTHINK_FUNC pCamThink;
	HUD_CALCREF_FUNC pCalcRefdef;
	HUD_ADDENTITY_FUNC pAddEntity;
	HUD_CREATEENTITIES_FUNC pCreateEntities;
	HUD_DRAWNORMALTRIS_FUNC pDrawNormalTriangles;
	HUD_DRAWTRANSTRIS_FUNC pDrawTransparentTriangles;
	HUD_STUDIOEVENT_FUNC pStudioEvent;
	HUD_POSTRUNCMD_FUNC pPostRunCmd;
	HUD_SHUTDOWN_FUNC pShutdown;
	HUD_TXFERLOCALOVERRIDES_FUNC pTxferLocalOverrides;
	HUD_PROCESSPLAYERSTATE_FUNC pProcessPlayerState;
	HUD_TXFERPREDICTIONDATA_FUNC pTxferPredictionData;
	HUD_DEMOREAD_FUNC pReadDemoBuffer;
	HUD_CONNECTIONLESS_FUNC pConnectionlessPacket;
	HUD_GETHULLBOUNDS_FUNC pGetHullBounds;
	HUD_FRAME_FUNC pHudFrame;
	HUD_KEY_EVENT_FUNC pKeyEvent;
	HUD_TEMPENTUPDATE_FUNC pTempEntUpdate;
	HUD_GETUSERENTITY_FUNC pGetUserEntity;
	HUD_VOICESTATUS_FUNC pVoiceStatus;			   // Possibly null on old client dlls.
	HUD_DIRECTORMESSAGE_FUNC pDirectorMessage;	   // Possibly null on old client dlls.
	HUD_STUDIO_INTERFACE_FUNC pStudioInterface;	   // Not used by all clients
	HUD_CHATINPUTPOSITION_FUNC pChatInputPosition; // Not used by all clients
	HUD_GETPLAYERTEAM pGetPlayerTeam;			   // Not used by all clients
	CLIENTFACTORY pClientFactory;
} cldll_func_t;

// ********************************************************
// Functions exported by the engine
// ********************************************************

// Function type declarations for engine exports
typedef HSPRITE (*pfnEngSrc_pfnSPR_Load_t)(const char* szPicName);
typedef int (*pfnEngSrc_pfnSPR_Frames_t)(HSPRITE hPic);
typedef int (*pfnEngSrc_pfnSPR_Height_t)(HSPRITE hPic, int frame);
typedef int (*pfnEngSrc_pfnSPR_Width_t)(HSPRITE hPic, int frame);
typedef void (*pfnEngSrc_pfnSPR_Set_t)(HSPRITE hPic, int r, int g, int b);
typedef void (*pfnEngSrc_pfnSPR_Draw_t)(int frame, int x, int y, const Rect* prc);
typedef void (*pfnEngSrc_pfnSPR_DrawHoles_t)(int frame, int x, int y, const Rect* prc);
typedef void (*pfnEngSrc_pfnSPR_DrawAdditive_t)(int frame, int x, int y, const Rect* prc);
typedef void (*pfnEngSrc_pfnSPR_EnableScissor_t)(int x, int y, int width, int height);
typedef void (*pfnEngSrc_pfnSPR_DisableScissor_t)(void);
typedef struct client_sprite_s* (*pfnEngSrc_pfnSPR_GetList_t)(const char* psz, int* piCount);
typedef void (*pfnEngSrc_pfnFillRGBA_t)(int x, int y, int width, int height, int r, int g, int b, int a);
typedef int (*pfnEngSrc_pfnGetScreenInfo_t)(struct SCREENINFO_s* pscrinfo);
typedef void (*pfnEngSrc_pfnSetCrosshair_t)(HSPRITE hspr, Rect rc, int r, int g, int b);
typedef struct cvar_s* (*pfnEngSrc_pfnRegisterVariable_t)(const char* szName, const char* szValue, int flags);
typedef float (*pfnEngSrc_pfnGetCvarFloat_t)(const char* szName);
typedef const char* (*pfnEngSrc_pfnGetCvarString_t)(const char* szName);
typedef int (*pfnEngSrc_pfnAddCommand_t)(const char* cmd_name, void (*pfnEngSrc_function)(void));
typedef int (*pfnEngSrc_pfnHookUserMsg_t)(const char* szMsgName, pfnUserMsgHook pfn);
typedef int (*pfnEngSrc_pfnServerCmd_t)(const char* szCmdString);
typedef int (*pfnEngSrc_pfnClientCmd_t)(const char* szCmdString);
typedef void (*pfnEngSrc_pfnPrimeMusicStream_t)(const char* szFilename, int looping);
typedef void (*pfnEngSrc_pfnGetPlayerInfo_t)(int ent_num, struct hud_player_info_s* pinfo);
typedef void (*pfnEngSrc_pfnPlaySoundByName_t)(const char* szSound, float volume);
typedef void (*pfnEngSrc_pfnPlaySoundByNameAtPitch_t)(const char* szSound, float volume, int pitch);
typedef void (*pfnEngSrc_pfnPlaySoundVoiceByName_t)(const char* szSound, float volume, int pitch);
typedef void (*pfnEngSrc_pfnPlaySoundByIndex_t)(int iSound, float volume);
typedef void (*pfnEngSrc_pfnAngleVectors_t)(const float* vecAngles, float* forward, float* right, float* up);
typedef struct client_textmessage_s* (*pfnEngSrc_pfnTextMessageGet_t)(const char* pName);
typedef int (*pfnEngSrc_pfnDrawCharacter_t)(int x, int y, int number, int r, int g, int b);
typedef int (*pfnEngSrc_pfnDrawConsoleString_t)(int x, int y, char* string);
typedef void (*pfnEngSrc_pfnDrawSetTextColor_t)(float r, float g, float b);
typedef void (*pfnEngSrc_pfnDrawConsoleStringLen_t)(const char* string, int* length, int* height);
typedef void (*pfnEngSrc_pfnConsolePrint_t)(const char* string);
typedef void (*pfnEngSrc_pfnCenterPrint_t)(const char* string);
typedef int (*pfnEngSrc_GetWindowCenterX_t)(void);
typedef int (*pfnEngSrc_GetWindowCenterY_t)(void);
typedef void (*pfnEngSrc_GetViewAngles_t)(float*);
typedef void (*pfnEngSrc_SetViewAngles_t)(float*);
typedef int (*pfnEngSrc_GetMaxClients_t)(void);
typedef void (*pfnEngSrc_Cvar_SetValue_t)(const char* cvar, float value);
typedef int (*pfnEngSrc_Cmd_Argc_t)(void);
typedef const char* (*pfnEngSrc_Cmd_Argv_t)(int arg);
typedef void (*pfnEngSrc_Con_Printf_t)(const char* fmt, ...);
typedef void (*pfnEngSrc_Con_DPrintf_t)(const char* fmt, ...);
typedef void (*pfnEngSrc_Con_NPrintf_t)(int pos, const char* fmt, ...);
typedef void (*pfnEngSrc_Con_NXPrintf_t)(struct con_nprint_s* info, const char* fmt, ...);
typedef const char* (*pfnEngSrc_PhysInfo_ValueForKey_t)(const char* key);
typedef const char* (*pfnEngSrc_ServerInfo_ValueForKey_t)(const char* key);
typedef float (*pfnEngSrc_GetClientMaxspeed_t)(void);
typedef int (*pfnEngSrc_CheckParm_t)(const char* parm, const char** ppnext);
typedef void (*pfnEngSrc_Key_Event_t)(int key, int down);
typedef void (*pfnEngSrc_GetMousePosition_t)(int* mx, int* my);
typedef int (*pfnEngSrc_IsNoClipping_t)(void);
typedef struct cl_entity_s* (*pfnEngSrc_GetLocalPlayer_t)(void);
typedef struct cl_entity_s* (*pfnEngSrc_GetViewModel_t)(void);
typedef struct cl_entity_s* (*pfnEngSrc_GetEntityByIndex_t)(int idx);
typedef float (*pfnEngSrc_GetClientTime_t)(void);
typedef void (*pfnEngSrc_V_CalcShake_t)(void);
typedef void (*pfnEngSrc_V_ApplyShake_t)(float* origin, float* angles, float factor);
typedef int (*pfnEngSrc_PM_PointContents_t)(float* point, int* truecontents);
typedef int (*pfnEngSrc_PM_WaterEntity_t)(float* p);
typedef struct pmtrace_s* (*pfnEngSrc_PM_TraceLine_t)(float* start, float* end, int flags, int usehull, int ignore_pe);
typedef struct model_s* (*pfnEngSrc_CL_LoadModel_t)(const char* modelname, int* index);
typedef int (*pfnEngSrc_CL_CreateVisibleEntity_t)(int type, struct cl_entity_s* ent);
typedef const struct model_s* (*pfnEngSrc_GetSpritePointer_t)(HSPRITE hSprite);
typedef void (*pfnEngSrc_pfnPlaySoundByNameAtLocation_t)(const char* szSound, float volume, float* origin);
typedef unsigned short (*pfnEngSrc_pfnPrecacheEvent_t)(int type, const char* psz);
typedef void (*pfnEngSrc_pfnPlaybackEvent_t)(int flags, const struct edict_s* pInvoker, unsigned short eventindex, float delay, const float* origin, const float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
typedef void (*pfnEngSrc_pfnWeaponAnim_t)(int iAnim, int body);
typedef float (*pfnEngSrc_pfnRandomFloat_t)(float flLow, float flHigh);
typedef int32 (*pfnEngSrc_pfnRandomLong_t)(int32 lLow, int32 lHigh);
typedef void (*pfnEngSrc_pfnHookEvent_t)(const char* name, void (*pfnEvent)(struct event_args_s* args));
typedef int (*pfnEngSrc_Con_IsVisible_t)();
typedef const char* (*pfnEngSrc_pfnGetGameDirectory_t)(void);
typedef struct cvar_s* (*pfnEngSrc_pfnGetCvarPointer_t)(const char* szName);
typedef const char* (*pfnEngSrc_Key_LookupBinding_t)(const char* pBinding);
typedef const char* (*pfnEngSrc_pfnGetLevelName_t)(void);
typedef void (*pfnEngSrc_pfnGetScreenFade_t)(struct screenfade_s* fade);
typedef void (*pfnEngSrc_pfnSetScreenFade_t)(struct screenfade_s* fade);
typedef void* (*pfnEngSrc_VGui_GetPanel_t)();
typedef void (*pfnEngSrc_VGui_ViewportPaintBackground_t)(int extents[4]);
typedef byte* (*pfnEngSrc_COM_LoadFile_t)(const char* path, int usehunk, int* pLength);
typedef char* (*pfnEngSrc_COM_ParseFile_t)(const char* data, char* token);
typedef void (*pfnEngSrc_COM_FreeFile_t)(void* buffer);
typedef struct triangleapi_s* pTriAPI;
typedef struct efx_api_s* pEfxAPI;
typedef struct event_api_s* pEventAPI;
typedef struct demo_api_s* pDemoAPI;
typedef struct net_api_s* pNetAPI;
typedef struct IVoiceTweak_s* pVoiceTweak;
typedef int (*pfnEngSrc_IsSpectateOnly_t)(void);
typedef struct model_s* (*pfnEngSrc_LoadMapSprite_t)(const char* filename);
typedef void (*pfnEngSrc_COM_AddAppDirectoryToSearchPath_t)(const char* pszBaseDir, const char* appName);
typedef int (*pfnEngSrc_COM_ExpandFilename_t)(const char* fileName, char* nameOutBuffer, int nameOutBufferSize);
typedef const char* (*pfnEngSrc_PlayerInfo_ValueForKey_t)(int playerNum, const char* key);
typedef void (*pfnEngSrc_PlayerInfo_SetValueForKey_t)(const char* key, const char* value);
typedef qboolean (*pfnEngSrc_GetPlayerUniqueID_t)(int iPlayer, char playerID[16]);
typedef int (*pfnEngSrc_GetTrackerIDForPlayer_t)(int playerSlot);
typedef int (*pfnEngSrc_GetPlayerForTrackerID_t)(int trackerID);
typedef int (*pfnEngSrc_pfnServerCmdUnreliable_t)(char* szCmdString);
typedef void (*pfnEngSrc_GetMousePos_t)(Point* ppt);
typedef void (*pfnEngSrc_SetMousePos_t)(int x, int y);
typedef void (*pfnEngSrc_SetMouseEnable_t)(qboolean fEnable);
typedef struct cvar_s* (*pfnEngSrc_GetFirstCVarPtr_t)();
typedef unsigned int (*pfnEngSrc_GetFirstCmdFunctionHandle_t)();
typedef unsigned int (*pfnEngSrc_GetNextCmdFunctionHandle_t)(unsigned int cmdhandle);
typedef const char* (*pfnEngSrc_GetCmdFunctionName_t)(unsigned int cmdhandle);
typedef float (*pfnEngSrc_GetClientOldTime_t)();
typedef float (*pfnEngSrc_GetServerGravityValue_t)();
typedef struct model_s* (*pfnEngSrc_GetModelByIndex_t)(int index);
typedef void (*pfnEngSrc_pfnSetFilterMode_t)(int mode);
typedef void (*pfnEngSrc_pfnSetFilterColor_t)(float r, float g, float b);
typedef void (*pfnEngSrc_pfnSetFilterBrightness_t)(float brightness);
typedef sequenceEntry_s* (*pfnEngSrc_pfnSequenceGet_t)(const char* fileName, const char* entryName);
typedef void (*pfnEngSrc_pfnSPR_DrawGeneric_t)(int frame, int x, int y, const Rect* prc, int src, int dest, int w, int h);
typedef sentenceEntry_s* (*pfnEngSrc_pfnSequencePickSentence_t)(const char* sentenceName, int pickMethod, int* entryPicked);
// draw a complete string
typedef int (*pfnEngSrc_pfnDrawString_t)(int x, int y, const char* str, int r, int g, int b);
typedef int (*pfnEngSrc_pfnDrawStringReverse_t)(int x, int y, const char* str, int r, int g, int b);
typedef const char* (*pfnEngSrc_LocalPlayerInfo_ValueForKey_t)(const char* key);
typedef int (*pfnEngSrc_pfnVGUI2DrawCharacter_t)(int x, int y, int ch, unsigned int font);
typedef int (*pfnEngSrc_pfnVGUI2DrawCharacterAdd_t)(int x, int y, int ch, int r, int g, int b, unsigned int font);
typedef unsigned int (*pfnEngSrc_COM_GetApproxWavePlayLength)(const char* filename);
typedef void* (*pfnEngSrc_pfnGetCareerUI_t)();
typedef void (*pfnEngSrc_Cvar_Set_t)(const char* cvar, const char* value);
typedef int (*pfnEngSrc_pfnIsPlayingCareerMatch_t)();
typedef double (*pfnEngSrc_GetAbsoluteTime_t)(void);
typedef void (*pfnEngSrc_pfnProcessTutorMessageDecayBuffer_t)(int* buffer, int bufferLength);
typedef void (*pfnEngSrc_pfnConstructTutorMessageDecayBuffer_t)(int* buffer, int bufferLength);
typedef void (*pfnEngSrc_pfnResetTutorMessageDecayData_t)();
typedef void (*pfnEngSrc_pfnFillRGBABlend_t)(int x, int y, int width, int height, int r, int g, int b, int a);
typedef int (*pfnEngSrc_pfnGetAppID_t)(void);
typedef cmdalias_t* (*pfnEngSrc_pfnGetAliases_t)(void);
typedef void (*pfnEngSrc_pfnVguiWrap2_GetMouseDelta_t)(int* x, int* y);
typedef int (*pfnEngSrc_pfnFilteredClientCmd_t)(const char* szCmdString);

// Pointers to the exported engine functions themselves
typedef struct cl_enginefuncs_s
{
	pfnEngSrc_pfnSPR_Load_t pfnSPR_Load;
	pfnEngSrc_pfnSPR_Frames_t pfnSPR_Frames;
	pfnEngSrc_pfnSPR_Height_t pfnSPR_Height;
	pfnEngSrc_pfnSPR_Width_t pfnSPR_Width;
	pfnEngSrc_pfnSPR_Set_t pfnSPR_Set;
	pfnEngSrc_pfnSPR_Draw_t pfnSPR_Draw;
	pfnEngSrc_pfnSPR_DrawHoles_t pfnSPR_DrawHoles;
	pfnEngSrc_pfnSPR_DrawAdditive_t pfnSPR_DrawAdditive;
	pfnEngSrc_pfnSPR_EnableScissor_t pfnSPR_EnableScissor;
	pfnEngSrc_pfnSPR_DisableScissor_t pfnSPR_DisableScissor;
	pfnEngSrc_pfnSPR_GetList_t pfnSPR_GetList;
	pfnEngSrc_pfnFillRGBA_t pfnFillRGBA;
	pfnEngSrc_pfnGetScreenInfo_t pfnGetScreenInfo;
	pfnEngSrc_pfnSetCrosshair_t pfnSetCrosshair;
	pfnEngSrc_pfnRegisterVariable_t pfnRegisterVariable;
	pfnEngSrc_pfnGetCvarFloat_t pfnGetCvarFloat;
	pfnEngSrc_pfnGetCvarString_t pfnGetCvarString;
	pfnEngSrc_pfnAddCommand_t pfnAddCommand;
	pfnEngSrc_pfnHookUserMsg_t pfnHookUserMsg;
	pfnEngSrc_pfnServerCmd_t pfnServerCmd;
	pfnEngSrc_pfnClientCmd_t pfnClientCmd;
	pfnEngSrc_pfnGetPlayerInfo_t pfnGetPlayerInfo;
	pfnEngSrc_pfnPlaySoundByName_t pfnPlaySoundByName;
	pfnEngSrc_pfnPlaySoundByIndex_t pfnPlaySoundByIndex;
	pfnEngSrc_pfnAngleVectors_t pfnAngleVectors;
	pfnEngSrc_pfnTextMessageGet_t pfnTextMessageGet;
	pfnEngSrc_pfnDrawCharacter_t pfnDrawCharacter;
	pfnEngSrc_pfnDrawConsoleString_t pfnDrawConsoleString;
	pfnEngSrc_pfnDrawSetTextColor_t pfnDrawSetTextColor;
	pfnEngSrc_pfnDrawConsoleStringLen_t pfnDrawConsoleStringLen;
	pfnEngSrc_pfnConsolePrint_t pfnConsolePrint;
	pfnEngSrc_pfnCenterPrint_t pfnCenterPrint;
	pfnEngSrc_GetWindowCenterX_t GetWindowCenterX;
	pfnEngSrc_GetWindowCenterY_t GetWindowCenterY;
	pfnEngSrc_GetViewAngles_t GetViewAngles;
	pfnEngSrc_SetViewAngles_t SetViewAngles;
	pfnEngSrc_GetMaxClients_t GetMaxClients;
	pfnEngSrc_Cvar_SetValue_t Cvar_SetValue;
	pfnEngSrc_Cmd_Argc_t Cmd_Argc;
	pfnEngSrc_Cmd_Argv_t Cmd_Argv;
	pfnEngSrc_Con_Printf_t Con_Printf;
	pfnEngSrc_Con_DPrintf_t Con_DPrintf;
	pfnEngSrc_Con_NPrintf_t Con_NPrintf;
	pfnEngSrc_Con_NXPrintf_t Con_NXPrintf;
	pfnEngSrc_PhysInfo_ValueForKey_t PhysInfo_ValueForKey;
	pfnEngSrc_ServerInfo_ValueForKey_t ServerInfo_ValueForKey;
	pfnEngSrc_GetClientMaxspeed_t GetClientMaxspeed;
	pfnEngSrc_CheckParm_t CheckParm;
	pfnEngSrc_Key_Event_t Key_Event;
	pfnEngSrc_GetMousePosition_t GetMousePosition;
	pfnEngSrc_IsNoClipping_t IsNoClipping;
	pfnEngSrc_GetLocalPlayer_t GetLocalPlayer;
	pfnEngSrc_GetViewModel_t GetViewModel;
	pfnEngSrc_GetEntityByIndex_t GetEntityByIndex;
	pfnEngSrc_GetClientTime_t GetClientTime;
	pfnEngSrc_V_CalcShake_t V_CalcShake;
	pfnEngSrc_V_ApplyShake_t V_ApplyShake;
	pfnEngSrc_PM_PointContents_t PM_PointContents;
	pfnEngSrc_PM_WaterEntity_t PM_WaterEntity;
	pfnEngSrc_PM_TraceLine_t PM_TraceLine;
	pfnEngSrc_CL_LoadModel_t CL_LoadModel;
	pfnEngSrc_CL_CreateVisibleEntity_t CL_CreateVisibleEntity;
	pfnEngSrc_GetSpritePointer_t GetSpritePointer;
	pfnEngSrc_pfnPlaySoundByNameAtLocation_t pfnPlaySoundByNameAtLocation;
	pfnEngSrc_pfnPrecacheEvent_t pfnPrecacheEvent;
	pfnEngSrc_pfnPlaybackEvent_t pfnPlaybackEvent;
	pfnEngSrc_pfnWeaponAnim_t pfnWeaponAnim;
	pfnEngSrc_pfnRandomFloat_t pfnRandomFloat;
	pfnEngSrc_pfnRandomLong_t pfnRandomLong;
	pfnEngSrc_pfnHookEvent_t pfnHookEvent;
	pfnEngSrc_Con_IsVisible_t Con_IsVisible;
	pfnEngSrc_pfnGetGameDirectory_t pfnGetGameDirectory;
	pfnEngSrc_pfnGetCvarPointer_t pfnGetCvarPointer;
	pfnEngSrc_Key_LookupBinding_t Key_LookupBinding;
	pfnEngSrc_pfnGetLevelName_t pfnGetLevelName;
	pfnEngSrc_pfnGetScreenFade_t pfnGetScreenFade;
	pfnEngSrc_pfnSetScreenFade_t pfnSetScreenFade;
	pfnEngSrc_VGui_GetPanel_t VGui_GetPanel;
	pfnEngSrc_VGui_ViewportPaintBackground_t VGui_ViewportPaintBackground;
	pfnEngSrc_COM_LoadFile_t COM_LoadFile;
	pfnEngSrc_COM_ParseFile_t COM_ParseFile;
	pfnEngSrc_COM_FreeFile_t COM_FreeFile;
	struct triangleapi_s* pTriAPI;
	struct efx_api_s* pEfxAPI;
	struct event_api_s* pEventAPI;
	struct demo_api_s* pDemoAPI;
	struct net_api_s* pNetAPI;
	struct IVoiceTweak_s* pVoiceTweak;
	pfnEngSrc_IsSpectateOnly_t IsSpectateOnly;
	pfnEngSrc_LoadMapSprite_t LoadMapSprite;
	pfnEngSrc_COM_AddAppDirectoryToSearchPath_t COM_AddAppDirectoryToSearchPath;
	pfnEngSrc_COM_ExpandFilename_t COM_ExpandFilename;
	pfnEngSrc_PlayerInfo_ValueForKey_t PlayerInfo_ValueForKey;
	pfnEngSrc_PlayerInfo_SetValueForKey_t PlayerInfo_SetValueForKey;
	pfnEngSrc_GetPlayerUniqueID_t GetPlayerUniqueID;
	pfnEngSrc_GetTrackerIDForPlayer_t GetTrackerIDForPlayer;
	pfnEngSrc_GetPlayerForTrackerID_t GetPlayerForTrackerID;
	pfnEngSrc_pfnServerCmdUnreliable_t pfnServerCmdUnreliable;
	pfnEngSrc_GetMousePos_t pfnGetMousePos;
	pfnEngSrc_SetMousePos_t pfnSetMousePos;
	pfnEngSrc_SetMouseEnable_t pfnSetMouseEnable;
	pfnEngSrc_GetFirstCVarPtr_t GetFirstCvarPtr;
	pfnEngSrc_GetFirstCmdFunctionHandle_t GetFirstCmdFunctionHandle;
	pfnEngSrc_GetNextCmdFunctionHandle_t GetNextCmdFunctionHandle;
	pfnEngSrc_GetCmdFunctionName_t GetCmdFunctionName;
	pfnEngSrc_GetClientOldTime_t hudGetClientOldTime;
	pfnEngSrc_GetServerGravityValue_t hudGetServerGravityValue;
	pfnEngSrc_GetModelByIndex_t hudGetModelByIndex;
	pfnEngSrc_pfnSetFilterMode_t pfnSetFilterMode;
	pfnEngSrc_pfnSetFilterColor_t pfnSetFilterColor;
	pfnEngSrc_pfnSetFilterBrightness_t pfnSetFilterBrightness;
	pfnEngSrc_pfnSequenceGet_t pfnSequenceGet;
	pfnEngSrc_pfnSPR_DrawGeneric_t pfnSPR_DrawGeneric;
	pfnEngSrc_pfnSequencePickSentence_t pfnSequencePickSentence;
	pfnEngSrc_pfnDrawString_t pfnDrawString;
	pfnEngSrc_pfnDrawStringReverse_t pfnDrawStringReverse;
	pfnEngSrc_LocalPlayerInfo_ValueForKey_t LocalPlayerInfo_ValueForKey;
	pfnEngSrc_pfnVGUI2DrawCharacter_t pfnVGUI2DrawCharacter;
	pfnEngSrc_pfnVGUI2DrawCharacterAdd_t pfnVGUI2DrawCharacterAdd;
	pfnEngSrc_COM_GetApproxWavePlayLength COM_GetApproxWavePlayLength;
	pfnEngSrc_pfnGetCareerUI_t pfnGetCareerUI;
	pfnEngSrc_Cvar_Set_t Cvar_Set;
	pfnEngSrc_pfnIsPlayingCareerMatch_t pfnIsCareerMatch;
	pfnEngSrc_pfnPlaySoundVoiceByName_t pfnPlaySoundVoiceByName;
	pfnEngSrc_pfnPrimeMusicStream_t pfnPrimeMusicStream;
	pfnEngSrc_GetAbsoluteTime_t GetAbsoluteTime;
	pfnEngSrc_pfnProcessTutorMessageDecayBuffer_t pfnProcessTutorMessageDecayBuffer;
	pfnEngSrc_pfnConstructTutorMessageDecayBuffer_t pfnConstructTutorMessageDecayBuffer;
	pfnEngSrc_pfnResetTutorMessageDecayData_t pfnResetTutorMessageDecayData;
	pfnEngSrc_pfnPlaySoundByNameAtPitch_t pfnPlaySoundByNameAtPitch;
	pfnEngSrc_pfnFillRGBABlend_t pfnFillRGBABlend;
	pfnEngSrc_pfnGetAppID_t pfnGetAppID;
	pfnEngSrc_pfnGetAliases_t pfnGetAliasList;
	pfnEngSrc_pfnVguiWrap2_GetMouseDelta_t pfnVguiWrap2_GetMouseDelta;
	pfnEngSrc_pfnFilteredClientCmd_t pfnFilteredClientCmd;
} cl_enginefunc_t;
