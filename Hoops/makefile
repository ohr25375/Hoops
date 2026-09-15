# compiler
CC = g++
# c version
CVERSION = -std=c++20
# compile flags
CFLAGS = -fdiagnostics-color=always
CFLAGS += -g
CFLAGS += -Wall
# object include
COBJ = ../lib/obj/*.o
COBJ += ../lib/obj/**/*.o
COBJ += ../MMLParser/obj/*.o
COBJ += ../AudioStreamTest/obj/*.o

TARGETS = $(patsubst src/%.cpp, obj/%.o, $(wildcard src/*.cpp) $(wildcard src/**/*.cpp))
CAOBJ = $(wildcard obj/*.o) $(wildcard obj/**/*.o)
# includes
CINCDIR = -I/opt/homebrew/include/
CINCDIR += -I../lib
CINCDIR += -I../MMLParser/
CINCDIR += -I../AudioStreamTest/

# src dir
SRC_DIR = src
# src hpp
SRCS = $(wildcard src/**/*.cpp)
# obj dir
OBJ_DIR = obj
# obj
OBJS = $(wildcard obj/**/*.o)
# library includes
CLIBDIR = -L/opt/homebrew/lib
# sdl flags
SDLFLAGS = -D_THREAD_SAFE
SDLFLAGS += -lSDL2
SDLFLAGS += -lSDL2_gfx
SDLFLAGS += -lSDL2_image


# SDLFLAGS += -lSDL2_ttf

obj/%.o : src/%.cpp src/%.hpp makefile
	$(CC) $(CFLAGS) $(CINCDIR) -c src/$*.cpp $(CLIBDIR) $(SDLFLAGS) $(CVERSION) -o obj/$*.o

main: main.cpp main.hpp $(TARGETS) $(SRCS) makefile
	$(CC) $(CFLAGS) $(COBJ) $(CAOBJ) $(CINCDIR) $(CLIBDIR) $(SDLFLAGS) $(CVERSION) main.cpp -o main

leaks:
	leaks --atExit -- ./main

library:
	cd ../AudioStreamTest && make
	cd ../lib && make
	cd ../MMLParser && make
	cd ../fileReader && make

clean:
	rm -f $(OBJS)
	rm -f $(TARGETS)
	rm -f main
	make