IncDir			:=
Platform		:= Linux.x64
ProjectName		:= llc
Configuration	:= Debug

C_DEFS			:= -DNOMINMAX -DLLC_XCB -DLLC_DEBUG_ENABLED -DDEBUG
OPT				:= 0

ifeq ($(DEBUG), 0)
	Configuration	:= Release
	OPT				:= 3
	C_DEFS			:= -DNOMINMAX -DLLC_XCB
endif

OutDir	:= ../../$(Platform).$(Configuration)
IntDir	:= ../../obj/$(Platform).$(Configuration)/$(ProjectName)

OBJDIRS	:= $(IntDir)
INCDIR	:= $(patsubst %,-I%,$(IncDir))

CFLAGS	:= -std=c++20 -Wall -fpermissive -Wno-changes-meaning -Wno-invalid-constexpr -O$(OPT) $(INCDIR) -include stdio.h -include stdlib.h
CC		:= g++

_OBJ	:= $(filter-out llc_test_core.o,$(patsubst %.cpp,%.o,$(wildcard *.cpp)))
OBJ		:= $(patsubst %,$(IntDir)/%,$(_OBJ))

$(IntDir)/%.o: %.cpp
	$(CC) $(CFLAGS) $(C_DEFS) -c -o $@ $<

build: $(OBJDIRS) $(OBJ)
	mkdir -p $(OutDir)
	ar rcs $(OutDir)/$(ProjectName).a $(OBJ)

$(OBJDIRS):
	mkdir -p $@

.PHONY: build clean

clean:
	rm -rf $(IntDir)
