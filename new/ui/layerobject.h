#ifndef UI_LAYEROBJECT_H
#define UI_LAYEROBJECT_H

#include "defines.h"

#include <map>

class LayerObject : public Layer {
public:
	void Render() const;

private:
	struct Object { 
		enum ObjType { TREE } type;
		Object(enum ObjType t) : type(t) { }
	};

	void CreateObjectList(multimap<Point<float>, Object*>& objects, 
			int x1, int y1, int x2, int y2) const;
	void SortObjectPoints(multimap<Point<float>, Object*>& objects,
		              vector<Point<float>>& pos) const;

	// draw stuff
	void DrawObject(Point<float> p, Object* obj) const;
	void DrawTree(Point<int> p, TreeType tt) const;

	//
	// object types
	//
	struct TreeObj : public Object {
		TreeObj(TreeType ttype) : Object(TREE), ttype(ttype) { }
		TreeType ttype;
	};
};

#endif
