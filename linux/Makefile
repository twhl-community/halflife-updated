
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


CPLUS="g++ -m32"
CPP_LIB:=-L$(shell g++ -m32 -print-file-name=libstdc++.so | xargs dirname) -lstdc++ -ldl -lpthread


CLINK=$(CPLUS)

ifeq "$(CFG)" "release"
	ARCH_CFLAGS_I486=-O3 
	ARCH_CFLAGS_I686=-O3 
	ARCH_CFLAGS_AMD=-O3 
	ARCH_CFLAGS_AMD64=-m64 -O3 
else
	ARCH_CFLAGS_I486=-gdwarf-2 -g2 
	ARCH_CFLAGS_I686=-gdwarf-2 -g2 
	ARCH_CFLAGS_AMD=-gdwarf-2 -g2 
	ARCH_CFLAGS_AMD64=-m64 -gdwarf-2 -g2 
endif

ARCH_CFLAGS_I486+=-march=pentium-m -mfpmath=387
ARCH_CFLAGS_I686+=-march=pentium-m -mfpmath=387
ARCH_CFLAGS_AMD+=-march=k6 -mfpmath=387

ARCH_CFLAGS="$(ARCH_CFLAGS_I486)"

BASE_CFLAGS=-std=c++17 -fpermissive -fno-strict-aliasing -DNDEBUG -DPOSIX -D_POSIX -DLINUX -D_LINUX -DGNUC -DNO_MALLOC_OVERRIDE -fno-exceptions -fexpensive-optimizations -Werror=return-type
BASE_CFLAGS+=-w

BASE_CFLAGS+= -DLINUX -D_LINUX

SHLIBEXT=so
SHLIBCFLAGS=
ifeq "$(CFG)" "release"
	SHLIBLDFLAGS="-shared -Wl,-Map,$@_map.txt"
else
	SHLIBLDFLAGS="-shared -gdwarf-2 -g2 -Wl,-Map,$@_map.txt"
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
	-mkdir $(BUILD_DIR);
	cd $(BUILD_DIR)

targets: $(TARGETS)


hl_cdll: build_dir
	$(MAKE_HL_CDLL) ARCH=i686 CPLUS=$(CPLUS) CPP_LIB="$(CPP_LIB)" BUILD_DIR=$(BUILD_DIR) BUILD_OBJ_DIR=$(BUILD_OBJ_DIR) SOURCE_DIR=$(SOURCE_DIR) ENGINE_SRC_DIR=$(ENGINE_SRC_DIR) COMMON_SRC_DIR=$(COMMON_SRC_DIR) BASE_CFLAGS="$(BASE_CFLAGS)" PUBLIC_SRC_DIR=$(PUBLIC_SRC_DIR) ARCH_CFLAGS="$(ARCH_CFLAGS_I686)" GAME_SHARED_SRC_DIR=$(GAME_SHARED_SRC_DIR) CLINK=$(CLINK) PM_SRC_DIR=$(PM_SRC_DIR) SHLIBEXT=$(SHLIBEXT) SHLIBCFLAGS=$(SHLIBCFLAGS) SHLIBLDFLAGS=$(SHLIBLDFLAGS) CFG=$(CFG) OS=$(OS)

hl: build_dir
	$(MAKE_HL_LIB) CPLUS=$(CPLUS) ARCH=$(ARCH) CPP_LIB="$(CPP_LIB)" BUILD_DIR=$(BUILD_DIR) BUILD_OBJ_DIR=$(BUILD_OBJ_DIR) SOURCE_DIR=$(SOURCE_DIR) ENGINE_SRC_DIR=$(ENGINE_SRC_DIR) COMMON_SRC_DIR=$(COMMON_SRC_DIR) PM_SRC_DIR=$(PM_SRC_DIR) GAME_SHARED_SRC_DIR=$(GAME_SHARED_SRC_DIR) BASE_CFLAGS="$(BASE_CFLAGS)" SHLIBEXT=$(SHLIBEXT) SHLIBCFLAGS=$(SHLIBCFLAGS) SHLIBLDFLAGS=$(SHLIBLDFLAGS) PUBLIC_SRC_DIR=$(PUBLIC_SRC_DIR) CFG=$(CFG) OS=$(OS) ARCH_CFLAGS="$(ARCH_CFLAGS_I686)" 

clean:
	-rm -rf $(BUILD_OBJ_DIR)
