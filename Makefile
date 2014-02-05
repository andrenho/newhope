# New Hope
# See LICENSE file for copyright and license details.

#
# Please make your configuration changes in the `config.mk` file.
# 

include build/config.mk
ifneq ($(OS), Windows_NT)
  include build/libraries.mk
else
  include build/libraries_win.mk
endif

SRC = main.cc			\
      engine/block.cc		\
      engine/building.cc	\
      engine/buildinglayout.cc  \
      engine/city.cc		\
      engine/citylayout.cc	\
      engine/command.cc		\
      engine/hero.cc		\
      engine/mapgen.cc		\
      engine/rivergen.cc	\
      engine/person.cc		\
      engine/vehicle.cc		\
      engine/world.cc		\
      ui/minimap.cc		\
      ui/w/wireframeui.cc	\
      ui/w/wminimap.cc

OBJ = ${SRC:.cc=.o}
HEADERS = ${SRC:.cc=.h} ui/ui.h engine/point.h engine/rectangle.h
DIST = 
DATA = data/PressStart2P.ttf

LINT_IGNORE=-whitespace,-runtime/references,-legal/copyright,-readability/todo

all: options newhope

options:
	@echo
	@echo newhope build options:
	@echo "CPPFLAGS = ${CPPFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CPP       = ${CPP}"

-include ${OBJ:.o=.d}

.cc.o:
	@echo CPP $<
	@${CPP} -c ${CPPFLAGS} -o $@ $<
	@${CPP} -MM -MT $*.o ${CPPFLAGS} $*.cc > $*.d

${OBJ}: build/config.mk build/libraries.mk

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

findleaks:
	build/findleaks.sh

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
