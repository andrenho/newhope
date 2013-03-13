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
#include "ui/dialog/elements.h"
#include "ui/dialog/dialog.h"
#include "ui/dialog/textwall.h"
#include "ui/image.h"
#include "ui/imageindexed.h"
#include "ui/imagergb.h"
#include "ui/imageset.h"
#include "ui/layer.h"
#include "ui/layerterrain.h"
#include "ui/layerdialog.h"
#include "ui/ui.h"
#include "ui/uigame.h"
#include "editor/editor.h"
#include "editor/uieditor.h"

extern GameMode* game;
extern UI*       ui;

#endif
