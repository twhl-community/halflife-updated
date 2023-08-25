@echo off
setlocal EnableDelayedExpansion

rem Set this path to your mod directory. You can use environment variables to avoid hardcoding the path if your team members have different install locations.
set mod_directory=C:/Program Files (x86)/Steam/steamapps/common/Half-Life/halflife_updated

rem Input paths may end with a backslash which will be interpreted as an escape character when passed into robocopy, so this needs to be escaped.
set source=%~1
set source=%source:\=\\%

set destination=%mod_directory%

if NOT "%~2" == "" (
	set destination=!destination!/%~2
)

set destination=!destination:\=\\!

set filename=%~3

echo Installing "!destination!/%filename%"

rem If you are having problems with this command, remove the redirect to nul part (> nul) to get error messages from robocopy.
@robocopy "%source%" "!destination!" "%filename%" /njh /njs /ndl /nc /ns /np > nul

rem Needed because robocopy returns a non-zero code for success which makes Visual Studio treat this as failure.
exit /b 0
