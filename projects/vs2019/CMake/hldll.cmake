set(PROJECT_NAME hldll)

################################################################################
# Source groups
################################################################################
set(Header_Files__common
        "../../common/beamdef.h"
        "../../common/com_model.h"
        "../../common/con_nprint.h"
        "../../common/const.h"
        "../../common/crc.h"
        "../../common/cvardef.h"
        "../../common/director_cmds.h"
        "../../common/dlight.h"
        "../../common/dll_state.h"
        "../../common/entity_state.h"
        "../../common/entity_types.h"
        "../../common/enums.h"
        "../../common/event_api.h"
        "../../common/event_args.h"
        "../../common/event_flags.h"
        "../../common/hltv.h"
        "../../common/in_buttons.h"
        "../../common/mathlib.h"
        "../../common/netadr.h"
        "../../common/particledef.h"
        "../../common/Platform.h"
        "../../common/PlatformHeaders.h"
        "../../common/r_efx.h"
        "../../common/r_studioint.h"
        "../../common/screenfade.h"
        "../../common/Sequence.h"
        "../../common/studio_event.h"
        "../../common/usercmd.h"
        "../../common/weaponinfo.h"
        )
source_group("Header Files\\common" FILES ${Header_Files__common})

set(Header_Files__dlls
        "../../dlls/activity.h"
        "../../dlls/activitymap.h"
        "../../dlls/animation.h"
        "../../dlls/basemonster.h"
        "../../dlls/cbase.h"
        "../../dlls/cdll_dll.h"
        "../../dlls/client.h"
        "../../dlls/decals.h"
        "../../dlls/defaultai.h"
        "../../dlls/doors.h"
        "../../dlls/effects.h"
        "../../dlls/enginecallback.h"
        "../../dlls/explode.h"
        "../../dlls/extdll.h"
        "../../dlls/flyingmonster.h"
        "../../dlls/func_break.h"
        "../../dlls/gamerules.h"
        "../../dlls/hornet.h"
        "../../dlls/items.h"
        "../../dlls/monsterevent.h"
        "../../dlls/monsters.h"
        "../../dlls/nodes.h"
        "../../dlls/plane.h"
        "../../dlls/player.h"
        "../../dlls/saverestore.h"
        "../../dlls/schedule.h"
        "../../dlls/scripted.h"
        "../../dlls/scriptevent.h"
        "../../dlls/skill.h"
        "../../dlls/soundent.h"
        "../../dlls/spectator.h"
        "../../dlls/squadmonster.h"
        "../../dlls/talkmonster.h"
        "../../dlls/teamplay_gamerules.h"
        "../../dlls/trains.h"
        "../../dlls/UserMessages.h"
        "../../dlls/util.h"
        "../../dlls/vector.h"
        "../../dlls/weapons.h"
        )
source_group("Header Files\\dlls" FILES ${Header_Files__dlls})

set(Header_Files__engine
        "../../engine/custom.h"
        "../../engine/customentity.h"
        "../../engine/edict.h"
        "../../engine/eiface.h"
        "../../engine/progdefs.h"
        "../../engine/progs.h"
        "../../engine/shake.h"
        "../../engine/studio.h"
        )
source_group("Header Files\\engine" FILES ${Header_Files__engine})

set(Header_Files__game_shared
        "../../game_shared/filesystem_utils.h"
        )
source_group("Header Files\\game_shared" FILES ${Header_Files__game_shared})

set(Header_Files__pm_shared
        "../../pm_shared/pm_debug.h"
        "../../pm_shared/pm_defs.h"
        "../../pm_shared/pm_info.h"
        "../../pm_shared/pm_materials.h"
        "../../pm_shared/pm_movevars.h"
        "../../pm_shared/pm_shared.h"
        )
source_group("Header Files\\pm_shared" FILES ${Header_Files__pm_shared})

set(Header_Files__public
        "../../public/interface.h"
        )
