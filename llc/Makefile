                                                                                                                                                                                                                                                                                                                                                                                                                                                                     IncDir			:= ../zlib
Platform		:= esp32 # Linux.ARM
ProjectName		:= llc
Configuration	:= Release

C_DEFS			:= -DNOMINMAX
OPT				:= 3

ifeq ($(DEBUG), 1)
	Configuration	:= Debug
	OPT				:= 0
	C_DEFS			+= -DDEBUG
endif

OutDir	:= ../$(Platform).$(Configuration)
IntDir	:= ../obj/$(Platform).$(Configuration)/$(ProjectName)

OBJDIRS	:= $(patsubst %, $(IntDir), $(ProjectName))
INCDIR	:= $(patsubst %,-I%,$(IncDir))

CFLAGS	:= -std=c++17 -Wall -O$(OPT) $(INCDIR) $(C_DEFS)
CC		:= g++

_OBJ	:= $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJ		:= $(patsubst %,$(IntDir)/%,$(_OBJ))

# Platform-specific flags
C_DEFS_LINUX	:= -DLLC_DEBUG_ENABLED -DLLC_XCB
C_DEFS_ESP32	:= -DLLC_DEBUG_ENABLED

all: linux esp32

linux: PLATFORM_C_DEFS := $(C_DEFS_LINUX) # Platform-specific definitions
linux: $(OutDir)/$(ProjectName).a

esp32: PLATFORM_C_DEFS := $(C_DEFS_ESP32) # Platform-specific definitions
esp32: $(OutDir)/$(ProjectName).a

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
