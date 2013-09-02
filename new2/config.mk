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

# paths
PREFIX = /usr/local
DATAPREFIX = ${PREFIX}/share/newhope

# basic flags
CFLAGS = -DVERSION=\"${VERSION}\" -DDATADIR=\"${VINTERMPREFIX}\" -pedantic -Wall -I. -I/usr/include -std=c99
LDFLAGS = -L/usr/lib

# Duma libraries
ifeq (${DUMA}, yes)
  LDFLAGS += -lduma
endif

ifeq (${DEBUG},yes)
  # debug flags
  CFLAGS += -g -O0 -DDEBUG
  LDFLAGS += -g
else
  # production flags
  CFLAGS += -Os
  LDFLAGS += -s
endif

# compiler and linker
CC = gcc
#CC = clang