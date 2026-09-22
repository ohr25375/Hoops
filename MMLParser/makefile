# compiler
CC = g++
# c version
CVERSION = -std=c++20
# compile flags
CFLAGS = -fdiagnostics-color=always
CLFAGS += -O3
CFLAGS += -g
# object include
COBJ = ../fileReader/obj/*.o


TARGETS = $(patsubst src/%.cpp, obj/%.o, $(wildcard src/*.cpp) $(wildcard src/**/*.cpp))
CAOBJ = $(wildcard obj/*.o) $(wildcard obj/**/*.o)
# includes
CINCDIR = -I/opt/homebrew/include/
CINCDIR = -I../fileReader/

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

obj/%.o : src/%.cpp src/%.hpp makefile
	$(CC) $(CFLAGS) $(CINCDIR) -c src/$*.cpp $(CLIBDIR) $(CVERSION) -o obj/$*.o

make: $(TARGETS)

clean:
	rm -f $(OBJS)
	rm -f $(TARGETS)
	rm -f main
	make
