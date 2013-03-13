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
#include "ui/dialog/elements.h"
#include "ui/dialog/dialog.h"
#include "ui/dialog/dialoginput.h"
#include "ui/dialog/textwall.h"
#include "ui/image/image.h"
#include "ui/image/imageindexed.h"
#include "ui/image/imagergb.h"
#include "ui/imageset.h"
#include "ui/layer.h"
#include "ui/layerterrain.h"
#include "ui/layerdialog.h"
#include "ui/ui.h"
#include "ui/uigame.h"
#include "ui/editor/uieditor.h"

extern GameMode* game;
extern UI*       ui;

#endif
