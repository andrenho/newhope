#include "ui/firstplane.h"

#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

#include "libs/graphiclibrary.h"
#include "libs/image.h"
#include "ui/resource.h"
#include "ui/ui.h"
#include "util/logger.h"
#include "world/person.h"

FirstPlane::~FirstPlane()
{
}


void
FirstPlane::DrawObjectsInFrontOf(const Person& person) const
{
	// check for trees
	vector<Point<int>> points {
		Point<int>(person.Pos.x+1, person.Pos.y+1),
		Point<int>(person.Pos.x, person.Pos.y+1),
	};
	for(auto pt: points) {
		TreeType t = world.Tree(pt);
		if(t) {
			DrawTree(pt, t);
		}
	}
}


void
FirstPlane::DrawTree(Point<int> p, TreeType t) const
{
	logger.Debug("%d %d", p.x, p.y);

	Point<int> scr = ui.TileToRel(Point<int>(p.x-1, p.y-1));

	// draw trunk
	string treecode;
	if(t == TreeType::TREE_ROUND) {
		treecode = "1";
	} else if(t == TreeType::TREE_POINTY) {
		treecode = "2";
	} else {
		abort();
	}
	res["trunkfull_" + treecode]->Blit(*video.Window, scr);
	
	// draw treetop
	string cl(world.TreeSmall(p) ? "b" : "a");
	scr = ui.TileToRel(Point<int>(p.x-1, p.y-3));
	res["treetopfull_" + treecode + "_" + cl]->Blit(*video.Window, scr);
}
