# Vintage Terminal
# See LICENSE file for copyright and license details.

#
# Please make your configuration changes in the `config.mk` file.
# 

include src/config.mk
ifneq ($(OS), Windows_NT)
  include src/libraries.mk
else
  include src/libraries_win.mk
endif

SRC = src/main.c	\
      src/luah.c	\
      src/physics.c	\
      src/dynamicp.c    \
      src/minimap.c     \
      src/staticp.c     \
      src/ui.c		\
      src/vehicle.c	\
      src/wireframe.c

OBJ = ${SRC:.c=.o} src/block.h
HEADERS = ${SRC:.c=.h}
DIST = 
DATA = data/PressStart2P.ttf

all: options newhope

options:
	@echo
	@echo newhope build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} -o $@ $<
	@${CC} -MM ${CFLAGS} $*.c > $*.d

${OBJ}: src/config.mk src/libraries.mk

-include ${OBJ:.o=.d}

newhope: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

src/block.h: engine/block.lua
	@echo SH -o $@
	./engine/c_utils block > $@

docs: doc/classes.txt
	@echo plantuml $<
	@java -jar doc/plantuml.jar $<

clean:
	@echo cleaning
	@rm -f newhope ${OBJ} newhope-${VERSION}.tar.gz ${OBJ:.o=.d}

dist: clean
	@echo creating dist tarball
	@mkdir -p newhope-${VERSION}
	@cp --parents config.mk libraries.mk Makefile ${DIST} ${SRC} ${HEADERS} ${DATA} newhope-${VERSION}
	@tar -cf newhope-${VERSION}.tar newhope-${VERSION}
	@gzip newhope-${VERSION}.tar
	@rm -rf newhope-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@install -f newhope ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/newhope
	@echo installing data to ${DESTDIR}${VINTERMPREFIX}
	@mkdir -p ${DESTDIR}${VINTERMPREFIX}
	@install -f ${DATA} ${DESTDIR}${VINTERMPREFIX}

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/st
	@echo removing data from ${DESTDIR}${VINTERMPREFIX}
	@rm -f ${DESTDIR}${VINTERMPREFIX}/*
	@rmdir ${DESTDIR}${VINTERMPREFIX}

.PHONY: all options clean dist install uninstall
