#include "defines.h"

void 
LayerObject::Render() const
{
	ui->Scene().setProportion(PROP_TILES);
	
	int x1, y1, x2, y2;
	ui->Scene().WindowTileBorders(x1, y1, x2, y2);
	
	multimap<Point<float>, Object*> objects;
	CreateObjectList(objects, x1, y1, x2, y2);

	vector<Point<float>> pos;
	SortObjectPoints(objects, pos);

	for(auto const& p: pos) {
		auto obj_it = objects.equal_range(p);
		for(auto obj=obj_it.first; obj != obj_it.second; ++obj) {
			DrawObject(obj->first, obj->second);
			delete obj->second;
		}
	}
}


void 
LayerObject::CreateObjectList(multimap<Point<float>, Object*>& objects,
		int x1, int y1, int x2, int y2) const
{
	for(int x=x1; x<=x2; x++) {
		for(int y=y1; y<=y2; y++) {

			// add trees
			TreeType tt = game->Map().Tree(Point<int>(x, y));
			if(tt != tt_NONE) {
				objects.insert(pair<Point<float>, Object*>(
							Point<float>(x, y),
							new TreeObj(tt)));
			}

		} 
	}
}


void
LayerObject::SortObjectPoints(multimap<Point<float>, Object*>& objects,
		              vector<Point<float>>& pos) const
{
	set<Point<float>> s;
	for(auto it = objects.begin(); it != objects.end(); it++) {
		s.insert(it->first);
	}
	pos.assign(s.begin(), s.end());
	sort(pos.begin(), pos.end(), 
		[](Point<float> a, Point<float> b) { 
			if(a.y == b.y) {
				return a.x < b.x;
			}
			return a.y < b.y;
		});
}


void
LayerObject::DrawObject(Point<float> p, Object* obj) const
{
	switch(obj->type) {
	case Object::TREE:
		DrawTree(p, static_cast<TreeObj*>(obj)->ttype);
		break;
	}
}


void 
LayerObject::DrawTree(Point<int> p, TreeType tt) const
{
	// draw trunk
	ui->Scene().DrawImage("tree_trunk", p.x, p.y-1);

	// draw treetop
	string ts = "tree_pine";
	if(tt == tt_CONIFER) {
		ts = "tree_conifer";
	} else if(tt == tt_BROADLEAF) {
		ts = "tree_broadleaf";
	}
	ui->Scene().DrawImage(ts, p.x-1, p.y-1);
}
