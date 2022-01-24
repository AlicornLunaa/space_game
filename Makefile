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
$(APPNAME): obj/main.o obj/engine.o obj/collisionbody.o obj/colliders.o obj/vehicle.o obj/planetmanager.o obj/planet.o obj/player.o obj/parallax.o obj/debugger.o obj/vec_math.o obj/panel.o obj/renderer.o obj/game.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Builds every object
obj/main.o: src/main.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/vehicle.o: src/objects/vehicle.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/player.o: src/objects/player.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/parallax.o: src/objects/parallax.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^
	
obj/planet.o: src/objects/planet.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/debugger.o: src/util/debugger.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/vec_math.o: src/util/vec_math.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/colliders.o: src/phys/colliders.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/collisionbody.o: src/phys/collisionbody.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/engine.o: src/phys/engine.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/panel.o: src/interface/panel.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/renderer.o: src/interface/renderer.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/planetmanager.o: src/app/planetManager.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

obj/game.o: src/app/game.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $^

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(APPNAME) $(OBJDIR)