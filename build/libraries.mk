# system libraries
SDL2 = $(shell sdl2-config --cflags > /dev/null 2> /dev/null; echo $$?)

# SDL libraries
ifeq (${SDL2},0)
  CPPFLAGS += `sdl2-config --cflags` -D_SDL2
  LDFLAGS += `sdl2-config --libs` -lSDL2_ttf
else
  $(error The SDL2 library was not detected in the system. Please install it and try again)
endif

# chipmkunk library
CPPFLAGS += -isystem/usr/include/chipmunk
LDFLAGS += -lchipmunk -lm
