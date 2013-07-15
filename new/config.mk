#
# CUSTOMIZABLE SECTION
# 

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
NEWHOPEPREFIX = ${PREFIX}/share/newhope
LOCALEPREFIX = /usr/share/locale

# libraries
GLFW = yes
OPENGL = yes
SDL = no

# compiler and linker
CC = g++
#CC = clang++

# parse translations
GETTEXT = no

# add debugging info
DEBUG = yes
PROFILING = no
DUMA = no

#########################################################################
#
# PLEASE AVOID CHANGING THE INFORMATION BELOW, UNLESS NECESSARY
#

# New Hope version
VERSION = 0.0.2

# system libraries
PNG = yes
USE_CPP_THREADS = yes

# basic flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -DDATADIR=\"${NEWHOPEPREFIX}\" -Wall \
	   -Wfatal-errors -I. -I/usr/include -std=c++11
LDFLAGS = -L/usr/lib -lm -lpthread -ldl

UNAME = $(shell uname -o)

# GLFW library
ifeq (${GLFW},yes)
  CPPFLAGS += `pkg-config --cflags glfw3`
  LDFLAGS  += `pkg-config --libs   glfw3`
endif

# OPENGL library
ifeq (${OPENGL},yes)
  CPPFLAGS += `pkg-config --cflags gl glu`
  LDFLAGS  += `pkg-config --libs   gl glu`
endif

# PNG library
ifeq (${PNG},yes)
  CPPFLAGS += `pkg-config --cflags libpng`
  LDFLAGS += `pkg-config --libs libpng` -lz
endif

# gettext
ifeq (${GETTEXT},yes)
  CPPFLAGS += -DI18N -DLOCALEDIR=\"${LOCALEPREFIX}\"
  ifeq (${UNAME},Msys)
    LDFLAGS += -lintl
  endif
endif

# Duma libraries
ifeq (${DUMA}, yes)
  LDFLAGS += -lduma
endif

# debugging information
ifeq (${DEBUG},yes)
  # debug flags
  CPPFLAGS += -g -O0 -DDEBUG
  LDFLAGS += -g
else
  # production flags
  CPPFLAGS += -Os -DNDEBUG
  LDFLAGS += -s
endif

# profiling information
ifeq (${PROFILING},yes)
  CPPFLAGS += -pg
  LDFLAGS += -pg
endif

# SDL libraries
ifeq (${SDL},yes)
  CPPFLAGS += `sdl-config --cflags` -D_SDL
  # if you want the console window on Windows, edit the sdl-config script
  # and remove the '-mwindows' option
  LDFLAGS += `sdl-config --libs` -lSDL_ttf
endif

# use C++ threads
ifeq (${USE_CPP_THREADS},yes)
  ifeq (${CC},clang++)
    USE_CPP_THREADS = no
  else
    CPPFLAGS += -DUSE_CPP_THREADS
  endif
endif
