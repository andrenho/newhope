#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <map>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#include "options.h"
#include "game/map.h"
#include "game/gamemode.h"
#include "game/game.h"
#include "game/editor.h"
#include "ui/image.h"
#include "ui/imageindexed.h"
#include "ui/imagergb.h"
#include "ui/imageset.h"
#include "ui/layer.h"
#include "ui/layerterrain.h"
#include "ui/ui.h"
#include "ui/uieditor.h"
#include "ui/uigame.h"

extern GameMode* game;
extern UI*       ui;

#endif
