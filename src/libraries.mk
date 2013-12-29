# system libraries
LUA  = $(shell pkg-config --cflags luajit > /dev/null 2> /dev/null; echo $$?)
SDL2 = $(shell sdl2-config --cflags > /dev/null 2> /dev/null; echo $$?)

# SDL libraries
ifeq (${SDL2},0)
  CFLAGS += `sdl2-config --cflags` -D_SDL2
  LDFLAGS += `sdl2-config --libs` -lSDL2_ttf
else
  $(error The SDL2 library was not detected in the system. Please install it and try again)
endif

# lua library
ifeq (${LUA},0)
  CFLAGS += `pkg-config --cflags luajit` -DLUA
  LDFLAGS += `pkg-config --libs luajit`
else
  $(error IMPORTANT: The lua library was not detected in the system. Please install it and try again)
endif

# chipmkunk library
CFLAGS += -I/usr/include/chipmunk
LDFLAGS += -lchipmunk -lm
