#!/bin/sh

APP=urxvt

$APP -e vim -p Makefile main.c engine/world.h engine/world.c engine/person.h engine/person.c engine/car.h engine/car.c &
$APP -e vim -p engine/city.h engine/city.c engine/building.h engine/building.c engine/buildingtype.h engine/buildingtype.c &
vim -p ui/ui.h ui/ui.c ui/background.h ui/background.c ui/resources.h ui/resources.c
