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
