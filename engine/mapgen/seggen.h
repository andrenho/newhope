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
    virtual std::vector<Point> CreateSegment() = 0;
    bool TileInSegment(int x, int y) const;

protected:
    Seggen(const double (&hm)[255][255], const Rectangle rect, unsigned int& seedp);
    virtual ~Seggen() {}

    virtual void CreatePoints();
    void CalculateAltitudes();

    const double (&hm)[255][255];
    const Rectangle rect;
    unsigned int& seedp;
    std::vector<Point> points;
    std::map<Point,double> altitude;
    std::vector<Segment> segments;
    
private:
    Seggen(const Seggen&);
    Seggen& operator=(const Seggen&);
};

#endif  // ENGINE_MAPGEN_SEGGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
