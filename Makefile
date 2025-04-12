# Folders
SRC = src

TEST_SRC = tests

# SFMl directory - Change if neeeded
SFML_ROOT = C:\Users\micha\OneDrive\Documents\libraries\SFML-2.5.1

# this is the directory I will compile from (i.e. the argument to -I)
IDIR := include -I$(TEST_SRC)

# all object files will be put here
OBJDIR := build

# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS += -Wno-deprecated-declarations -Wall -Wextra -pedantic -std=c++17 -Weffc++ -isystem$(SFML_ROOT)/include

LDFLAGS += -L$(SFML_ROOT)/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# file which contains the main function
MAINFILE := main.cpp

# name of executable
EXE = play

TEST_EXE = test

# Object modules
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/game.o $(OBJDIR)/gamestate.o $(OBJDIR)/gameobject.o \
		  $(OBJDIR)/context.o $(OBJDIR)/normalmode.o $(OBJDIR)/mainmenu.o $(OBJDIR)/ui.o \
		  $(OBJDIR)/projectile.o $(OBJDIR)/resourcemanager.o $(OBJDIR)/gameconfiguration.o \
		  $(OBJDIR)/player.o $(OBJDIR)/ship.o $(OBJDIR)/enemy.o $(OBJDIR)/enemyminion.o \
		  $(OBJDIR)/pausemenu.o $(OBJDIR)/bossmode.o $(OBJDIR)/powerup.o $(OBJDIR)/endscreen.o \
		  $(OBJDIR)/enemyboss.o $(OBJDIR)/enemytank.o $(OBJDIR)/enemymover.o \
		  $(OBJDIR)/enemymultishot.o $(OBJDIR)/enemyboss2.o \

TEST_OBJECTS = $(OBJDIR)/test_main.o $(OBJDIR)/game.o $(OBJDIR)/gamestate.o $(OBJDIR)/gameobject.o \
  			   $(OBJDIR)/context.o $(OBJDIR)/normalmode.o $(OBJDIR)/mainmenu.o $(OBJDIR)/ui.o \
		  	   $(OBJDIR)/projectile.o $(OBJDIR)/resourcemanager.o $(OBJDIR)/gameconfiguration.o \
		  	   $(OBJDIR)/player.o $(OBJDIR)/ship.o $(OBJDIR)/enemy.o $(OBJDIR)/enemyminion.o \
		  	   $(OBJDIR)/pausemenu.o $(OBJDIR)/bossmode.o $(OBJDIR)/powerup.o $(OBJDIR)/endscreen.o \
		  	   $(OBJDIR)/enemyboss.o $(OBJDIR)/enemytank.o $(OBJDIR)/enemymover.o \
		  	   $(OBJDIR)/enemymultishot.o $(OBJDIR)/gamemode_test.o $(OBJDIR)/enemyboss2.o \

# Main objetice - created with 'make' or 'make main'.
main: $(OBJDIR) $(OBJECTS) Makefile
	$(CCC) -I$(IDIR) $(CCFLAGS) -o $(EXE) $(OBJECTS) $(LDFLAGS)

test: $(OBJDIR) $(TEST_OBJECTS) Makefile
	$(CCC) -I$(IDIR) -I$(TEST_SRC) $(CCFLAGS) -o $(TEST_EXE) $(TEST_OBJECTS) $(LDFLAGS)

# Part objectives
$(OBJDIR)/main.o: $(SRC)/main.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/game.o: $(SRC)/game.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/game.cpp -o $(OBJDIR)/game.o

$(OBJDIR)/gamestate.o: $(SRC)/gamestate.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/gamestate.cpp -o $(OBJDIR)/gamestate.o

$(OBJDIR)/gameobject.o: $(SRC)/gameobject.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/gameobject.cpp -o $(OBJDIR)/gameobject.o

$(OBJDIR)/context.o: $(SRC)/context.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/context.cpp -o $(OBJDIR)/context.o

$(OBJDIR)/ship.o: $(SRC)/ship.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/ship.cpp -o $(OBJDIR)/ship.o

$(OBJDIR)/enemy.o: $(SRC)/enemy.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemy.cpp -o $(OBJDIR)/enemy.o

$(OBJDIR)/enemyminion.o: $(SRC)/enemyminion.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemyminion.cpp -o $(OBJDIR)/enemyminion.o

$(OBJDIR)/player.o: $(SRC)/player.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/player.cpp -o $(OBJDIR)/player.o

$(OBJDIR)/normalmode.o: $(SRC)/normalmode.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/normalmode.cpp -o $(OBJDIR)/normalmode.o

$(OBJDIR)/mainmenu.o: $(SRC)/mainmenu.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/mainmenu.cpp -o $(OBJDIR)/mainmenu.o

$(OBJDIR)/ui.o: $(SRC)/ui.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/ui.cpp -o $(OBJDIR)/ui.o

$(OBJDIR)/projectile.o: $(SRC)/projectile.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/projectile.cpp -o $(OBJDIR)/projectile.o

$(OBJDIR)/resourcemanager.o: $(SRC)/resourcemanager.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/resourcemanager.cpp -o $(OBJDIR)/resourcemanager.o

$(OBJDIR)/gameconfiguration.o: $(SRC)/gameconfiguration.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/gameconfiguration.cpp -o $(OBJDIR)/gameconfiguration.o

$(OBJDIR)/pausemenu.o: $(SRC)/pausemenu.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/pausemenu.cpp -o $(OBJDIR)/pausemenu.o

$(OBJDIR)/powerup.o: $(SRC)/powerup.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/powerup.cpp -o $(OBJDIR)/powerup.o

$(OBJDIR)/bossmode.o: $(SRC)/bossmode.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/bossmode.cpp -o $(OBJDIR)/bossmode.o

$(OBJDIR)/endscreen.o: $(SRC)/endscreen.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/endscreen.cpp -o $(OBJDIR)/endscreen.o

$(OBJDIR)/enemyboss.o: $(SRC)/enemyboss.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemyboss.cpp -o $(OBJDIR)/enemyboss.o

$(OBJDIR)/enemyboss2.o: $(SRC)/enemyboss2.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemyboss2.cpp -o $(OBJDIR)/enemyboss2.o

$(OBJDIR)/enemytank.o: $(SRC)/enemytank.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemytank.cpp -o $(OBJDIR)/enemytank.o

$(OBJDIR)/enemymover.o: $(SRC)/enemymover.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemymover.cpp -o $(OBJDIR)/enemymover.o

$(OBJDIR)/enemymultishot.o: $(SRC)/enemymultishot.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/enemymultishot.cpp -o $(OBJDIR)/enemymultishot.o

$(OBJDIR)/test_main.o: $(TEST_SRC)/test_main.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(TEST_SRC)/test_main.cpp -o $(OBJDIR)/test_main.o

$(OBJDIR)/gamemode_test.o: $(TEST_SRC)/gamemode_test.cpp
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(TEST_SRC)/gamemode_test.cpp -o $(OBJDIR)/gamemode_test.o

# create OBJDIR directory
$(OBJDIR):
	mkdir $(OBJDIR)

# 'make clean' removes object files and memory dumps.
clean:
	@ \rm -rf $(OBJDIR)/*.o *.gch core

# 'make zap' also removes the executable and backup files.
zap: clean
	@ \rm -rf $(EXE) *~
