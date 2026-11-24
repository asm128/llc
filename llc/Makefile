Platform		?= Linux.x64
ProjectName		:= llc
Configuration	:= Release
ExtraIncDir		?=
ExcludeSrc		?=
DefaultExcludeSrc	:= llc_test_core.cpp

C_DEFS			:= -DNOMINMAX -DLLC_DEBUG_ENABLED -DLLC_XCB
OPT				:= 3

ifeq ($(DEBUG), 1)
	Configuration	:= Debug
	OPT				:= 0
	C_DEFS			+= -DDEBUG
endif

OutDir	:= ../$(Platform).$(Configuration)
IntDir	:= ../obj/$(Platform).$(Configuration)/$(ProjectName)

OBJDIRS	:= $(patsubst %, $(IntDir), $(ProjectName))
INCDIR	:= $(patsubst %,-I%,$(ExtraIncDir))

CFLAGS	?= -std=c++20 -Wall -O$(OPT) $(INCDIR) $(C_DEFS)
CC		:= g++

_SRC	:= $(filter-out $(DefaultExcludeSrc) $(ExcludeSrc),$(wildcard *.cpp))
_OBJ	:= $(patsubst %.cpp,%.o,$(_SRC))
OBJ		:= $(patsubst %,$(IntDir)/%,$(_OBJ))

all: build

build: $(OutDir)/$(ProjectName).a

linux: build

$(OutDir)/$(ProjectName).a: $(OBJDIRS) $(OBJ)
	mkdir -p $(OutDir)
	ar rcs $@ $(OBJ)

$(IntDir)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIRS):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(IntDir)
