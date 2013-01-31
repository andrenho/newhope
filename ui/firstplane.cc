#include "ui/firstplane.h"

#include <cstdlib>
#include <queue>
using namespace std;

#include "libs/graphiclibrary.h"
#include "libs/image.h"
#include "ui/resource.h"
#include "ui/terrainsurface.h"
#include "ui/ui.h"
#include "util/logger.h"
#include "world/person.h"

FirstPlane::~FirstPlane()
{
}


void
FirstPlane::DrawObjectsInFrontOf(const Person& person) const
{
	for(int x(person.Pos.x); x<=(person.Pos.x+1); x++) {
		for(int y(person.Pos.y); y<=(person.Pos.y+2); y++) {
			DrawFrontTile({x, y}, person.Pos.y);
		}
	}

}


void 
FirstPlane::DrawFrontTile(Point<int> p, double feet) const
{
	// find image queue
	queue<const Image*> st;
	ui.TerrSurface()->AddFirstPlane(p, st, feet);

	// draw
	Point<int> rel = ui.TileToRel(p);
	while(!st.empty()) {
		const Image* im = st.front();
		Point<int> p = res.Desloc(im);
		st.front()->Blit(*video.Window, rel + p);
		st.pop();
	}
}
