@echo off
setlocal EnableDelayedExpansion

rem Set this path to your mod directory. You can use environment variables to avoid hardcoding the path if your team members have different install locations.
if NOT DEFINED HL_MOD_PATH (
	echo hl mod path not defined >&2
	exit /b 1
)

set mod_directory=%HL_MOD_PATH%
set "mod_directory=%mod_directory:\=/%"

rem Input paths may end with a backslash which will be interpreted as an escape character when passed into robocopy, so this needs to be escaped.
set source=%~1
set source=%source:\=\\%

set destination=%mod_directory%

if NOT "%~2" == "" (
	set destination=!destination!/%~2
)

set destination=!destination:\=\\!

set filename=%~3

set "options=/njh /njs /ndl /nc /ns /np"

if "%filename%" == "" (
    set "options=%options% /s"
)

echo Installing "!destination!/%filename%"

rem If you are having problems with this command, remove the redirect to nul part (> nul) to get error messages from robocopy.
@robocopy "%source%" "!destination!" "%filename%" %options% > nul

rem Needed because robocopy returns a non-zero code for success which makes Visual Studio treat this as failure.
exit /b 0
