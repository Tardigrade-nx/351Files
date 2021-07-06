TARGET = 351Files

# DEVICE ?= RG351P
# DEVICE ?= RG351V
DEVICE ?= PC

START_PATH ?= "/"
RES_PATH ?= "./res"

ifeq ($(DEVICE),PC)
	CC = $(CXX)
	SDL2_CONFIG = sdl2-config
	START_PATH = $(PWD)
endif

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %cpp,%o,$(SRC))
COMPILER_FLAGS =  $(shell $(SDL2_CONFIG) --cflags) -Wall -pedantic -Wfatal-errors -DDEVICE_$(DEVICE) -DSTART_PATH=\"$(START_PATH)\" -DRES_PATH=\"$(RES_PATH)\" -DFIRMWARE_$(FIRMWARE)
LINKER_FLAGS = $(shell $(SDL2_CONFIG) --libs) -lSDL2_image -lSDL2_ttf

all : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LINKER_FLAGS)

%.o:%.cpp
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

clean :
	rm -f $(OBJ) $(TARGET)
