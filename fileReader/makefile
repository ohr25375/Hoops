# compiler
CC = g++
# option
CFLAGS = -O3 -std=c++20
# targets
TARGETS = $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp))
# source
SRCS = $(wildcard src/*.cpp)
# object
OBJS = $(wildcard obj/*.o)

# includes
INCDIR = -I/opt/homebrew/include/

# libdir
LIBDIR = -L/opt/homebrew/lib
# libs
LIBS = -lSDL2 -lSDL2_gfx -lSDL2_ttf



# obj
%.o : src/%.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $< $(LIBDIR) $(LIBS) -o obj/$@

# "make all"
all: $(TARGETS)
# clean
clean:
	rm -f $(OBJS) *.d
	make