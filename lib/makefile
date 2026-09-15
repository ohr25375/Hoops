# compiler
CC = g++
# option
CFLAGS = -O3 -std=c++20
# targets
TARGETS = $(patsubst src/%.cpp, obj/%.o, $(wildcard src/*.cpp) $(wildcard src/**/*.cpp))
# source
SRCS = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
# object
OBJS = $(wildcard obj/*.o) $(wildcard obj/**/*.o)

# includes
INCDIR = -I/opt/homebrew/include/SDL2/

# libdir
LIBDIR = -L/opt/homebrew/lib/
# libs
LIBS = -lSDL2 -lSDL2_gfx -lSDL2_ttf



# obj
obj/%.o : src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/$*.cpp $(LIBDIR) $(LIBS) -o obj/$*.o

make: $(TARGETS) makefile

# clean
clean:
	-rm -f $(OBJS) *.d
	make