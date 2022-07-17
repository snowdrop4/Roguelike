## -------------------------------------------------------------------------- ##
## Notes
## -----
#
# This makefile will automatically build all the .cpp files in each directory
# set in `CPP_FILES`. After adding a directory to `CPP_FILES` a directory
# of the same name should be added under `obj/` and that directory should be
# cleaned in the `clean:` command.
#

## -------------------------------------------------------------------------- ##
## Compiler Vars
## -------------
GTK_CC_FLAGS := $(shell echo `pkg-config --cflags gtk+-3.0`)
GTK_LD_FLAGS := $(shell echo `pkg-config --libs gtk+-3.0`)

SDL_CC_FLAGS := 
SDL_LD_FLAGS := -lSDL -lSDL_image -lSDL_gfx

OTHER_LD_FLAGS := -lpthread

CC := g++
CC_FLAGS := -g -O0 -pedantic -Wall -Werror -Weffc++ -Wno-deprecated-declarations -std=c++20 $(GTK_CC_FLAGS)
LD_FLAGS := $(SDL_LD_FLAGS) $(GTK_LD_FLAGS) $(OTHER_LD_FLAGS)

CPP_FILES := $(wildcard *.cpp) $(wildcard MapTile/*.cpp)
OBJ_FILES := $(patsubst %.cpp,obj/%.o,$(CPP_FILES))

## -------------------------------------------------------------------------- ##
## Build
## -----
obj/%.o : %.cpp %.hpp
	@echo Compiling $<
	@$(CC) $(CC_FLAGS) -c -o $@ $<

main : $(OBJ_FILES)
	@echo Linking
	@$(CC) $^ -o $@ $(LD_FLAGS)

debug:
	@echo Debugging ./main
	@ddd ./main 2> /dev/null

run:
	@echo Running ./main
	@./main

clean:
	@echo Cleaned
	@rm -f obj/*.o
	@rm -f obj/MapTile/*.o
