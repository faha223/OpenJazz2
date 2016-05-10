CXX = g++
FLAGS = -std=c++0x -g
LIBS = -lSDL2 -lGL -lGLU -lGLEW -lz -L/usr/local/lib -lboost_system -lboost_filesystem -lSDL2_mixer

OBJS = Controls.o FileIO.o FileSystem.o Level.o Math.o Music.o Player.o Tileset.o Animation.o Game.o Event.o Actor.o

driver : makefile $(OBJS) OpenJazz2/Driver.cpp OpenJazz2/Structures.h
	$(CXX) $(FLAGS) $(OBJS) OpenJazz2/Driver.cpp -o driver $(LIBS)

%.o : OpenJazz2/%.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean : 
	rm -f $(OBJS) driver
