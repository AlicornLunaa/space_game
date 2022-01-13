########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -DSFML_STATIC -I./sfml/include -L./sfml/lib
LDFLAGS = -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lfreetype -lgdi32 -lopengl32 -lwinmm

# Makefile settings - Can be customized.
APPNAME = space_game
SRCDIR = src
OBJDIR = obj

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): obj/main.o obj/objects.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Builds every object
obj/main.o: src/main.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/objects.o: src/objects/ship.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(APPNAME)