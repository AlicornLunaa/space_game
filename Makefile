########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -g -std=c++17 -Wall -DSFML_STATIC -I./sfml/include -L./sfml/lib
LDFLAGS = -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lfreetype -lgdi32 -lopengl32 -lwinmm

# Makefile settings - Can be customized.
APPNAME = space_game
SRCDIR = src
OBJDIR = obj

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
# UNIX-based OS variables & settings
RM = rm -r
DELOBJ = $(OBJ)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: dirs $(APPNAME)

# Setup
dirs:
	mkdir -p $(OBJDIR)

# Builds the app
$(APPNAME): obj/main.o obj/vehicle.o obj/player.o obj/util.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Builds every object
obj/main.o: src/main.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/vehicle.o: src/objects/vehicle.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/player.o: src/objects/player.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/util.o: src/util/debugger.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(APPNAME) $(OBJDIR)