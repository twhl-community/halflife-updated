#pragma once

#include "Platform.h"

extern "C" 
{
	// From hl_weapons
	void DLLEXPORT HUD_PostRunCmd( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );

	// From cdll_int
	int DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );
	int DLLEXPORT HUD_VidInit( void );
	void DLLEXPORT HUD_Init( void );
	int DLLEXPORT HUD_Redraw( float flTime, int intermission );
	int DLLEXPORT HUD_UpdateClientData( client_data_t *cdata, float flTime );
	void DLLEXPORT HUD_Reset ( void );
	void DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server );
	void DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove );
	char DLLEXPORT HUD_PlayerMoveTexture( char *name );
	int DLLEXPORT HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
	int DLLEXPORT HUD_GetHullBounds( int hullnumber, float *mins, float *maxs );
	void DLLEXPORT HUD_Frame( double time );
	void DLLEXPORT HUD_VoiceStatus(int entindex, qboolean bTalking);
	void DLLEXPORT HUD_DirectorMessage( int iSize, void *pbuf );
	void DLLEXPORT HUD_ChatInputPosition( int *x, int *y );

	// From demo
	void DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer );

	// From entity
	int DLLEXPORT HUD_AddEntity( int type, struct cl_entity_s *ent, const char *modelname );
	void DLLEXPORT HUD_CreateEntities( void );
	void DLLEXPORT HUD_StudioEvent( const struct mstudioevent_s *event, const struct cl_entity_s *entity );
	void DLLEXPORT HUD_TxferLocalOverrides( struct entity_state_s *state, const struct clientdata_s *client );
	void DLLEXPORT HUD_ProcessPlayerState( struct entity_state_s *dst, const struct entity_state_s *src );
	void DLLEXPORT HUD_TxferPredictionData ( struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd );
	void DLLEXPORT HUD_TempEntUpdate( double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int ( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void ( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
	struct cl_entity_s DLLEXPORT *HUD_GetUserEntity( int index );

	// From in_camera
	void DLLEXPORT CAM_Think( void );
	int DLLEXPORT CL_IsThirdPerson( void );
	void DLLEXPORT CL_CameraOffset( float *ofs );

	// From input
	struct kbutton_s DLLEXPORT *KB_Find( const char *name );
	void DLLEXPORT CL_CreateMove ( float frametime, struct usercmd_s *cmd, int active );
	void DLLEXPORT HUD_Shutdown( void );
	int DLLEXPORT HUD_Key_Event( int eventcode, int keynum, const char *pszCurrentBinding );

	// From inputw32
	void DLLEXPORT IN_ActivateMouse( void );
	void DLLEXPORT IN_DeactivateMouse( void );
	void DLLEXPORT IN_MouseEvent (int mstate);
	void DLLEXPORT IN_Accumulate (void);
	void DLLEXPORT IN_ClearStates (void);

	// From tri
	void DLLEXPORT HUD_DrawNormalTriangles( void );
	void DLLEXPORT HUD_DrawTransparentTriangles( void );

	// From view
	void	DLLEXPORT V_CalcRefdef( struct ref_params_s *pparams );

	// From GameStudioModelRenderer
	int DLLEXPORT HUD_GetStudioModelInterface( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio );
}
