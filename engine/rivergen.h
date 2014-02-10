#ifndef ENGINE_RIVERGEN_H_
#define ENGINE_RIVERGEN_H_

#include <map>
#include <vector>
#include "engine/point.h"
#include "engine/rectangle.h"

class RiverSegment final {
public:
    RiverSegment(Point const& p1, Point const& p2, int width);
    bool TileIsRiver(int x, int y) const;

private:
    Point const& p1, p2;
    const int width;
    Rectangle const rect;
    std::vector<Point> polygon;
};


class Rivergen final {
public:
    Rivergen(const double (&hm)[255][255], const Rectangle rect,
            unsigned int& seedp);

    std::vector<Point> CreateRiver();
    bool TileIsRiver(int x, int y) const;

private:
    Rivergen(const Rivergen&);
    Rivergen& operator=(const Rivergen&);

    Point RandomPoint() const;
    Point NextPoint(Point& p, std::vector<Point> ignore) const;

    const double (&hm)[255][255];
    const Rectangle rect;
    unsigned int& seedp;
    std::vector<Point> points;
    std::map<Point,double> altitude;
    std::vector<RiverSegment> segments;
};

#endif  // ENGINE_RIVERGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
