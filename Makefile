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
      engine/buildinglayout.cc 	\
      engine/city.cc		\
      engine/citylayout.cc	\
      engine/command.cc		\
      engine/hero.cc		\
      engine/resources.cc	\
      engine/person.cc		\
      engine/vehicle.cc		\
      engine/worker.cc		\
      engine/world.cc		\
      engine/mapgen/mapgen.cc	\
      engine/mapgen/seggen.cc	\
      engine/mapgen/roadgen.cc	\
      engine/mapgen/rivergen.cc	\
      engine/npc/banker.cc	\
      engine/npc/bartender.cc	\
      engine/npc/cardealer.cc	\
      engine/npc/dispatcher.cc	\
      engine/npc/medic.cc	\
      engine/npc/ordinary.cc	\
      engine/npc/secretary.cc	\
      engine/npc/smuggler.cc	\
      engine/npc/shopkeeper.cc 	\
      ui/dialogmanager.cc	\
      ui/minimap.cc		\
      ui/w/wdialogmanager.cc 	\
      ui/w/wireframeui.cc	\
      ui/w/wminimap.cc		\
      util/stdio.cc

OBJ = ${SRC:.cc=.o}
HEADERS = ${SRC:.cc=.h} ui/ui.h engine/point.h engine/rectangle.h
DIST = 
DATA = data/PressStart2P.ttf

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
	${CPP} -o $@ ${OBJ} ${LDFLAGS}

docs: doc/newhope.uml
	@echo plantuml $<
	@java -jar doc/plantuml.jar $<

lint:
	cpplint --filter=${LINT_IGNORE} ${HEADERS} ${SRC}

check:
	cppcheck --enable=all --inconclusive ${HEADERS} ${SRC}

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

create_translation_file:
	xgettext -d newhope -C -o i18n/newhope.po --keyword=_ ${SRC} ${HEADERS}
	msginit -l pt_BR.utf8 -o i18n/pt.po -i i18n/newhope.po

translation:
	xgettext -d newhope -C -j -o i18n/newhope.po --keyword=_ ${SRC} ${HEADERS}
	msgmerge i18n/pt.po i18n/newhope.po -o i18n/pt.po
	msgfmt -c -o i18n/pt_BR/LC_MESSAGES/newhope.mo i18n/pt.po

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
