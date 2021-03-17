//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// eventscripts.h
#if !defined ( EVENTSCRIPTSH )
#define EVENTSCRIPTSH

// Some of these are HL/TFC specific?
void EV_EjectBrass( float *origin, float *velocity, float rotation, int model, int soundtype );
void EV_GetGunPosition( struct event_args_s *args, float *pos, float *origin );
void EV_GetDefaultShellInfo( struct event_args_s *args, float *origin, float *velocity, float *ShellVelocity, float *ShellOrigin, float *forward, float *right, float *up, float forwardScale, float upScale, float rightScale );
qboolean EV_IsLocal( int idx );
qboolean EV_IsPlayer( int idx );
void EV_CreateTracer( float *start, float *end );

struct cl_entity_s *GetEntity( int idx );
struct cl_entity_s *GetViewEntity();
void EV_MuzzleFlash();

#endif // EVENTSCRIPTSH
