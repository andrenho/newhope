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
	for(double x(person.pos().x); x<=(person.pos().x+1); x++) {
		for(double y(person.pos().y); y<=(person.pos().y+2); y++) {
			DrawFrontTile({x, y}, person.pos().y);
		}
	}

}


void 
FirstPlane::DrawFrontTile(Tile p, double feet) const
{
	// find image queue
	queue<const Image*> st;
	ui.TerrSurface()->AddFirstPlane(p, st, feet);

	// draw
	RelPoint rel(p);
	while(!st.empty()) {
		const Image* im(st.front());
		Point<int> p(res.Desloc(im));
		st.front()->Blit(video.Window(), rel + p);
		st.pop();
	}
}
