# New Hope
# See LICENSE file for copyright and license details.

#
# Please make your configuration changes in the `config.mk` file.
# 

include config.mk
ifneq ($(OS), Windows_NT)
  include libraries.mk
else
  include libraries_win.mk
endif

SRC = main.cc			\
      engine/block.cc		\
      engine/building.cc	\
      engine/buildinglayout.cc  \
      engine/city.cc		\
      engine/citylayout.cc	\
      engine/command.cc		\
      engine/hero.cc		\
      engine/person.cc		\
      engine/vehicle.cc		\
      engine/world.cc		\
      ui/w/wireframeui.cc

OBJ = ${SRC:.cc=.o}
HEADERS = ${SRC:.cc=.h} ui/ui.h engine/point.h engine/rectangle.h
DIST = 
DATA = data/PressStart2P.ttf

LINT_IGNORE=-whitespace,-runtime/references

all: options newhope

options:
	@echo
	@echo newhope build options:
	@echo "CPPFLAGS = ${CPPFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CPP       = ${CPP}"

.cc.o:
	@echo CPP $<
	@${CPP} -c ${CPPFLAGS} -o $@ $<
	@${CPP} -MM ${CPPFLAGS} $*.cc > $*.d

${OBJ}: config.mk libraries.mk

-include ${OBJ:.o=.d}

newhope: ${OBJ}
	@echo CPP -o $@
	@${CPP} -o $@ ${OBJ} ${LDFLAGS}

docs: doc/classes.txt
	@echo plantuml $<
	@java -jar doc/plantuml.jar $<

lint:
	cpplint --filter=${LINT_IGNORE} ${HEADERS} ${SRC}

check:
	cppcheck --include=all --inconclusive ${HEADERS} ${SRC}

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