source_group("Header Files\\public" FILES ${Header_Files__public})

set(Source_Files__common
        "../../common/mathlib.cpp"
        )
source_group("Source Files\\common" FILES ${Source_Files__common})

set(Source_Files__dlls
        "../../dlls/aflock.cpp"
        "../../dlls/agrunt.cpp"
        "../../dlls/airtank.cpp"
        "../../dlls/animating.cpp"
        "../../dlls/animation.cpp"
        "../../dlls/apache.cpp"
        "../../dlls/barnacle.cpp"
        "../../dlls/barney.cpp"
        "../../dlls/bigmomma.cpp"
        "../../dlls/bloater.cpp"
        "../../dlls/bmodels.cpp"
        "../../dlls/bullsquid.cpp"
        "../../dlls/buttons.cpp"
        "../../dlls/cbase.cpp"
        "../../dlls/client.cpp"
        "../../dlls/combat.cpp"
        "../../dlls/controller.cpp"
        "../../dlls/defaultai.cpp"
        "../../dlls/doors.cpp"
        "../../dlls/effects.cpp"
        "../../dlls/egon.cpp"
        "../../dlls/explode.cpp"
        "../../dlls/flyingmonster.cpp"
        "../../dlls/func_break.cpp"
        "../../dlls/func_tank.cpp"
        "../../dlls/game.cpp"
        "../../dlls/gamerules.cpp"
        "../../dlls/gargantua.cpp"
        "../../dlls/genericmonster.cpp"
        "../../dlls/gman.cpp"
        "../../dlls/h_ai.cpp"
        "../../dlls/h_battery.cpp"
        "../../dlls/h_cine.cpp"
        "../../dlls/h_cycler.cpp"
        "../../dlls/h_export.cpp"
        "../../dlls/hassassin.cpp"
        "../../dlls/headcrab.cpp"
        "../../dlls/healthkit.cpp"
        "../../dlls/hgrunt.cpp"
        "../../dlls/hornet.cpp"
        "../../dlls/houndeye.cpp"
        "../../dlls/ichthyosaur.cpp"
        "../../dlls/islave.cpp"
        "../../dlls/items.cpp"
        "../../dlls/leech.cpp"
        "../../dlls/lights.cpp"
        "../../dlls/maprules.cpp"
        "../../dlls/monstermaker.cpp"
        "../../dlls/monsters.cpp"
        "../../dlls/monsterstate.cpp"
        "../../dlls/mortar.cpp"
        "../../dlls/mp5.cpp"
        "../../dlls/multiplay_gamerules.cpp"
        "../../dlls/nihilanth.cpp"
        "../../dlls/nodes.cpp"
        "../../dlls/observer.cpp"
        "../../dlls/osprey.cpp"
        "../../dlls/pathcorner.cpp"
        "../../dlls/plane.cpp"
        "../../dlls/plats.cpp"
        "../../dlls/player.cpp"
        "../../dlls/python.cpp"
        "../../dlls/rat.cpp"
        "../../dlls/roach.cpp"
        "../../dlls/satchel.cpp"
        "../../dlls/schedule.cpp"
        "../../dlls/scientist.cpp"
        "../../dlls/scripted.cpp"
        "../../dlls/singleplay_gamerules.cpp"
        "../../dlls/skill.cpp"
        "../../dlls/sound.cpp"
        "../../dlls/soundent.cpp"
        "../../dlls/spectator.cpp"
        "../../dlls/squadmonster.cpp"
        "../../dlls/squeakgrenade.cpp"
        "../../dlls/subs.cpp"
        "../../dlls/talkmonster.cpp"
        "../../dlls/teamplay_gamerules.cpp"
        "../../dlls/tempmonster.cpp"
        "../../dlls/tentacle.cpp"
        "../../dlls/triggers.cpp"
        "../../dlls/tripmine.cpp"
        "../../dlls/turret.cpp"
        "../../dlls/UserMessages.cpp"
        "../../dlls/util.cpp"
        "../../dlls/world.cpp"
        "../../dlls/xen.cpp"
        "../../dlls/zombie.cpp"
        )
