TARGET = 351Files
# DEVICE=RG351P
# DEVICE=RG351V
CC = g++

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %cpp,%o,$(SRC))
COMPILER_FLAGS =  $(shell sdl2-config --cflags) -Wall -pedantic -Wfatal-errors -DDEVICE_$(DEVICE)
LINKER_FLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_gfx

all : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LINKER_FLAGS)

%.o:%.cpp
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

clean :
	rm -f $(OBJ) $(TARGET)
