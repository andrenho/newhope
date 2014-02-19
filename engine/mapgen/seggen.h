#ifndef ENGINE_MAPGEN_SEGGEN_H_
#define ENGINE_MAPGEN_SEGGEN_H_

#include <map>
#include <vector>
#include "engine/point.h"
#include "engine/rectangle.h"

class Segment final {
public:
    Segment(Point const& p1, Point const& p2, int width);
    bool TileInSegment(int x, int y) const;

private:
    Point const& p1, p2;
    const int width;
    Rectangle const rect;
    std::vector<Point> polygon;
};


class Seggen {
public:
    bool TileInSegment(int x, int y) const;
    void AddSegment(std::vector<Point> points, int width);

protected:
    Seggen(const Rectangle rect, unsigned int& seedp);
    virtual ~Seggen() {}

    virtual void CreatePoints();

    const Rectangle rect;
    unsigned int& seedp;
    std::vector<Point> points;
    std::vector<Segment> segments;
    
private:
    Seggen(const Seggen&);
    Seggen& operator=(const Seggen&);
};

#endif  // ENGINE_MAPGEN_SEGGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
