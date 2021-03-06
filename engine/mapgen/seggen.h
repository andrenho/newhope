#ifndef ENGINE_MAPGEN_SEGGEN_H_
#define ENGINE_MAPGEN_SEGGEN_H_

#include <map>
#include <vector>
using namespace std;
#include "engine/point.h"
#include "engine/rectangle.h"

class Segment final {
public:
    Segment(Point const& p1, Point const& p2, int width);

    Segment(const Segment&) = default;
    Segment& operator=(const Segment&) = delete;
    
    bool TileInSegment(int x, int y) const;

private:
    Point const& p1, p2;
    const int width;
    Rectangle_ const rect;
    vector<Point> polygon = {};
};


class Seggen {
public:
    bool TileInSegment(int x, int y) const;
    void AddSegment(vector<Point> points, int width);

protected:
    Seggen(const Rectangle_ rect, unsigned int& seedp);
    virtual ~Seggen() = default;

    Seggen(const Seggen&) = delete;
    Seggen& operator=(const Seggen&) = delete;

    virtual void CreatePoints();

    const Rectangle_ rect;
    unsigned int& seedp;
    vector<Point> points = {};
    vector<Segment> segments = {};
};

#endif  // ENGINE_MAPGEN_SEGGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
