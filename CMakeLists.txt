cmake_minimum_required(VERSION 3.23)

include(CheckIPOSupported)

if (MSVC)
	set(CMAKE_SYSTEM_VERSION 10.0)
endif()

project(HalfLifeMod
	VERSION 1.0.0
	DESCRIPTION "Half-Life CMake Mod Template"
	LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Disable the use of absolute paths in library paths even in development builds.
set(CMAKE_SKIP_BUILD_RPATH ON)

# Link statically with the runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Because we have 2 libraries that use the same symbols we have to default visibility to hidden so there are no collisions,
# and so the symbols don't merge and cause problems like server code calling the client version of a function.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Get the Half-Life directory and mod name from the install path
cmake_path(HAS_PARENT_PATH CMAKE_INSTALL_PREFIX HAS_HALFLIFE_DIRECTORY)
cmake_path(HAS_STEM CMAKE_INSTALL_PREFIX HAS_MOD_NAME)

if (NOT HAS_HALFLIFE_DIRECTORY OR NOT HAS_MOD_NAME)
	message(FATAL_ERROR "The install path must point to a mod directory")
endif()

cmake_path(GET CMAKE_INSTALL_PREFIX PARENT_PATH HALFLIFE_DIRECTORY)
cmake_path(GET CMAKE_INSTALL_PREFIX STEM MOD_NAME)

# All Half-Life specific commands are prefixed with HalfLife so they show up in the same group in the CMake GUI
set(HalfLife_HLDS_DIRECTORY "" CACHE PATH "Path to the Half-Life Dedicated Server directory. Must be specified to automatically set up dedicated server debugging settings and to copy libraries")

option(HalfLife_LTO "Enable Link-Time Optimization" OFF)

if(HalfLife_LTO)
	check_ipo_supported()
endif()

# Sets properties common to all projects
function(set_common_properties TARGET_NAME)
	set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

	# Enable use of Clang-Tidy in code analysis. See [repo_root]/.clang-tidy for the checks used in the project.
	set_target_properties(${TARGET_NAME} PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis true)

	if(HalfLife_LTO)
		set_target_properties(${TARGET_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()

	target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

	target_compile_definitions(${TARGET_NAME} PRIVATE
		_CRT_SECURE_NO_WARNINGS
		$<$<CONFIG:DEBUG>:_DEBUG>
		CLIENT_WEAPONS
		# These are defined for OSX as well
		$<$<PLATFORM_ID:Linux, Darwin>:POSIX _POSIX LINUX _LINUX GNUC>
		$<$<PLATFORM_ID:Darwin>:OSX _OSX>
		UNIFIED_SDK_CONFIG="$<CONFIG>")

	target_link_libraries(${TARGET_NAME} PRIVATE
		${CMAKE_DL_LIBS})

	target_include_directories(${TARGET_NAME} PRIVATE
		${CMAKE_BINARY_DIR})

	# TODO: review Linux compiler flags
	target_compile_options(${TARGET_NAME} PRIVATE
		# force 387 for FP math so the precision between win32 and linux and osx match
		# Note: the pentium-m arch setting is not used for AMD systems in the original makefile
		# Since the arch settings used are i686 this means including the setting ensures original settings are used,
		# but it could cause problems for AMD targets
		$<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-fpermissive -fno-strict-aliasing -Wno-invalid-offsetof -march=pentium-m -mfpmath=387>

		# flifetime-dse=1 is needed to disable a compiler optimization that optimizes out std::memset calls in CBaseEntity::operator new
		# See https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-flifetime-dse for more information about this flag
		# fno-gnu-unique is needed to disable a compiler optimization that prevents dlclose from unloading mod dlls,
		# causing them to retain state and crash when the engine tries to access memory in an invalid way
		# See https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html#index-fno-gnu-unique for more information about this flag
		$<$<CXX_COMPILER_ID:GNU>:-flifetime-dse=1 -fno-gnu-unique>

		# Can't set warning level to W4 until the compiler can deal with std::visit containing static_assert, which currently causes false positive "Unreachable code" warnings
		$<$<CXX_COMPILER_ID:MSVC>:/W3 /MP>

		# These are all from the original Makefile
		# They have not been tested with CMake since there is no Mac system available to test on, so this might not work
		$<$<PLATFORM_ID:Darwin>:-Qunused-arguments -mmacosx-version-min=10.5 -fasm-blocks -arch i386 -march=prescott -momit-leaf-frame-pointer -mtune=core2>)

	# Disable certain compiler warnings.
	if (MSVC)
		target_compile_options(${TARGET_NAME} PRIVATE
			# int or float down-conversion
			/wd4244
			# int or float data truncation
			/wd4305
			# unreferenced inline function removed
			/wd4514
			# unreferenced formal parameter
			/wd4100
			# Variable is uninitialized
			/wd26495
			# Arithmetic overflow
			/wd26451
			# The enum type is unscoped
			/wd26812)
	endif()

	target_link_options(${TARGET_NAME} PRIVATE
		$<$<PLATFORM_ID:Linux>:-static-libstdc++ -Wl,-Map,${TARGET_NAME}_map.txt -Wl,--no-undefined>)
endfunction()

add_subdirectory(cl_dll)
add_subdirectory(dlls)

# Set Visual Studio starting project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT client)

if (MSVC)
	set(HL_EXECUTABLE "hl.exe")
	set(HLDS_EXECUTABLE "hlds.exe")

	set(HalfLife_DEVELOPER_LEVEL "0" CACHE STRING "When launching from Visual Studio, the developer level is set to this value")
	set_property(CACHE HalfLife_DEVELOPER_LEVEL PROPERTY STRINGS 0 1 2 3)

	option(HalfLife_ENABLE_CHEATS "When launching from Visual Studio, enable cheats as well" OFF)
	option(HalfLife_ENABLE_CONSOLE "When launching from Visual Studio, enable the console on startup" OFF)
	option(HalfLife_ENABLE_DEV_MODE "When launching from Visual Studio, enables developer mode. The engine will enable the console on startup, sets developer to 1 during startup and sets sv_cheats to 1" OFF)

	set(HalfLife_ADDITIONAL_COMMAND_ARGUMENTS "" CACHE STRING "When launching from Visual Studio, additional command line arguments to add")

	set(COMMAND_ARGUMENTS "-game ${MOD_NAME} +developer ${HalfLife_DEVELOPER_LEVEL}")

	if (HalfLife_ENABLE_CHEATS)
		set(COMMAND_ARGUMENTS "${COMMAND_ARGUMENTS} +sv_cheats 1")
	endif()

	if (HalfLife_ENABLE_CONSOLE)
		set(COMMAND_ARGUMENTS "${COMMAND_ARGUMENTS} -console")
	endif()

	if (HalfLife_ENABLE_DEV_MODE)
		set(COMMAND_ARGUMENTS "${COMMAND_ARGUMENTS} -dev")
	endif()

	set(COMMAND_ARGUMENTS "${COMMAND_ARGUMENTS} ${HalfLife_ADDITIONAL_COMMAND_ARGUMENTS}")

	# Set debugging settings to work on the mod directory
	set_target_properties(client PROPERTIES
		VS_DEBUGGER_COMMAND "${HALFLIFE_DIRECTORY}/${HL_EXECUTABLE}"
		VS_DEBUGGER_COMMAND_ARGUMENTS "${COMMAND_ARGUMENTS}"
		VS_DEBUGGER_WORKING_DIRECTORY "${HALFLIFE_DIRECTORY}")

	# If HalfLife_HLDS_DIRECTORY is not specified it will leave a non-functional "default" setting
	set_target_properties(hl PROPERTIES
		VS_DEBUGGER_COMMAND "${HalfLife_HLDS_DIRECTORY}/${HLDS_EXECUTABLE}"
		VS_DEBUGGER_COMMAND_ARGUMENTS "${COMMAND_ARGUMENTS}"
		VS_DEBUGGER_WORKING_DIRECTORY "${HalfLife_HLDS_DIRECTORY}")
endif()

# Set install paths to the user-defined mod directory
install(TARGETS client
	RUNTIME DESTINATION cl_dlls
	LIBRARY DESTINATION cl_dlls
)

install(TARGETS hl
	RUNTIME DESTINATION dlls
	LIBRARY DESTINATION dlls
)

install(FILES 
	network/delta.lst
	DESTINATION .)

if (MSVC)
	install(FILES $<TARGET_PDB_FILE:client> DESTINATION cl_dlls OPTIONAL)
	install(FILES $<TARGET_PDB_FILE:hl> DESTINATION dlls OPTIONAL)
endif()

# Set up the script that will copy the mod libraries to the HLDS directory
if (NOT ${HalfLife_HLDS_DIRECTORY} STREQUAL "")
	set(hldsInstallScriptFile ${CMAKE_CURRENT_BINARY_DIR}/InstallToHLDS.cmake)
	configure_file(cmake/InstallToHLDS.cmake.in ${hldsInstallScriptFile} @ONLY)
	install(SCRIPT ${hldsInstallScriptFile})
endif()
