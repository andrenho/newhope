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
WARNINGS=-Wall -Weffc++ -pedantic  \
    -pedantic-errors -Wextra  -Wall -Wcast-align \
    -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion \
    -Wdisabled-optimization \
    -Wfloat-equal  -Wformat  -Wformat=2 \
    -Wformat-nonliteral -Wformat-security  \
    -Wformat-y2k \
    -Wimport  -Winit-self  -Winline \
    -Winvalid-pch   \
    -Wunsafe-loop-optimizations  -Wlong-long -Wmissing-braces \
    -Wmissing-field-initializers -Wmissing-format-attribute   \
    -Wmissing-include-dirs -Wmissing-noreturn \
    -Wpacked -Wparentheses  -Wpointer-arith \
    -Wredundant-decls -Wreturn-type \
    -Wsequence-point  -Wsign-compare  -Wstack-protector \
    -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default \
    -Wswitch-enum -Wtrigraphs  -Wuninitialized \
    -Wunknown-pragmas  -Wunreachable-code -Wunused \
    -Wunused-function  -Wunused-label  -Wunused-parameter \
    -Wunused-value  -Wunused-variable  -Wvariadic-macros \
    -Wvolatile-register-var  -Wwrite-strings
CPPFLAGS = -DVERSION=\"${VERSION}\" -DDATADIR=\"${DATAPREFIX}\" ${WARNINGS} -I. -I/usr/include -std=c++11
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
CPP = g++
#CPP = clang++
