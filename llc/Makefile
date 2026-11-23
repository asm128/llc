FRAMEBUFFER		?= 0
Platform		?= Linux.x64
ProjectName		:= llc
Configuration	:= Release

C_DEFS			:= -DNOMINMAX
PLATFORM_C_DEFS	:= -DLLC_DEBUG_ENABLED -DLLC_XCB
OPT				:= 3

ifeq ($(FRAMEBUFFER), 1)
	Platform		:= LinuxFB.x64
	PLATFORM_C_DEFS	:= -DLLC_DEBUG_ENABLED -DLLC_FRAMEBUFFER
endif

ifeq ($(DEBUG), 1)
	Configuration	:= Debug
	OPT				:= 0
	C_DEFS			+= -DDEBUG
endif

OutDir	:= ../$(Platform).$(Configuration)
IntDir	:= ../obj/$(Platform).$(Configuration)/$(ProjectName)

OBJDIRS	:= $(patsubst %, $(IntDir), $(ProjectName))
INCDIR	:= $(patsubst %,-I%,$(IncDir))

CFLAGS	:= -std=c++20 -Wall -Werror -O$(OPT) $(INCDIR) $(C_DEFS)
CC		:= g++

_OBJ	:= $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJ		:= $(patsubst %,$(IntDir)/%,$(_OBJ))

all: build

build: $(OutDir)/$(ProjectName).a

linux: build

esp32:
	$(MAKE) build Platform=esp32 FRAMEBUFFER=0 PLATFORM_C_DEFS=-DLLC_DEBUG_ENABLED

$(OutDir)/$(ProjectName).a: $(OBJDIRS) $(OBJ)
	mkdir -p $(OutDir)
	ar rcs $@ $(OBJ)

$(IntDir)/%.o: %.cpp
	$(CC) $(CFLAGS) $(PLATFORM_C_DEFS) -c -o $@ $<

$(OBJDIRS):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(IntDir)
