# Half-Life Updated changelog

## Changes in V1.1.1

> Note: this update has not been released yet.

### Bug fixes

* Added missing client side event for `func_vehicle` sounds
* Link Linux binaries with `-Wl` and `--no-undefined` flags to avoid situations where something was referenced but wasn't added in the build (Thanks a1batross)
* Prevented game_zone_player from transitioning across levels to fix Mod_NumForName: not found issue [#241](https://github.com/twhl-community/halflife-updated/pull/241) (Thanks FreeSlave)
* Fixed null dereference in game_score [#246](https://github.com/twhl-community/halflife-updated/pull/246) (Thanks FreeSlave)
* Fixed null dereference of m_rawinput and mouse issues on Linux [#251](https://github.com/twhl-community/halflife-updated/pull/251) (Thanks a1batross)

### Features

* [HL25] Backported changes from October 2nd 2024 update

## Changes in V1.1.0

### Bug Fixes

* Fixed potential buffer overflows in text localization (Thanks OMAM)
* Reset frame to 0 when grenade bounces [#238](https://github.com/twhl-community/halflife-updated/issues/238) (Thanks FreeSlave)
* Fixed weapon events not treating pushable objects as BSP models [#220](https://github.com/twhl-community/halflife-updated/pull/220) (Thanks Toodles2You)
* Fixed crowbar applying breakable glass decals to unbreakable pushable objects [#219](https://github.com/twhl-community/halflife-updated/pull/219) (Thanks Toodles2You)
* [HL25] Fixed client/server view origin/angles sync and view bob affectation
* [HL25] Fixed crowbar full swing logic (already fixed but reverted to use HL25's version for consistency)
* [HL25] Fixed `func_pushable` framerate/input handling
* [HL25] Fixed geiger counter sound at range 800 units and higher
* [HL25] Fixed "ghost shots" syndrome (backported from Counter-Strike)
* [HL25] Fixed Gluon gun's flare sprite (already fixed but reverted to use HL25's version for consistency)
* [HL25] Fixed how flesh sounds are handled on entities
* [HL25] Fixed monsters turning speed being dependant on framerate (already fixed but reverted to use HL25's version for consistency)
* [HL25] Fixed potential null pointer on field name when restoring (loading saved game) entities
* [HL25] Fixed some player movement cases where client can be stuck
* [HL25] Fixed some joystick and mouse sensitivity issues
* [HL25] Fixed some unsafe string operations
* [HL25] Fixed some out of bounds indexes
* [HL25] Fixed `trigger_hurt` healing dead players in multiplayer
* [HL25] Fixed incorrect touch function reference in the unused trip beam entity
* [HL25] Renamed extra CVAR flags including their description to HL25's SDK counterpart for consistency
* [HL25] Reworked how multiplayer corpses are handled to fix various issues

### Features

* Removed some leftovers related to the Mac platform
* [HL25] Added `EngineFilteredClientCmd` macro on the client project
* [HL25] Added `_sv_override_scientist_mdl` CVAR functionality
* [HL25] Added `sv_allow_autoaim` CVAR functionality
* [HL25] Added 1280 and 2560 HUD resolutions support (including other changes to the HUD)
* [HL25] Added changes to 9mm AR's default magazine capacity (25 if singleplayer, 50 if multiplayer)
* [HL25] Added changes to multiplayer spawn selection logic
* [HL25] Added changes to the Gauss gun's charging mechanic
* [HL25] Added changes to the hand grenade throwing arc
* [HL25] Added changes to the hive hand's (hornet gun) auto-switch weight and recharge time (faster if multiplayer)
* [HL25] Added changes to the RPG's empty sound, rocket tracking and detonation if touching the sky
* [HL25] Added changes to the sentences system (moved from `CBaseMonster` to `CBaseToggle`)
* [HL25] Added changes to the snarks throw
* [HL25] Added comments about the `netadr_s` structure
* [HL25] Added "busters" multiplayer game mode
* [HL25] Added `END3` message to end the game functionality
* [HL25] Added `func_vehicle` entity
* [HL25] Added "modern" satchel charge/radio controls (those who want the "legacy" behavior can comment the `MODERN_SATCHEL_CONTROLS` define at the beginning of `dlls/satchel.cpp`)
* [HL25] Added multiplayer specific limit of live satchels per player
* [HL25] Added new method to filesystem interface
* [HL25] Added Python's damage buff in multiplayer (from 40 to 50)
* [HL25] Added Steam Rich Presence commands calls (disabled due to being reserved to mods hosted on Steam, see `STEAM_RICH_PRESENCE` define)
* [HL25] Increased max. sentences count from 1536 to 2048
* [HL25] Made players non-solid upon dying in multiplayer
* [HL25] Made dropping active weapon to be reloaded before actually dropping it
* [HL25] Server will now execute `spserver.cfg` when initializing singleplayer game rules
* [HL25] Updated procinfo, SDL2, Steam types and VGUI 1 dependencies


## Changes in V1.0.0

### Bug fixes

* Disabled GCC optimization that prevents mod dlls from unloading after engine calls dlclose
* Fixed third party libraries possibly not being linked to when building Linux server dll (Thanks a1batross)

### Features

* Mods made with this SDK will now shut down if they detect they are being run from a Valve game directory (e.g. by placing the dlls in `Half-Life/valve/cl_dlls` and `Half-Life/valve/dlls`). This is not supported and puts users at risk of being VAC banned. Run mods from their intended location only

## Changes in V1.0.0 Release Candidate 003

### Bug fixes

* Fixed save game system not saving arrays of EHANDLEs if the first half of the array contains null handles (mainly affected Nihilanth's spheres) [#224](https://github.com/twhl-community/halflife-updated/issues/224) (Thanks Ronin4862)
* Fixed player gaining health when drowning with god mode enabled and recovering health after surfacing (Thanks malortie)
* Fixed human grunts continuing to fire for a few seconds after killing the last enemy in an area [Opposing Force Updated #100](https://github.com/twhl-community/halflife-op4-updated/issues/100) (Thanks Ronin4862 and malortie)
* Fixed crash when +USEing NPCs that have just exited a scripted sequence (Thanks malortie)
* Fixed talk monsters resetting other talk monsters' dying schedule if they are both killed at the same time (Thanks FreeSlave)
* Fixed RPG laser turning on when reloading immediately after equipping the weapon
* Reverted weapon selection using weapon IDs to prevent the game from malfunctioning when delta.lst is missing

### Features

* Added sv_load_all_maps & sv_stop_loading_all_maps to help automate node graph generation

## Changes in V1.0.0 Release Candidate 002

### Bug fixes

* Fixed hand grenade animations not playing correctly [#209](https://github.com/twhl-community/halflife-updated/pull/209) (Thanks Toodles2You)
* Fixed out of bounds access in studiomodel renderer bone setup code (halflife issue [#3360](https://github.com/ValveSoftware/halflife/issues/3360))
* Fixed mouse cursor being invisible in VGUI1 menus when raw input is enabled [#211](https://github.com/twhl-community/halflife-updated/issues/211) (Thanks RykahKnight)
* Mouse movement in the main menu no longer affects in-game view angles when not using raw input
* Fixed RPG being flagged as unusable while a rocket is loaded [#213](https://github.com/twhl-community/halflife-updated/pull/213) (Thanks Toodles2You)
* Have clients select weapons by ID, rather than by name [#217](https://github.com/twhl-community/halflife-updated/pull/217) (Thanks Toodles2You)
* Copy delta.lst when building client or server to ensure mods have correct delta.lst file (Thanks P38TaKjYzY)
* Reset current history icon slot when resetting item history HUD [#223](https://github.com/twhl-community/halflife-updated/issues/223) (Thanks malortie)
* Fixed Gauss gun dealing full damage when saving and loading right after starting a charged shot (Thanks Oxofemple.)
* Prevent breakables from spawning multiple items when destroyed by gunfire and explosives at the same time (Thanks Oxofemple.)

### Features

* Added cvar `sv_allowbunnyhopping` to control whether the bunny hopping limiter is enabled (halflife issue [#11](https://github.com/ValveSoftware/halflife/issues/11))

## Changes in V1.0.0 Release Candidate 001

### Bug fixes

* Fixed tripmines not detonating if placed on breakable and a save game is loaded [#192](https://github.com/twhl-community/halflife-updated/issues/192) (Thanks forklift.mdl)
* Fixed func_friction not working properly in multiplayer (halflife issue [#1542](https://github.com/ValveSoftware/halflife/issues/1542)) (Thanks L453rh4wk)
* Fixed spray logo using wrong decal after save game load when not using custom spray [#193](https://github.com/twhl-community/halflife-updated/issues/193) (Thanks Ronin4862)
* Fixed ammo pickup sound playing when picking up a weapon for the first time (bug introduced by [#153](https://github.com/twhl-community/halflife-updated/issues/153) in Beta 12)
* Fixed Gauss gun sometimes settting player uranium ammo to -1 (halflife issue [#3343](https://github.com/ValveSoftware/halflife/issues/3343))
* Fixed pistol not playing empty sound when using secondary attack
* Fixed user interface coordinates and sizes being incorrectly adjusted for resolution (halflife issue [#3344](https://github.com/ValveSoftware/halflife/issues/3344))
* Fixed player weapons still receiving input when starting to use a func_tank (halflife issue [#3345](https://github.com/ValveSoftware/halflife/issues/3345)) (Thanks Oxofemple.)
* Fixed alien slave beams staying forever if they exist during a level change (halflife issue [#3104](https://github.com/ValveSoftware/halflife/issues/3104))
* Fixed cycler_wreckage storing time value in int instead of float
* Fixed limit in world weapons (e.g. Hand Grenade) respawning at wrong time if server is near edict limit
* Fixed shotgun starting idle animations too quickly after exhausting all ammo using primary attack [#195](https://github.com/twhl-community/halflife-updated/issues/195) (Thanks Ronin4862)
* Fixed RPG not playing empty sound when attempting to fire with no ammo left [#196](https://github.com/twhl-community/halflife-updated/issues/196) (Thanks Ronin4862)
* Fixed Human Grunts dropping weapons again if the game is saved and loaded while the grunt is dying (Thanks Oxofemple.)
* Added missing monster state name to ReportAIState (halflife issue [#3220](https://github.com/ValveSoftware/halflife/issues/3220)) (Thanks Shepard)
* Fixed mouse movement during map load affecting initial view angles
* Fixed being able to break scripted_sequence by +using friendly NPCs to make them follow player [#200](https://github.com/twhl-community/halflife-updated/issues/200) (Thanks Oxofemple. for reporting this and FreeSlave for finding the solution)
* Fixed potential incorrect facing in scripted sequence (Thanks FreeSlave)
* Made the Linux version link statically to the C++ runtime to help avoid problems when running mods on older systems (Thanks a1ba and FreeSlave)
* Fixed Egon not stopping its attack animation if the attack button is held down and ammo runs out (Thanks the man)
* Fixed scientists crashing when speaking fear dialogue when enemy has been removed
* Disabled fall think function for weapons when the player picks it up to prevent possible double-pickup which removes the weapon and crashes the game
* Disabled jump sounds while player is frozen (e.g. trigger_camera, trigger_playerfreeze)
* Fixed node graph code incorrectly flagging node graphs as out of date if an outdated graph exists in a search path other than the mod directory (e.g. a graph in `halflife_updated_addon/map/graphs`)

## Changes in V1.0.0 Beta 014

### Bug fixes

* Correctly save and restore controller and blending variables [#175](https://github.com/twhl-community/halflife-updated/issues/175) (Thanks vasiavasiavasia95)
* Set activity before SetYawSpeed so it has the right activity [#176](https://github.com/twhl-community/halflife-updated/issues/176) (Thanks [FreeSlave](https://github.com/FreeSlave) and vasiavasiavasia95)
* Fixed animation code accessing invalid sequence descriptor (halflife issue [#3308](https://github.com/ValveSoftware/halflife/issues/3308)) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Fixed weapon bits being incorrectly set and checked in HasAnyWeapons [#178](https://github.com/twhl-community/halflife-updated/issues/178) (Thanks jay!)
* Fixed projects with spaces in paths failing to execute post build step [#179](https://github.com/twhl-community/halflife-updated/issues/179) (Thanks Shepard)
* Fixed NPCs not being able to speak scripted sentences while in scripted death [#180](https://github.com/twhl-community/halflife-updated/issues/180) (Thanks λλλλλλ)
* Removed unnecessary semicolons (Thanks Shepard)
* Updated source file encoding to UTF-8 [#183](https://github.com/twhl-community/halflife-updated/issues/183) (Thanks anchurcn)
* Renamed `CWorld::Instance` to `CWorld::World` to avoid conflicting with `CBaseEntity::Instance` function name
* Added `-flifetime-dse=1` flag to Linux Makefile to disable compiler optimization that removed entity memory zero-initialization, resulting in the game crashing when any entity touches the world [#187](https://github.com/twhl-community/halflife-updated/issues/187) (Thanks FreeSlave)
* Fixed game_player_equip crashing when given a null activator [#189](https://github.com/twhl-community/halflife-updated/issues/189)
* Fixed Hornet gun recharging to full ammo after loading a save game [#190](https://github.com/twhl-community/halflife-updated/issues/190)
* Fixed explosives that impact the underside of a brush dealing damage to entities on the other side of that brush (halflife issue [#3244](https://github.com/ValveSoftware/halflife/issues/3244))
* Fixed entities with an index greater than 2047 corrupting the client's heap if sent over the network [#191](https://github.com/twhl-community/halflife-updated/issues/191)

### New features

* Save and restore game_player_equip [#188](https://github.com/twhl-community/halflife-updated/issues/188)
* Moved IsFacing function from barney.cpp to h_ai.cpp to help prevent linker errors when copy pasting source file
* When using `impulse 107` to get the name of a texture the texture type (as used in `materials.txt`) will also be printed
* Made `PM_FindTextureType` const correct
* Added `WRITE_FLOAT` function corresponding to the client's `READ_FLOAT` function
* Set maximum edicts to 2048 in liblist.gam [#181](https://github.com/twhl-community/halflife-updated/issues/181)

### Project changes

* Added `delta.lst` to the game installation again (was accidentally removed in the previous beta)
* Added game icons to the game installation

## Changes in V1.0.0 Beta 013

### Bug fixes

* Fixed Room DSP effects not always activating [#164](https://github.com/twhl-community/halflife-updated/issues/164) (Thanks [Ronin4862](https://github.com/Ronin4862))
* Try to unstuck player after level transition if stuck in the world [#165](https://github.com/twhl-community/halflife-updated/issues/165) (Thanks [Ronin4862](https://github.com/Ronin4862))
* Fixed MP5 not creating bullet decals half the time (halflife issue [#383](https://github.com/ValveSoftware/halflife/issues/383) and Updated issue [#163](https://github.com/twhl-community/halflife-updated/issues/163)) (Thanks [Ronin4862](https://github.com/Ronin4862) and [Matthaiks](https://github.com/Matthaiks))
* Fixed Particle man removing more particles than expected [#169](https://github.com/twhl-community/halflife-updated/issues/169) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Fixed domain errors in func_tank (halflife issue [#3303](https://github.com/ValveSoftware/halflife/issues/3303)) (Thanks pat and [a1batross](https://github.com/a1batross))
* Fixed crash while parsing command menu with unsupported custom button (halflife issue [#1730](https://github.com/ValveSoftware/halflife/issues/1730))
* Fixed NPCs being able to speak scripted_sentences while dying [#171](https://github.com/twhl-community/halflife-updated/issues/171)
* Fixed tripmines blowing up after loading save game in c1a3d [#172](https://github.com/twhl-community/halflife-updated/issues/172) (Thanks λλλλλλ)
* Fixed game crashing when triggering certain entities on an empty dedicated server with maxplayers 1 (halflife issue [#3307](https://github.com/ValveSoftware/halflife/issues/3307))
    * Access world through global
    * Access local player through helper function
    * Removed some obsolete utility functions

### Project changes

* Cleaned up the Linux makefiles to remove obsolete logic and simplify the compilation process
* Removed use of obsolete register keyword (caused compiler errors when using Clang++)
* Added support for overriding the compiler used when using the Linux makefiles
* Fixed the Linux makefiles not working when using Clang++ due to missing `-mno-sse` compiler flag
* Added particle man sources to the object list in Linux client Makefile [#168](https://github.com/twhl-community/halflife-updated/pull/168) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Added game icons for program icon and Steam library icon
* Use post build event to copy dlls instead of using output directory [#167](https://github.com/twhl-community/halflife-updated/issues/167)

> **Note**: you will need to clean solution and rebuild to remove remaining references to the old output directory.

### Git repository changes

* Added Github Actions CI configuration file based on Half-Life Unified SDK's version. Continuous Integration provides pre-built binaries for Windows and Linux. (Thanks to [JoelTroch](https://github.com/JoelTroch) for providing the original file and providing the basis for this file)

### Github wiki changes

* Explicitly specified the scope of the projects
* Added links to TWHL website, Discord server, SDK setup tutorial and Half-Life Updated status thread

## Changes in V1.0.0 Beta 012

### Bug fixes

* Fixed func_button "Sparks" spawnflag using wrong origin (halflife issue [#3269](https://github.com/ValveSoftware/halflife/issues/3269)) (Thanks Descen)
* Fixed func_rot_button "X Axis" spawnflag also enabling func_button "Sparks" behavior (halflife issue [#3270](https://github.com/ValveSoftware/halflife/issues/3270)) (Thanks Descen)
* Fixed second case of incorrect timebase for button sparks (halflife issue [#1681](https://github.com/ValveSoftware/halflife/issues/1681))
* Fixed env_spark transitioning to other maps when it shouldn't (halflife issue [#3280](https://github.com/ValveSoftware/halflife/issues/3280)) (Thanks Ronin4862)
* Fixed ServerActivate checking for player entities incorrectly (halflife [#3282](https://github.com/ValveSoftware/halflife/issues/3282)) (Thanks vasiavasiavasia95)
* Fixed PM_TraceModel possibly returning uninitialized trace (halflife [#3283](https://github.com/ValveSoftware/halflife/issues/3283)) (Thanks [a1batross](https://github.com/a1batross))
* Print error message if trigger_changelevel points to itself
* Fixed ichthyosaur restarting death sequence on save game load [#152](https://github.com/twhl-community/halflife-updated/issues/152) (Thanks [Ronin4862](https://github.com/Ronin4862))
* Reset server's client FOV value so loading save games restores FOV correctly with weapon prediction disabled (halflife issue [#3044](https://github.com/ValveSoftware/halflife/issues/3044)) (Thanks [Ronin4862](https://github.com/Ronin4862))
* Commented out fix for [#131](https://github.com/twhl-community/halflife-updated/issues/131) to allow players to keep weapons and be given empty exhaustible weapons
* Fixed players not being given exhaustible weapons when they get the ammo for them [#153](https://github.com/twhl-community/halflife-updated/issues/153) (Thanks [Ronin4862](https://github.com/Ronin4862))
* Fixed weapon deploy animations caused by lastinv command not working properly [#154](https://github.com/twhl-community/halflife-updated/issues/154) (Thanks jay!)
* Fixed alien controllers facing in non-combat state [#155](https://github.com/twhl-community/halflife-updated/pull/155) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Fixed scientist voice pitch [#156](https://github.com/twhl-community/halflife-updated/pull/156) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Cleaned up weapons code a bit (Note: some code has been moved which could break mods, make sure to double check these changes in your mod if you merge them in) [#157](https://github.com/twhl-community/halflife-updated/issues/157)
* Fixed tripmine body value being influenced by events from other weapons during deploy [#158](https://github.com/twhl-community/halflife-updated/issues/158) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Fixed incorrect body values being passed into EV_WeaponAnimation [#159](https://github.com/twhl-community/halflife-updated/issues/159) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Fixed autoswitch from tripmine to RPG causing wrong animation to play when weapon prediction is off [#160](https://github.com/twhl-community/halflife-updated/issues/160)
* Updated smdlexp to use 3DS Max 2023 SDK [#149](https://github.com/twhl-community/halflife-updated/pull/149) (Thanks [fel1x-developer](https://github.com/fel1x-developer))
    * Updated to fix configuration issues so both debug and release profiles compile and don't fail when trying to copy the library to the 3DS Max plugins directory
    * Added errors to stop compilation if the library is not built as 64 bit or if it does not use the Unicode character set (required for 3DS Max 2023 plugins)
    * Note: plugin has not been tested. Use this plugin instead: https://knockout.chat/thread/806/1
* Added keyvalue `allow_item_dropping` to control whether NPCs can drop items (Opposing Force Updated [#72](https://github.com/twhl-community/halflife-op4-updated/issues/72))
* Refactored env_sound code to use correct data type for room type, don't use goto to control think rate
* Always send room_type changes to client, set room_type to 0 by default, reset room_type to 0 on map change, save room_type and restore it when loading same map (halflife issues [#1144](https://github.com/ValveSoftware/halflife/issues/1144) and [#2936](https://github.com/ValveSoftware/halflife/issues/2936))
* Fixed weapon deploy animations on fast switching with user binds [#161](https://github.com/twhl-community/halflife-updated/pull/161) (Thanks [FreeSlave](https://github.com/FreeSlave))
* Added fixes from Marphy Black's [Half-Life Fact Fixes](https://github.com/Revenant100/halflife-FactFilesFixes) (Thanks Marphy Black)

### Project changes

* Added `fgd/halflife.fgd`

## Changes in V1.0.0 Beta 011

### Bug fixes

* Fixed node graphs being loaded from search paths other than GAMECONFIG path [#145](https://github.com/twhl-community/halflife-updated/issues/145) (properly this time)
* Fixed node graphs being loaded with a null byte appended [#146](https://github.com/twhl-community/halflife-updated/issues/146)
* Fixed "fullupdate" call making a HUD disappear [#147](https://github.com/twhl-community/halflife-updated/issues/147) (Thanks SmileyAG)
* Fixed STL Algorithm Header Errors When Included with Platform.h [#148](https://github.com/twhl-community/halflife-updated/pull/148) (Thanks edgarbarney)

## Changes in V1.0.0 Beta 010

### Bug fixes

* Fixed Osprey crashing if it has no valid target (halflife issue [#3259](https://github.com/ValveSoftware/halflife/issues/3259))
* Fixed Osprey teleporting to world origin if it isn't moving at all (halflife issue [#3260](https://github.com/ValveSoftware/halflife/issues/3260))
* Fixed Osprey not firing trigger targets (halflife issue [#3261](https://github.com/ValveSoftware/halflife/issues/3261))
* Fixed Osprey engine damage effects showing on wrong engine (halflife issue [#3263](https://github.com/ValveSoftware/halflife/issues/3263)) (Thanks Hezus)
* Fixed Apache not firing trigger targets (halflife issue [#3262](https://github.com/ValveSoftware/halflife/issues/3262))
* Fixed Error in ServerCtrl [#135](https://github.com/twhl-community/halflife-updated/pull/135) (Thanks fel1x-developer)
* Pass director stufftext commands to filtered client command function (halflife issue [#1497](https://github.com/ValveSoftware/halflife/issues/1497))
* Fixed RPG sometimes getting stuck unable to reload (halflife issue [#3264](https://github.com/ValveSoftware/halflife/issues/3264)) (Thanks Ronin4862)
* Fixed monster_tentacle not interpolating (halflife issue [#3228](https://github.com/ValveSoftware/halflife/issues/3228)) (Thanks Anton)
* Fixed node graphs being loaded from search paths other than GAME path [#145](https://github.com/twhl-community/halflife-updated/issues/145)
* Fixed momentary_door restarting movement sound repeatedly when moving back to starting position (halflife issue [#3265](https://github.com/ValveSoftware/halflife/issues/3265))

### Project changes

* Fixed symlinks to Linux and MacOS builds of SDL2 [#137](https://github.com/twhl-community/halflife-updated/pull/137) (Thanks MegaBrutal)
* Deleted Makefiles and libraries for MacOS X (Half-Life 1 is 32 bit, no longer supported by MacOS) [#141](https://github.com/twhl-community/halflife-updated/pull/141) (Thanks fel1x-developer)
* Upgraded tool projects to use Visual Studio 2019, fixed as many warnings as possible, all tools aside from smdlexp compile, all tools compiled as C++, most tools work as intended [#138](https://github.com/twhl-community/halflife-updated/issues/138) (Thanks fel1x-developer for helping with this work)
    * Fixed missing archtypes.h header include [#136](https://github.com/twhl-community/halflife-updated/issues/136)
    * Updated readme.txt for latest methods installing MFC, smdlexp and SDL2 [#140](https://github.com/twhl-community/halflife-updated/pull/140) (Thanks fel1x-developer)
    * Removed obsolete VS6 & makefile project files from tools code [#142](https://github.com/twhl-community/halflife-updated/issues/142)
    * Removed bsplib.h & bsplib.cpp (redundant) [#143](https://github.com/twhl-community/halflife-updated/issues/143)

## Changes in V1.0.0 Beta 009

### Weapons

* Allow weapons to network data as needed
* Initialize frametime value so weapon prediction code can use it
* Fixed chainsaw hornetgun sounds and mostly fixed hornetgun not playing firing animation (halflife issue [#556](https://github.com/ValveSoftware/halflife/issues/556))
* Fixed Gauss charge sound not always being reset (halflife issue [#3188](https://github.com/ValveSoftware/halflife/issues/3188))
* Show pickup icons in HUD for all weapons (halflife issue [#3137](https://github.com/ValveSoftware/halflife/issues/3137))
* Fixed picking up weapons that give free/regenerating ammo sometimes showing weapon icon as red (halflife issue [#3250](https://github.com/ValveSoftware/halflife/issues/3250))
* Fixed hand grenades staying primed when switching away or dropping the weapon (halflife issue [#3251](https://github.com/ValveSoftware/halflife/issues/3251))
* Don't give player exhaustible weapon if it has no ammo left [#131](https://github.com/twhl-community/halflife-updated/issues/131)
* Applied Opposing Force weapon prediction timing fix to Half-Life (halflife issue [#3252](https://github.com/ValveSoftware/halflife/issues/3252))
* Fixed event code crashing if weapon prediction is disabled [#132](https://github.com/twhl-community/halflife-updated/issues/132)
* Automatically switch away from exhaustible weapons that have no ammo left [#133](https://github.com/twhl-community/halflife-updated/issues/133)
* Fixed controlling a func_tank with empty weapon deploying invisible weapon when stopping control [#134](https://github.com/twhl-community/halflife-updated/issues/134)

### Bug fixes

* Fixed hornets crashing the game if they hit you after a level transition (halflife issue [#1598](https://github.com/ValveSoftware/halflife/pull/1598))
* Fixed UTIL_IsServer returning inverted value [#130](https://github.com/twhl-community/halflife-updated/issues/130) (Thanks tmp64)
* Fixed m_rawinput changes not taking effect if a new map has started and less time has passed than on any previous map or if weapon prediction is disabled (halflife issue [#3255](https://github.com/ValveSoftware/halflife/issues/3255))
* Fixed FL_FAKECLIENT flag being cleared in some places (halflife issue [#3256](https://github.com/ValveSoftware/halflife/issues/3256))
* Fixed Osprey not interpolating (halflife issue [#3228](https://github.com/ValveSoftware/halflife/issues/3228)) (Thanks Hezus)

### Project changes

* Rewrote the installation and packing scripts to use C#

## Changes in V1.0.0 Beta 008

### Bug fixes

* Fixed crowbar playing back swing events twice sometimes (halflife issue [#3230](https://github.com/ValveSoftware/halflife/issues/3230))
* Fixed weapon empty sound playing twice (halflife issue [#3231](https://github.com/ValveSoftware/halflife/issues/3231))
* Fixed Gauss sound not stopping when players are not in the PAS (halflife issue [#3233](https://github.com/ValveSoftware/halflife/issues/3233))
* Fixed command menu text not being localized (halflife issue [#2387](https://github.com/ValveSoftware/halflife/issues/2387))
* Fixed localization function cutting off text (halflife issue [#2608](https://github.com/ValveSoftware/halflife/issues/2608))
* Fixed chat text sometimes using the wrong color (halflife issue [#3240](https://github.com/ValveSoftware/halflife/issues/3240))
* Fixed status bar accepting out of range indices (halflife issue [#3242](https://github.com/ValveSoftware/halflife/issues/3242))
* Fixed explosions going through walls in certain cases (halflife issue [#3244](https://github.com/ValveSoftware/halflife/issues/3244))
* Fixed tripmine beams not using correctly start position sometimes (halflife issue [#1670](https://github.com/ValveSoftware/halflife/issues/1670)) (Thanks vasiavasiavasia95)
* Fixed trip mine laser beam duplicating after going back and forth through level changes (halflife issue [#3245](https://github.com/ValveSoftware/halflife/issues/3245)) (Thanks vasiavasiavasia95)

### UX improvements

* Print the "SayText" messages instantly in the console (halflife issue [#1440](https://github.com/ValveSoftware/halflife/issues/1440))

## Changes in V1.0.0 Beta 007

### Bug fixes

* Fixed cycler_sprite not saving [#43](https://github.com/twhl-community/halflife-updated/issues/43)
* Fixed rpg reload reading uninitialized variable [#42](https://github.com/twhl-community/halflife-updated/issues/42)
* Fixed RPG client prediction results stored in wrong pointer [#67](https://github.com/twhl-community/halflife-updated/pull/67) (Thanks malortie)
* Fixed crossbow fire on empty sequence not playing [#68](https://github.com/twhl-community/halflife-updated/pull/68) (Thanks malortie)
* Fixed glock fire on empty sequence not playing [#70](https://github.com/twhl-community/halflife-updated/pull/70) (Thanks malortie)
* Fixed func_rotating not reporting keyvalue spawnorigin as handled [#72](https://github.com/twhl-community/halflife-updated/issues/72)
* Fixed infodecal not reporting keyvalue texture as handled [#73](https://github.com/twhl-community/halflife-updated/issues/73)
* Fixed beam attachment invalidated on restore [#74](https://github.com/twhl-community/halflife-updated/pull/74) (Thanks malortie)
* Fixed Snark movetype being changed unconditionally (halflife issue [#3175](https://github.com/ValveSoftware/halflife/issues/3175))
* Fixed Human Grunt checking enemy incorrectly in CheckMeleeAttack1 (halflife issue [#3176](https://github.com/ValveSoftware/halflife/issues/3176))
* Fixed CTalkMonster::RunTask not properly initializing variable (halflife issue [#3177](https://github.com/ValveSoftware/halflife/issues/3177))
* Fixed speaker entity not handling invalid preset values correctly (halflife issue [#3178](https://github.com/ValveSoftware/halflife/issues/3178))
* Reserved enough entvars for all weapons + local player in client weapon prediction code [#97](https://github.com/twhl-community/halflife-updated/issues/97)
* Fixed crowbar not showing in weapon list if there are empty weapon slots with a lower id (halflife [#3181](https://github.com/ValveSoftware/halflife/issues/3181))
* Fixed invalidated linked entities in node graph [#106](https://github.com/twhl-community/halflife-updated/pull/106) (Thanks zaklaus)
* Fixed default implementation of HasTarget not checking the right variable [#109](https://github.com/twhl-community/halflife-updated/pull/109) (Thanks malortie)
* Implemented particle manager in client library [#122](https://github.com/twhl-community/halflife-updated/issues/122)
    * This change fixes halflife issue [#2196](https://github.com/ValveSoftware/halflife/issues/2196) by embedding the particle manager in the client library. It also fixes compatibility issues caused by optimizations applied to the Vector type.
    * Reworked particle memory management to allow far more particles to be allocated before crashing by running out of memory (previous limit was around 100 particles at the same time, new limit closer to 1 million). Fixes halflife [#1447](https://github.com/ValveSoftware/halflife/issues/1447)
    * Fixed memory leak in the physics force list: halflife [#3213](https://github.com/ValveSoftware/halflife/issues/3213)
    * Fixed use-after-free bug that could cause difficult to debug problems in (hopefully) theoretical cases: [#3214](https://github.com/ValveSoftware/halflife/issues/3214)
    * Optimized some of the code to be more efficient
    * Reworked the CBaseParticle class to use a simpler design
* Fixed the crowbar damage always being calculated halved (halflife issue [#1600](https://github.com/ValveSoftware/halflife/pull/1600) (Thanks YaLTeR)
* Fixed packing script creating empty directory in hd directory [#120](https://github.com/twhl-community/halflife-updated/issues/120)
* Fixed V_CalcBob not initializing static variables [#126](https://github.com/twhl-community/halflife-updated/issues/126)
* Fixed tripmine viewmodel having wrong body on first pickup [#129](https://github.com/twhl-community/halflife-updated/issues/129)
* Fixed models stuttering due to client side interpolation (halflife issue [#3228](https://github.com/ValveSoftware/halflife/issues/3228)) (Thanks Uncle Mike, and vasiavasiavasia95 and Bacontsu for bringing this fix to my attention)
* Fixed player physics hull sizes not initializing properly (halflife issue [#3229](https://github.com/ValveSoftware/halflife/issues/3229)) (Thanks vasiavasiavasia95)

### Project changes

* Rewrote installation scripts to use PowerShell
* Enabled multiprocessor compilation for Windows builds [#39](https://github.com/twhl-community/halflife-updated/issues/39)
* Changed default toolset to non-XP compatible one and set Windows SDK version to W10 [#44](https://github.com/twhl-community/halflife-updated/issues/44)
* Added new cvar flags added by recent engine updates [#46](https://github.com/twhl-community/halflife-updated/issues/46)
* Added new cvar flags for command filtering [#121](https://github.com/twhl-community/halflife-updated/issues/121) (Thanks a1batross and vasiavasiavasia95)
* Set C++ standard to 17 [#85](https://github.com/twhl-community/halflife-updated/issues/85)
* Provided access to engine filesystem [#86](https://github.com/twhl-community/halflife-updated/issues/86)
* Use engine filesystem to load and save node graph files [#87](https://github.com/twhl-community/halflife-updated/issues/87)
* Define and use constants for maximum number of players and weapons [#96](https://github.com/twhl-community/halflife-updated/issues/96)
* Added support for saving and restoring 64 bit integers [#99](https://github.com/twhl-community/halflife-updated/issues/99)
* Added support for adding 64 weapons [#98](https://github.com/twhl-community/halflife-updated/issues/98)
* Added missing tracerCount for EV_HLDM_FireBullets [#107](https://github.com/twhl-community/halflife-updated/pull/107) (Thanks malortie)
* Removed VS2017 projects [#118](https://github.com/twhl-community/halflife-updated/issues/118)
* Fixed func_tanks not being usable [#125](https://github.com/twhl-community/halflife-updated/pull/125) (Thanks suXinjke)
* Fixed case sensitivity of OpenGL header inclusion
* Fixed reworked CGamePlayerEquip::KeyValue method [#128](https://github.com/twhl-community/halflife-updated/issues/128)

### Code cleanup

* Made event playback functions const correct [#41](https://github.com/twhl-community/halflife-updated/issues/41)
* Removed unused skiplocal parameter [#38](https://github.com/twhl-community/halflife-updated/issues/38)
* Made CBaseEntity debug setters const correct
* Made HUD string drawing methods const correct
* Improved const correctness in engine APIs
* Removed obsolete preprocessor macro checks [#47](https://github.com/twhl-community/halflife-updated/issues/47)
* Cleaned up uses of the node graph global [#48](https://github.com/twhl-community/halflife-updated/issues/48)
* Disabled as many Windows.h dependencies as possible [#49](https://github.com/twhl-community/halflife-updated/issues/49)
* Removed benchmarking functionality [#54](https://github.com/twhl-community/halflife-updated/issues/54)
* Removed game_controls.lib from the client project [#55](https://github.com/twhl-community/halflife-updated/issues/55)
* Defined platform-agnostic types for common structures [#56](https://github.com/twhl-community/halflife-updated/issues/56)
* Removed obsolete engine APIs related to security modules [#57](https://github.com/twhl-community/halflife-updated/issues/57)
* Removed extern C wrappers around functions [#58](https://github.com/twhl-community/halflife-updated/issues/58)
* Reworked some uses of platform-specific code to be cross-platform [#59](https://github.com/twhl-community/halflife-updated/issues/59)
* Merged platform abstraction headers [#53](https://github.com/twhl-community/halflife-updated/issues/53)
* Removed redundant include guards [#52](https://github.com/twhl-community/halflife-updated/issues/52)
* Converted all include guards to pragma once [#50](https://github.com/twhl-community/halflife-updated/issues/50)
* Removed duplicate interface.h/.cpp files [#60](https://github.com/twhl-community/halflife-updated/issues/60)
* Added pragma once to all headers that need it [#51](https://github.com/twhl-community/halflife-updated/issues/51)
* Use snprintf and vsnprintf everywhere instead of _snprintf and _vsnprintf [#61](https://github.com/twhl-community/halflife-updated/issues/61)
* Use WIN32 preprocessor macro instead of _WIN32 [#62](https://github.com/twhl-community/halflife-updated/issues/62)
* Removed unnecessary _cdecl in function declarations [#63](https://github.com/twhl-community/halflife-updated/issues/63)
* Removed useless try-catch statement in command menu parsing code [#64](https://github.com/twhl-community/halflife-updated/issues/64)
* Reworked some uses of platform-specific code to be cross-platform [#59](https://github.com/twhl-community/halflife-updated/issues/59)
* Save FIELD_BOOLEAN as byte array [#65](https://github.com/twhl-community/halflife-updated/issues/65)
* Reworked operator new and delete overloads to allocate memory directly [#71](https://github.com/twhl-community/halflife-updated/issues/71)
* Removed identical line in conditional statement [#75](https://github.com/twhl-community/halflife-updated/pull/75) (Thanks malortie)
* Removed unused files (Thanks malortie):
    * AI_BaseNPC_Schedule.cpp [#77](https://github.com/twhl-community/halflife-updated/pull/77)
    * glock.cpp [#78](https://github.com/twhl-community/halflife-updated/pull/78)
    * mpstubb.cpp [#80](https://github.com/twhl-community/halflife-updated/pull/80)
    * MOTD.cpp [#81](https://github.com/twhl-community/halflife-updated/pull/81)
    * scoreboard.cpp [#82](https://github.com/twhl-community/halflife-updated/pull/82)
* Removed DMC and Ricochet VS 2017 projects to match removal of VS 2019 projects [#79](https://github.com/twhl-community/halflife-updated/pull/79) (Thanks malortie)
* Fixed invalid conversions between bool and other types [#69](https://github.com/twhl-community/halflife-updated/issues/69)
* Reworked all uses of int and int-like types to use bool when the value is boolean [#83](https://github.com/twhl-community/halflife-updated/issues/83)
* Enabled certain Clang-tidy checks [#66](https://github.com/twhl-community/halflife-updated/issues/66)
* Formatted all files and added clang-format configuration file [#84](https://github.com/twhl-community/halflife-updated/issues/84)
* Removed unused types [#88](https://github.com/twhl-community/halflife-updated/issues/88)
* Removed unused globals and duplicate global variable forward declarations [#89](https://github.com/twhl-community/halflife-updated/issues/89)
* Removed obsolete interface APIs [#90](https://github.com/twhl-community/halflife-updated/issues/90)
* Overhauled vector types to use constexpr, simplify code [#92](https://github.com/twhl-community/halflife-updated/issues/92)
* Fixed or silence compiler warnings [#91](https://github.com/twhl-community/halflife-updated/issues/91)
* Removed unused VGUI1 code in vgui_int.cpp [#93](https://github.com/twhl-community/halflife-updated/issues/93)
* Reworked CSave & CRestore to eliminate null data pointers [#94](https://github.com/twhl-community/halflife-updated/issues/94)
* Reworked mouse thread code to use standard types [#95](https://github.com/twhl-community/halflife-updated/issues/95)
* Replaced stackalloc-based saytext code with fixed-size buffer to avoid using alloca (halflife issue [#3179](https://github.com/ValveSoftware/halflife/issues/3179))
* Use sound array macros (Thanks malortie):
    * Alien grunt [#100](https://github.com/twhl-community/halflife-updated/pull/100)
    * Gargantua [#101](https://github.com/twhl-community/halflife-updated/pull/101)
    * Alien slave [#102](https://github.com/twhl-community/halflife-updated/pull/102)
    * Leech [#103](https://github.com/twhl-community/halflife-updated/pull/103)
    * Zombie [#104](https://github.com/twhl-community/halflife-updated/pull/104)
* Removed unused variables in the client side event playback code [#105](https://github.com/twhl-community/halflife-updated/pull/105) (Thanks malortie)
* Renamed hl_wpn_glock.cpp to glock.cpp [#108](https://github.com/twhl-community/halflife-updated/pull/108) (Thanks malortie and JoelTroch for the Linux changes related to this)
* Removed preprocessor checks for other games (Thanks malortie):
    * Team Fortress Classic [#110](https://github.com/twhl-community/halflife-updated/pull/110)
    * Condition Zero [#111](https://github.com/twhl-community/halflife-updated/pull/111)
    * Counter-Strike [#112](https://github.com/twhl-community/halflife-updated/pull/112)
    * Day Of Defeat [#113](https://github.com/twhl-community/halflife-updated/pull/113)
    * Blue Shift [#114](https://github.com/twhl-community/halflife-updated/pull/114)
    * Deathmatch Classic and Threewave [#115](https://github.com/twhl-community/halflife-updated/pull/115)

## Changes in V1.0.0 Beta 006

* Reverted "Fixed multisource having off-by-one error in trigger handling" (halflife issue [[#1737|https://github.com/ValveSoftware/halflife/issues/1737]]) to avoid edge cases where entities triggering the multisource use a delayed trigger which is not supported by fixed code
* Fixed func_breakable keyvalue "explosion" not working (#24) (Thanks ArroganceJustified)
* Fixed saytext message handling potentially causing buffer overflow (halflife issue [[#3095|https://github.com/ValveSoftware/halflife/issues/3095]])
* Implemented skill cvars `sk_plr_hornet_dmg1/2/3` for player Hornet gun damage
* Fixed memory leak in VGUI1 image loading code (halflife issue [[#3101|https://github.com/ValveSoftware/halflife/issues/3101]])
* Fixed underwater breathing sounds playing when standing in shallow water (halflife issue [[#3110|https://github.com/ValveSoftware/halflife/issues/3110]])
* Fixed camera not being consistently centered when raw mouse input is enabled (Thanks IntriguingTiles) [#32](https://github.com/twhl-community/halflife-updated/pull/32)
*  Update Linux makefiles to use g++ instead of gcc to compile and link all code (Thanks MegaBrutal for bringing this to my attention) [#33](https://github.com/twhl-community/halflife-updated/issues/33)
* Moved DMC & Ricochet source code and projects to their own repositories. See About page for more information: https://github.com/twhl-community/halflife-updated/wiki/About
* Deleted DMC & Ricochet fgds
* Removed DMC & Ricochet Makefiles

## Changes in V1.0.0 Beta 005

* Fixed Egon flare sprite not showing (halflife issue [[#3066|https://github.com/ValveSoftware/halflife/issues/3066]])
* Fixed crosshair remaining present when starting new game (halflife issue [[#692|https://github.com/ValveSoftware/halflife/issues/692]])
* Ensure CBaseEntity::Instance(entvars_t*) does not cause crashes when given null pointer (halflife issue [[#3056|https://github.com/ValveSoftware/halflife/issues/3056]])
* Moved DMC & Ricochet projects to their own solution files. These projects were updated to work with VS2017 and VS2019 but will not receive any further changes.
* Made Half-Life client dll depend on server dll to ensure server is built when launching a client debug session
* Use RGB_YELLOWISH for health vertical bar & TFC concussion effect (was previously hardcoded to the same color)
* Fixed incorrect idle time values for RPG fidget, satchel & snark grenade deploy animations (halflife issue [[#2495|https://github.com/ValveSoftware/halflife/issues/2495]])
* Fixed hand grenade not playing deploy animation after finishing a throw (halflife issue [[#2495|https://github.com/ValveSoftware/halflife/issues/2495]])
* Switch to no weapon if retired weapon could not be switched from (halflife issue [[#2495|https://github.com/ValveSoftware/halflife/issues/2495]])
* Ensured PrimaryAmmoIndex & SecondaryAmmoIndex return valid values (halflife issue [[#3073|https://github.com/ValveSoftware/halflife/issues/3073]])
* Increased size of Health message to 2 to allow sending of signed short values (halflife issue [[#2149|https://github.com/ValveSoftware/halflife/issues/2149]])
* Increased number of digits allowed in health value to 5 (halflife issue [[#2149|https://github.com/ValveSoftware/halflife/issues/2149]])
* Fixed Gargantua stomp attack being framerate-dependent (halflife issue [[#2876|https://github.com/ValveSoftware/halflife/issues/2876]])
* Fixed trigger_camera rotation speed being framerate-dependent (halflife issue [[#2924|https://github.com/ValveSoftware/halflife/issues/2924]])
* Fixed flashlight icon not restoring properly on save game load (halflife issue [[#388|https://github.com/ValveSoftware/halflife/issues/388]])
* Set current time to time specified in save game data when saving & restoring entities (halflife issue [[#3065|https://github.com/ValveSoftware/halflife/issues/3065]])
* Initialize ItemInfoArray & AmmoInfoArray on the client (halflife issue [[#3071|https://github.com/ValveSoftware/halflife/issues/3071]])
* Use cached item info for client predicted reloads
* Fixed gauss gun beams going in the wrong direction when player is viewing themselves through a camera (halflife issue [[#1744|https://github.com/ValveSoftware/halflife/issues/1744]])
* Fixed local player model using wrong pitch angles when seen through another entity (halflife issue [[#3075|https://github.com/ValveSoftware/halflife/issues/3075]])
* Fixed buttons not sparking in multiplayer (halflife issue [[#1681|https://github.com/ValveSoftware/halflife/issues/1681]])
* Fixed incorrect check for monster gag spawnflag (halflife issue [[#3076|https://github.com/ValveSoftware/halflife/issues/3076]])
* Fixed func_plat trigger accessing freed memory if platform is removed (halflife issue [[#1725|https://github.com/ValveSoftware/halflife/issues/1725]])
* Fixed multisource having off-by-one error in trigger handling (halflife issue [[#1737|https://github.com/ValveSoftware/halflife/issues/1737]])
* Fixed GetHullBounds & HUD_GetHullBounds using wrong syntax to copy vectors (halflife issue [[#1703|https://github.com/ValveSoftware/halflife/issues/1703]])
* Fixed CreateBaseline using player bounds parameters incorrectly (halflife issue [[#3078|https://github.com/ValveSoftware/halflife/issues/3078]])
* Fixed camera.h include guard (halflife issue [[#3035|https://github.com/ValveSoftware/halflife/pull/3035]])
* Added new variables to player_info_t struct (Thanks ArroganceJustified)
* Linux compilation fixes (Thanks Shepard and LogicAndTrick)
* Made IsFacing non-static so it can be used in other source files
* Fixed HD Revolver model not playing the reload sound (halflife issue [[2351|https://github.com/ValveSoftware/halflife/issues/2351]])
* Added `delta.lst` and the `resource` directory to the game installation

### Code cleanup

* Ensured entity destructors are called
* Removed void from functions taking no arguments (clang-tidy modernize-redundant-void-arg)
* Used override specifier where needed (clang-tidy modernize-use-override) (Thanks LogicAndTrick for catching some missing ones)
* Explicitly cast result of `offsetof` to int to silence warnings
* Removed unnecessary returns and semicolons
* Defined NOMINMAX to disable min & max macros defined in Windows header
* Moved FindEntityForward to util.h/.cpp & rename to UTIL_FindEntityForward
* Refactored ClientCommand to acquire the player pointer only once
* Refactored Vector code to remove use of vec3_t
* Removed duplicate Vector definitions in util_vector.h
* Removed shared_vec3_origin
* Removed unused vector functions
* Consolidated math header includes
* Removed duplicate PI constant definitions
* Clean up animation.cpp header includes
* Moved primitive types aliases to Platform.h when possible
* Properly defined model_s & model_t in pm_defs.h to silence Intellisense errors
* Added #pragma once to mathlib header
* Made Vector constructors constexpr
* Refactored g_vecZero & vec3_origin into constexpr constants
* Made R_TempSprite dir parameter const
* Made hull vectors constexpr constants
* Removed duplicate player view constants
* Defined constant for dead player view
* Cleaned up magic numbers to use constants
* Removed duplicate weapon animation enums
* Consolidated damage constants
* Removed duplicate Egon constants, move constants to weapons.h
* Removed duplicate vector cone constants
* Removed duplicate SND_CHANGE_PITCH constant
* Moved ARRAYSIZE definition to Platform.h
* Moved duplicated weapons code into weapons_shared.cpp
* Moved clamp macro to Platform.h
* Moved FNullEnt(CBaseEntity*) to cbase.h
* Removed duplicate function declarations in player.cpp
* Added common & engine headers to projects
* Removed hud_iface.h (redundant)
* Removed duplicate FTENT_FADEOUT constant
* Removed VGUI1 server browser code (obsolete)
* Removed unused soundsystem, console panel, control config panel files
* Removed old project files & module definition files
* Removed duplicate event function declarations, use event_args_t to ensure Intellisense can link declarations and definitions
* Removed some double semicolons (Thanks LogicAndTrick)

## Changes in V1.0.0 Beta 004

* Added the option to change the default field of view to the Multiplayer Advanced dialog
* Updated delta.lst to use latest one from vanilla Half-Life (includes spectator variables) (halflife issue [[#3062|https://github.com/ValveSoftware/halflife/issues/3062]])
* Use correct number of bits for iuser2 to ensure spectating player with index 32 works correctly (halflife issue [[#826|https://github.com/ValveSoftware/halflife/issues/826]])
* Fixed sprite attachment bug causing sprites to appear in seemingly random locations (halflife issue [[#3058|https://github.com/ValveSoftware/halflife/issues/3058]])
* Fixed monsters being resurrected if triggered while dying (halflife issue [[#3061|https://github.com/ValveSoftware/halflife/issues/3061]])
* Made default_fov cvar archived (halflife issue [[#2237|https://github.com/ValveSoftware/halflife/issues/2237]])
* Fixed Tripmine & Python having wrong bodies sometimes when weapon prediction is enabled
* Reset flashlight HUD status on save game load (halflife issue [[#388|https://github.com/ValveSoftware/halflife/issues/388]])
* Fixed ammo HUD drawing last weapon ammo if player has all weapons stripped (halflife issue [[#3063|https://github.com/ValveSoftware/halflife/issues/3063]])
* Fixed train HUD not restoring (halflife issue [[#3064|https://github.com/ValveSoftware/halflife/issues/3064]])
* Changed game directory name to `halflife_updated`

## Changes in V1.0.0 Beta 003

* Made scientists aware of carcasses, meat and garbage so they can comment on it (halflife issue [[#2982|https://github.com/ValveSoftware/halflife/issues/2982]]) (Thanks NongBenz)
* Fixed Human Grunt shotgun damage not using correct skill values (halflife issue [[#3046|https://github.com/ValveSoftware/halflife/issues/3046]])
* Ensured flashlight sprites have enough space between them and death notice text (halflife issue [[#1047|https://github.com/ValveSoftware/halflife/issues/1047]])
* Removed all uses of pev->fov to simplify changing fov, and always let the server set the fov to ensure it restores correctly (halflife issue [[#3044|https://github.com/ValveSoftware/halflife/issues/3044]])
* Removed m_fInZoom from Python & Crossbow to ensure zoom state restores correctly (halflife issue [[#3045|https://github.com/ValveSoftware/halflife/issues/3045]])
* Made cheat protected commands `give`, `fov` and `impulse` check the value of `sv_cheats` on demand instead of caching the value, enabling the use of these cheats without having to restart the map (halflife issue [[#2511|https://github.com/ValveSoftware/halflife/issues/2511]])
* Fixed Crowbar using wrong time base for swing think (halflife issue [[#2296|https://github.com/ValveSoftware/halflife/issues/2296]])
* Refactored user messages globals to remove duplicate forward declarations
* Added new engine functions added in the latest engine updates

## Changes in V1.0.0 Beta 002

* Re-implemented view roll (halflife issue [[#1544|https://github.com/ValveSoftware/halflife/issues/1544]])
    * Cvars `cl_rollangle` (maximum roll angle) and `cl_rollspeed` (roll speed) control this feature, and can be changed in the Multiplayer Advanced dialog
* Re-implemented view model tilt (halflife issue [[#1544|https://github.com/ValveSoftware/halflife/issues/1544]])
    * Cvar `cl_bobtilt` controls this feature (default off), and can be changed in the Multiplayer Advanced dialog
* Fixed Egon gun beam colors being too bright (halflife issue [[#3033|https://github.com/ValveSoftware/halflife/issues/3033]])

## Changes in V1.0.0 Beta 001

Project changes:
* Added project files for Visual Studio 2017 and Visual Studio 2019
* Removed project files for Visual Studio 2010

Fixes that were added by Valve to the game but not the SDK:
* Fixed crash when switching back to last item when item was removed (halflife issue [[#1815|https://github.com/ValveSoftware/halflife/issues/1815]])
* Implement cl_autowepswitch cvar (halflife issues [[#169|https://github.com/ValveSoftware/halflife/issues/169]] and [[#2749|https://github.com/ValveSoftware/halflife/issues/2749]])
* Fixed MP5 using wrong spread values for multiplayer and singleplayer (halflife issue [[#2095|https://github.com/ValveSoftware/halflife/issues/2095]])
* Fixed not being able to reload weapons after loading a save game (halflife issue [[#1707|https://github.com/ValveSoftware/halflife/issues/1707]])
* Use fps-independent frametime for NPC pitch & yaw adjustments (halflife issue [[#2458|https://github.com/ValveSoftware/halflife/issues/2458]])
* Multiple fixes for halflife issue [[#617|https://github.com/ValveSoftware/halflife/issues/617]]:
    * Fixed shotgun pump sound not playing when holding down attack or when shooting last shell
    * Fixed Python repeatedly playing dry fire sound
    * Fixed RPG empty sound playing constantly
    * Fixed Tripmine weapon appearing zoomed in

Fixes that have been reported but not yet fixed in the original game:
* Updated codebase to use correct functions to allow compilation with newer versions of Visual Studio
* Added null check to bullsquid spit attack animation event to prevent crash if enemy pointer is cleared between animation start and animation event occurrence (halflife issue [[#2834|https://github.com/ValveSoftware/halflife/issues/2834]])
* Fixed a bug that could cause a crash if an RPG weapon were removed from a player while a rocket fired from it is in flight (halflife issue [[#1719|https://github.com/ValveSoftware/halflife/issues/1719]])
* Fixed chainsaw crowbar effect when hitting corpse (halflife issue [[#2590|https://github.com/ValveSoftware/halflife/issues/2590]])
* Fixed server not staying in sync with client when handling weapon attack times (halflife issue [[#1621|https://github.com/ValveSoftware/halflife/issues/1621]])
* Fixed m_rawinput 1 getting mouse stuck in box (halflife issue [[#1377|https://github.com/ValveSoftware/halflife/issues/1377]])
* Fixed gauss gun beams having wrong color and brightness compared to WON (halflife issue [[#2865|https://github.com/ValveSoftware/halflife/issues/2865]])
* Fixed mp5 autoswitching when primary ammo is depleted, even if player still has secondary ammo (halflife issue [[#3001|https://github.com/ValveSoftware/halflife/issues/3001]])
* Always return m_iSecondaryAmmoType from CBasePlayerWeapon::SecondaryAmmoIndex (halflife issue [[#3029|https://github.com/ValveSoftware/halflife/issues/3029]])
* Stop controlling func_tank on disconnect (prevents crashes) (halflife issue [[#2594|https://github.com/ValveSoftware/halflife/issues/2594]])
* Fixed egon creating fake beam when holding secondary attack key (halflife issue [[#2612|https://github.com/ValveSoftware/halflife/issues/2612]])
* Fixed Egon looping fire sound playing twice sometimes (halflife issue [[#1750|https://github.com/ValveSoftware/halflife/issues/1750]])
* Fixed weapon prediction allowing multiple reload actions instead of just one (halflife issue [[#2301|https://github.com/ValveSoftware/halflife/issues/2301]])
* Use MAX_WEAPONS for array of weapons (halflife issue [[#3030|https://github.com/ValveSoftware/halflife/issues/3030]])
* The player view entity is now restored after loading a save game, which allows trigger_camera entities to work properly if you save while they're active (halflife issue [[#3031|https://github.com/ValveSoftware/halflife/issues/3031]])
* Fixed Python glitching animation when holding primary attack with empty magazine (halflife issue [[3028|https://github.com/ValveSoftware/halflife/issues/3028]])
* Fixed weapon being left behind when picking up ammo from it (halflife issue [[#3027|https://github.com/ValveSoftware/halflife/issues/3027]])
* Fixed autoaim angles not resetting if autoaim is disabled while aiming at a target (halflife issue [[#966|https://github.com/ValveSoftware/halflife/issues/966]])
