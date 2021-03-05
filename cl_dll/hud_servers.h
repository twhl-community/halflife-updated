//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined( HUD_SERVERSH )
#define HUD_SERVERSH
#pragma once

#define NET_CALLBACK /* */

// Dispatchers
void NET_CALLBACK ListResponse( struct net_response_s *response );
void NET_CALLBACK ServerResponse( struct net_response_s *response );
void NET_CALLBACK PingResponse( struct net_response_s *response );
void NET_CALLBACK RulesResponse( struct net_response_s *response );
void NET_CALLBACK PlayersResponse( struct net_response_s *response );

void ServersInit();
void ServersShutdown();
void ServersThink( double time );
void ServersCancel();

// Get list and get server info from each
void ServersList();

// Query for IP / IPX LAN servers
void BroadcastServersList( int clearpending );

void ServerPing( int server );
void ServerRules( int server );
void ServerPlayers( int server );

int	ServersGetCount();
const char *ServersGetInfo( int server );
int	ServersIsQuerying();
void SortServers( const char *fieldname );

#endif // HUD_SERVERSH