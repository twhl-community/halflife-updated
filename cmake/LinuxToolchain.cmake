# the name of the target operating system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i386)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER gcc-11)
set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_COMPILER g++-11)
set(CMAKE_CXX_FLAGS "-m32 -static-libgcc -static-libstdc++ -D_GLIBCXX_USE_CXX11_ABI=0")
