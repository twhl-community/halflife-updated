# Building this SDK

## Requirements

### Windows

* Windows 7 or newer
* Visual Studio 2019 or newer

### Linux

* GCC 9 or newer (C++17 support or newer required)
* Packages:
    * `g++-multilib`
    * `libgl1-mesa-dev` (or an equivalent package that provides the OpenGL headers)

## Building

The TWHL tutorial on setting up the Half-Life SDK for C++ mod development explains how to set up the source code and how to configure the mod installation: https://twhl.info/wiki/page/Half-Life_Programming_-_Getting_Started

## Packaging mod files

To package a mod for distribution as an archive, use the `CreatePackage` script:

```
cd path/to/Half-Life
dotnet script halflife_updated/scripts/packager/CreatePackage.csx --package-name <name of archive file excluding file extension>
```

## Building with CMake

**Note:** if you are a beginner and/or have no experience with CMake, please use Visual Studio projects/solution on Windows and Linux Makefiles instead.

Start by creating a `build` folder at the root of this repository. This folder is already git ignored so you don't have to update `.gitignore` yourself.

On Windows, make sure you specify the "Win32" architecture when invoking CMake.

On Linux, make sure you select the `cmake/LinuxToolchain.cmake` toolchain file when invoking CMake.

Specify the path to your Half-Life mod (typically `C:\Program Files (x86)\Steam\steamapps\common\Half-Life\mymod` on Windows) as CMake's install prefix (`CMAKE_INSTALL_PREFIX`). When invoking CMake's `INSTALL` target, this will install the build binaries and the required `delta.lst` changes for you.

If you are working on a multiplayer mod and want to test your binaries on a dedicated server installation (HLDS), you might want to do the same with the `HalfLife_HLDS_DIRECTORY` variable.

`HalfLife_LTO` is a toggle that defines if you want to enable "interprocedural optimization (IPO) / link-time optimization (LTO)" when building the binaries. If you're not sure, leave this disabled (if you want details, check [CMake's documentation](https://cmake.org/cmake/help/v3.23/module/CheckIPOSupported.html)).

Optional but highly recommended, set the others `HalfLife` variables, these are used as settings for your debugger thus providing you with more comfort.

Once these are configured, you can generate the `build` directory, open the project and build right away.
