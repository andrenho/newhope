#ifndef UI_W_WMINIMAP_H_
#define UI_W_WMINIMAP_H_

#include "ui/minimap.h"

class WMinimap : public Minimap {
public:
	WMinimap(int w, int h);
	virtual ~WMinimap();
};

#endif  // UI_W_WMINIMAP_H_
