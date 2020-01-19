# OBJ (.o) files will be compiled -c first 
# then the linker will be called -o to link th files

#OBJS specifies which files to compile as part of the project
OBJS = main.o LTexture.o Entity.o Tile.o

#CC specifies which compiler we're using
CC = g++

#CFLAGS additional compilation options e.g w - suppresses all warnings
CFLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

SOURCE = .

# Executable name
OBJ_NAME = run

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(OBJ_NAME) $(OBJS) $(LFLAGS)

main.o: main.cpp def.h
	$(CC) -c $(SOURCE)/main.cpp

LTexture.o: LTexture.cpp def.h
	$(CC) -c $(SOURCE)/LTexture.cpp 

Entity.o: Entity.cpp def.h
	$(CC) -c $(SOURCE)/Entity.cpp

Tile.o: Tile.cpp def.h
	$(CC) -c $(SOURCE)/Tile.cpp

#This is the target that compiles our executable
clean:
	rm $(OBJ_NAME) $(OBJS)
