#ifndef ENGINE_CITY_H_
#define ENGINE_CITY_H_

#include <vector>

#include "engine/building.h"
#include "engine/citylayout.h"
#include "engine/point.h"

class City final {
public:
	City(int x, int y, CityType type, int n);
	~City();

	int Tiles(const Block* (&block)[10], int x, int y) const;

	inline int W() const { return layout.w; }
	inline int H() const { return layout.h; }

	const int X, Y;

private:
	City(const City&);
	City& operator=(const City&);

	CityLayout const& layout;
	std::vector<const Building*> buildings;
};

#endif  // ENGINE_CITY_H_
