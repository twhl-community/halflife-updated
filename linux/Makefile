#
# Half-life Makefile for x86 Linux
#
#

OS:=$(shell uname)
HOSTNAME:=$(shell hostname)

ifeq "$(CFG)" ""
	CFG=release
endif

SOURCE_DIR=..
BUILD_DIR=$(CFG)

ENGINE_SRC_DIR=$(SOURCE_DIR)/engine
COMMON_SRC_DIR=$(SOURCE_DIR)/common
PM_SRC_DIR=$(SOURCE_DIR)/pm_shared
GAME_SHARED_SRC_DIR=$(SOURCE_DIR)/game_shared
PUBLIC_SRC_DIR=$(SOURCE_DIR)/public

BUILD_OBJ_DIR=$(BUILD_DIR)/obj

ARCH=i386
ELF-GC-DYNSTR=./elf-gc-dynstr

ifeq "$(COMPILER)" ""
	COMPILER=g++
endif

CPLUS="$(COMPILER) -m32"
CPP_LIB:=-L$(shell g++ -m32 -print-file-name=libstdc++.so | xargs dirname) -lstdc++ -ldl -lpthread -static-libstdc++ -static-libgcc

ARCH_CFLAGS=-march=pentium-m -mfpmath=387 -mno-sse

ifeq "$(CFG)" "release"
	ARCH_CFLAGS+=-O3 
else
	ARCH_CFLAGS+=-gdwarf-2 -g2 
endif

BASE_CFLAGS=-std=c++17 -fpermissive -fno-strict-aliasing -fno-exceptions -fexpensive-optimizations -Werror=return-type -w
BASE_CFLAGS+=-DNDEBUG -DPOSIX -D_POSIX -DLINUX -D_LINUX -DGNUC -DNO_MALLOC_OVERRIDE -DCLIENT_WEAPONS

# flifetime-dse=1 is needed to disable a compiler optimization that optimizes out std::memset calls in CBaseEntity::operator new
# See https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-flifetime-dse for more information about this flag
# fno-gnu-unique is needed to disable a compiler optimization that prevents dlclose from unloading mod dlls,
# causing them to retain state and crash when the engine tries to access memory in an invalid way
# See https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html#index-fno-gnu-unique for more information about this flag
ifeq "$(COMPILER)" "g++"
	BASE_CFLAGS+=-flifetime-dse=1 -fno-gnu-unique
endif

SHLIBEXT=so
SHLIBCFLAGS=
ifeq "$(CFG)" "release"
	SHLIBLDFLAGS="-shared -Wl,-Map,$@_map.txt -Wl,--no-undefined"
else
	SHLIBLDFLAGS="-shared -gdwarf-2 -g2 -Wl,-Map,$@_map.txt -Wl,--no-undefined"
endif

AR=ar
LIBEXT=a
MAKE+= -j8

MAKE_HL_LIB=$(MAKE) -f Makefile.hldll
MAKE_HL_CDLL=$(MAKE) -f Makefile.hl_cdll

#############################################################################
# SETUP AND BUILD
#############################################################################

all: build_dir targets

TARGETS=

TARGETS+= \
 	hl \
	hl_cdll \

build_dir:
	-mkdir -p $(BUILD_DIR);
	cd $(BUILD_DIR)

targets: $(TARGETS)

hl_cdll: build_dir
	$(MAKE_HL_CDLL) CPLUS=$(CPLUS) ARCH=$(ARCH) ARCH_CFLAGS="$(ARCH_CFLAGS)" SHLIBEXT=$(SHLIBEXT) SHLIBCFLAGS=$(SHLIBCFLAGS) SHLIBLDFLAGS=$(SHLIBLDFLAGS) CPP_LIB="$(CPP_LIB)" CFG=$(CFG) OS=$(OS) BASE_CFLAGS="$(BASE_CFLAGS)" BUILD_DIR=$(BUILD_DIR) BUILD_OBJ_DIR=$(BUILD_OBJ_DIR) SOURCE_DIR=$(SOURCE_DIR) ENGINE_SRC_DIR=$(ENGINE_SRC_DIR) COMMON_SRC_DIR=$(COMMON_SRC_DIR) PUBLIC_SRC_DIR=$(PUBLIC_SRC_DIR) GAME_SHARED_SRC_DIR=$(GAME_SHARED_SRC_DIR) PM_SRC_DIR=$(PM_SRC_DIR)

hl: build_dir
	$(MAKE_HL_LIB) CPLUS=$(CPLUS) ARCH=$(ARCH) ARCH_CFLAGS="$(ARCH_CFLAGS)" SHLIBEXT=$(SHLIBEXT) SHLIBCFLAGS=$(SHLIBCFLAGS) SHLIBLDFLAGS=$(SHLIBLDFLAGS) CPP_LIB="$(CPP_LIB)" CFG=$(CFG) OS=$(OS) BASE_CFLAGS="$(BASE_CFLAGS)" BUILD_DIR=$(BUILD_DIR) BUILD_OBJ_DIR=$(BUILD_OBJ_DIR) SOURCE_DIR=$(SOURCE_DIR) ENGINE_SRC_DIR=$(ENGINE_SRC_DIR) COMMON_SRC_DIR=$(COMMON_SRC_DIR) PUBLIC_SRC_DIR=$(PUBLIC_SRC_DIR) GAME_SHARED_SRC_DIR=$(GAME_SHARED_SRC_DIR) PM_SRC_DIR=$(PM_SRC_DIR)

clean:
	-rm -rf $(BUILD_OBJ_DIR)
