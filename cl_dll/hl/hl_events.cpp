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
#include "../hud.h"
#include "../cl_util.h"
#include "event_api.h"
#include "pmtrace.h"
#include "ev_hldm.h"
#include "filesystem_utils.h"

static void Game_HookEvent(const char* const szEventName, void (*const pfnEvent)(event_args_t*))
{
	UTIL_CheckEventScript(szEventName);

	gEngfuncs.pfnHookEvent(szEventName, pfnEvent);
}

/*
======================
Game_HookEvents

Associate script file name with callback functions. Note that the format is
 always the same.  Of course, a clever mod team could actually embed parameters, behavior
 into the actual .sc files and create a .sc file parser and hook their functionality through
 that.. i.e., a scripting system.

That was what we were going to do, but we ran out of time...oh well.
======================
*/
void Game_HookEvents()
{
	Game_HookEvent("events/glock1.sc", EV_FireGlock1);
	Game_HookEvent("events/glock2.sc", EV_FireGlock2);
	Game_HookEvent("events/shotgun1.sc", EV_FireShotGunSingle);
	Game_HookEvent("events/shotgun2.sc", EV_FireShotGunDouble);
	Game_HookEvent("events/mp5.sc", EV_FireMP5);
	Game_HookEvent("events/mp52.sc", EV_FireMP52);
	Game_HookEvent("events/python.sc", EV_FirePython);
	Game_HookEvent("events/gauss.sc", EV_FireGauss);
	Game_HookEvent("events/gaussspin.sc", EV_SpinGauss);
	Game_HookEvent("events/train.sc", EV_TrainPitchAdjust);
	Game_HookEvent("events/vehicle.sc", EV_VehiclePitchAdjust);
	Game_HookEvent("events/crowbar.sc", EV_Crowbar);
	Game_HookEvent("events/crossbow1.sc", EV_FireCrossbow);
	Game_HookEvent("events/crossbow2.sc", EV_FireCrossbow2);
	Game_HookEvent("events/rpg.sc", EV_FireRpg);
	Game_HookEvent("events/egon_fire.sc", EV_EgonFire);
	Game_HookEvent("events/egon_stop.sc", EV_EgonStop);
	Game_HookEvent("events/firehornet.sc", EV_HornetGunFire);
	Game_HookEvent("events/tripfire.sc", EV_TripmineFire);
	Game_HookEvent("events/snarkfire.sc", EV_SnarkFire);
}
