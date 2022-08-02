Directory Contents
==================

projects.sln
  Solution file for Visual C++ 2019, containing the main projects for
  compiling the valve DLL files.
  
utils.sln
  Solution file containing projects for the utilities.
  Special user interaction is required in order to compile a share of the
  projects, see "Installing SDL2" bellow.
  Special user interaction is also required for the smdlexp project, see
  "smdlexp project" bellow.
  The serverctrl project is problematic, see "serverctrl project" bellow.
  Further project specific notes are also provided bellow.
  
[other required files]


Installing SDL2 library:
-------------------------

Required by: mdlviewer, qbsp2, qcsg

SDL2(specifically SDL 2.0.0) is required for these projects.
SDL2.lib contained in this repository is used to link with it, while the executables need the SDL2.dll file from the game installation in order to run. 


Other
=====

Please note, that some of the utils have additional documentation (.doc / .txt)
in the appropriate sub-folders of the utils folder.



qrad project
============

Please not that qrad.exe requires additional files lights.rad and valve.rad
side-by-side. You can find those files in the Half-Life SDK in the Hammer
tools folder.



serverctrl project
==================

The serverctrl project is problematic for the following reasons:
- It doesn't seem to work anymore, I tried with a recent HLDS beta installation
  from steamcmd (make sure to set the hardcoded path in ServerCtrlDlg.cpp line
  477, i.e. to "." (which would mean current directory) or s.th. you like).
  While the hlds.exe seems to know about the -HFILE -HCHILD and -HPARENT
  command line arguments used by serverctrl, it doesn't seem to handle the
  events properly anymore.
- It requires MFC to be installed and needs to be configured in order to find
  the MFC installation (see "Installing MFC" bellow).
  
A further note:
The serverctrl.vcxproj differs from the original .dsp project a bit:
It is compiled against dynamic libraries (MFC and Windows Runtime), meaning
/MD or /MDd instead of /MT or /MTd now. The reason is that the MFC from the
Windows Driver Kit won't compile in static mode (missing resource files i.e.).
This means you'll have to redistribute the runtime and MFC DLLs along with
serverctrl in case you install it on another system.


Installing MFC
--------------

Required by: serverctrl

Using Visual Studio Installer, install C++ MFC from "Individual Components".



smdlexp project
===============

Please note that this project requires the 3DS Max 2023 SDK.
3DS Max 2023 SDK can be downloaded from https://www.autodesk.com/developer-network/platform-technologies/3ds-max.
You might need to adjust Include and Additional Library Directories according
to your intallation in C++ and Linker settings.