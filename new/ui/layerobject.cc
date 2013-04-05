#include "defines.h"

void 
LayerObject::Render() const
{
	ui->Scene().setProportion(PROP_TILES);
	
	int x1, y1, x2, y2;
	ui->Scene().WindowTileBorders(x1, y1, x2, y2);
	
	multimap<Point<float>, Object> objects;
	CreateObjectList(objects, x1, y1, x2, y2);

	vector<Point<float>> pos;
	SortObjectPoints(objects, pos);

	for(auto const& p: pos) {
		auto obj_it = objects.equal_range(p);
		for(auto obj=obj_it.first; obj != obj_it.second; ++obj) {
			DrawObject(obj->second);
		}
	}
}


void 
LayerObject::CreateObjectList(multimap<Point<float>, Object>& objects,
		int x1, int y1, int x2, int y2) const
{
	for(int x=x1; x<=x2; x++) {
		for(int y=y1; y<=y2; y++) {
			TreeType tt = game->Map().Tree(Point<int>(x, y));
			if(tt != tt_NONE) {
				objects.insert(pair<Point<float>, Object>(
							Point<float>(x, y),
							TreeObj(tt)));
			}
		} 
	}
}


void
LayerObject::SortObjectPoints(multimap<Point<float>, Object>& objects,
		              vector<Point<float>> pos) const
{
}


void
LayerObject::DrawObject(Object obj) const
{
}
