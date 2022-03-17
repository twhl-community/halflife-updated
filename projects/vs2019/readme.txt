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

Please note that this project requires the 3D Studio Max 4.2 SDK.
You might need to adjust Include and Additional Library Directories according
to your intallation in C++ and Linker settings.

The MAX 4.2 SDK needs adjustment:
Comment out the following line in max.h
#include <ctl3d.h>
So that it reads
//#include <ctl3d.h>
.

For C:\3dsmax42\maxsdk\Include\polyobj.h, line 109 should be changed from:
	for (i=0; i<mm.numv; i++) if (!mm.v[i].GetFlag (MN_DEAD)) numVerts++;
to:
	for (int i=0; i<mm.numv; i++) if (!mm.v[i].GetFlag (MN_DEAD)) numVerts++;
.

For C:\3dsmax42\maxsdk\Include\imtl.h, line 1390 should be added:
	int i;
and line 1392 (originally line 1391) should be change from:
	for( int i = 0; i < nUserIllumOut; ++i ){ 
to:
	for( i = 0; i < nUserIllumOut; ++i ){
.

You also need the phyexp.h from Character Studio, which you should place in
c:\3dsmax42\cstudio\sdk or adjust the Include Directories accordingly.

Someone should port that project to a newer MAX SDK Version, but that is
really beyond the current scope.