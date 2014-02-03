#################################
#                               #
#  CONFIGURE YOUR CHOICES HERE  #
#                               #
#################################

# vinterm version
VERSION = 0.0.1

# add debugging info
DEBUG = yes
DUMA = no
PROFILING = no

# paths
PREFIX = /usr/local
#DATAPREFIX = ${PREFIX}/share/newhope
DATAPREFIX="data"

# basic flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -DDATADIR=\"${DATAPREFIX}\" -pedantic -Wall -I. -I/usr/include -std=c++11
LDFLAGS = -L/usr/lib

# Duma libraries
ifeq (${DUMA}, yes)
  LDFLAGS += -lduma
endif

ifeq (${DEBUG},yes)
  # debug flags
  CPPFLAGS += -g -O0 -DDEBUG
  LDFLAGS += -g
else
  # production flags
  CPPFLAGS += -Os -DNDEBUG
  LDFLAGS += -s
endif

ifeq (${PROFILING},yes)
  CPPFLAGS += -pg
  LDFLAGS += -pg
endif

# compiler and linker
#CPP = g++
CPP = clang++