source_group("Source Files\\dlls" FILES ${Source_Files__dlls})

set(Source_Files__dlls__weapons
        "../../dlls/crossbow.cpp"
        "../../dlls/crowbar.cpp"
        "../../dlls/gauss.cpp"
        "../../dlls/ggrenade.cpp"
        "../../dlls/glock.cpp"
        "../../dlls/handgrenade.cpp"
        "../../dlls/hornetgun.cpp"
        "../../dlls/rpg.cpp"
        "../../dlls/shotgun.cpp"
        "../../dlls/weapons.cpp"
        "../../dlls/weapons_shared.cpp"
        )
source_group("Source Files\\dlls\\weapons" FILES ${Source_Files__dlls__weapons})

set(Source_Files__game_shared
        "../../game_shared/filesystem_utils.cpp"
        "../../game_shared/voice_gamemgr.cpp"
        )
source_group("Source Files\\game_shared" FILES ${Source_Files__game_shared})

set(Source_Files__pm_shared
        "../../pm_shared/pm_debug.cpp"
        "../../pm_shared/pm_math.cpp"
        "../../pm_shared/pm_shared.cpp"
        )
source_group("Source Files\\pm_shared" FILES ${Source_Files__pm_shared})

set(Source_Files__public
        "../../public/interface.cpp"
        )
source_group("Source Files\\public" FILES ${Source_Files__public})

set(ALL_FILES
        ${Header_Files__common}
        ${Header_Files__dlls}
        ${Header_Files__engine}
        ${Header_Files__game_shared}
        ${Header_Files__pm_shared}
        ${Header_Files__public}
        ${Source_Files__common}
        ${Source_Files__dlls}
        ${Source_Files__dlls__weapons}
        ${Source_Files__game_shared}
        ${Source_Files__pm_shared}
        ${Source_Files__public}
        )

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE hldll)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        )
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "hl"
        TARGET_NAME_RELEASE "hl"
        )
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/$<CONFIG>/${PROJECT_NAME}/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/$<CONFIG>/${PROJECT_NAME}/"
        )
set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
        )
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
        MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
        MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
        )
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../../dlls;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../engine;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../common;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../pm_shared;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../game_shared;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../public;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../utils/vgui/include;"
        )

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG"
        ">"
        "WIN32;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_WINDOWS;"
        "QUIVER;"
        "VOXEL;"
        "QUAKE2;"
        "VALVE_DLL;"
        "CLIENT_WEAPONS"
        )

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Od
            >
            $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
            >
            /MP;
            /std:c++17;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /GR;
            /Zc:threadSafeInit-;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
            )
    target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /INCREMENTAL
            >
            $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
            )
endif()

################################################################################
# Post build events
################################################################################
#add_custom_command_if(
#    TARGET ${PROJECT_NAME}
#    POST_BUILD
#    COMMANDS
#    COMMAND   $<CONFIG:Debug> cmd /c ""$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>..\\..\\filecopy.bat" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "dlls" "$<TARGET_FILE_NAME:${PROJECT_NAME}>""
#    COMMAND   $<CONFIG:Debug> cmd /c ""$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>..\\..\\filecopy.bat" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "dlls" "${TARGET_NAME}.pdb""
#    COMMAND $<CONFIG:Release> cmd /c ""$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>..\\..\\filecopy.bat" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "dlls" "$<TARGET_FILE_NAME:${PROJECT_NAME}>""
#    COMMAND $<CONFIG:Release> cmd /c ""$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>..\\..\\filecopy.bat" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>" "dlls" "${TARGET_NAME}.pdb""
#    COMMENT "Installing server files"
#)

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME} hldll)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../utils/vgui/lib/win32_vc6/vgui.lib;"
        "wsock32;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/public/sdl2.lib"
        )
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")



