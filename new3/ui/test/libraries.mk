# system libraries
LUA = $(shell pkg-config --cflags lua > /dev/null 2> /dev/null; echo $$?)
#SDL2   = $(shell sdl2-config --cflags > /dev/null 2> /dev/null; echo $$?)
#CONFIG = $(shell pkg-config --cflags libconfig > /dev/null 2> /dev/null; echo $$?)

# SDL libraries
#ifeq (${SDL2},0)
#  CFLAGS += `sdl2-config --cflags` -D_SDL2
#  LDFLAGS += `sdl2-config --libs` -lSDL2_image
#else
#  $(error The SDL2 library was not detected in the system. Please install it and try again)
#endif

# libconfig library
#ifeq (${CONFIG},0)
#  CFLAGS += `pkg-config --cflags libconfig` -DLIBCONFIG
#  LDFLAGS += `pkg-config --libs libconfig`
#else
#  $(warning IMPORTANT: The libconfig library was not detected in the system. The installation will continue but the audible beep in the terminal will not be avaliable)
#endif

# lua library
ifeq (${LUA},0)
  CFLAGS += `pkg-config --cflags lua` -DLUA
  LDFLAGS += `pkg-config --libs lua`
else
  $(error IMPORTANT: The lua library was not detected in the system. Please install it and try again)
endif
