#include "engine/mapgen/seggen.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
using namespace std;

#include "./main.h"
#include "engine/world.h"

Seggen::Seggen(const Rectangle rect, unsigned int& seedp)
    : rect(rect), seedp(seedp), points({}), segments({})
{
    CreatePoints();
}


void
Seggen::CreatePoints()
{
    for(int i=0; i<4000; i++) {
        double x = world->Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X();
        double y = world->Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y();
        Point p = Point(x, y);
        points.push_back(p);
    }
}


bool 
Seggen::TileInSegment(int x, int y) const
{
    for(auto const& seg: segments) {
        if(seg.TileInSegment(x, y)) {
            return true;
        }
    }
    return false;
}


void 
Seggen::AddSegment(vector<Point> segment, int width)
{
    for(unsigned int i=0; i<segment.size()-1; i++) {
        segments.push_back(Segment(segment[i], segment[i+1], width));
    }
}


Segment::Segment(Point const& p1, Point const& p2, int width)
    : p1(p1), p2(p2), width(width), 
      rect(Rectangle(
                  Point(min(p1.X()-width, p2.X()-width), min(p1.Y()-width, p2.Y()-width)),
                  Point(max(p1.X()+width, p2.X()+width), max(p1.Y()+width, p2.Y()+width)))),
      polygon({})
{
    // create polygon
    double dx = p2.X() - p1.X();
    double dy = p2.Y() - p1.Y();
    double line_length = sqrt(dx * dx + dy * dy);
    dx /= line_length;
    dy /= line_length;
    const double xoff = 0.5 * static_cast<double>(width) * (dy);
    const double yoff = 0.5 * static_cast<double>(width) * dx;
    polygon.push_back(Point(p1.X() - xoff, p1.Y() + yoff));
    polygon.push_back(Point(p2.X() - xoff, p2.Y() + yoff));
    polygon.push_back(Point(p2.X() + xoff, p2.Y() - yoff));
    polygon.push_back(Point(p1.X() + xoff, p1.Y() - yoff));
}


bool 
Segment::TileInSegment(int x, int y) const
{
    Point p(x, y);

    // coarse check
    if(rect.ContainsPoint(p)) {
        // fine check
        uint32_t i, j;
        bool c = false;
        auto nvert = polygon.size();
        for(i=0, j=nvert-1; i < nvert; j = i++) {
            if(((polygon[i].Y() > p.Y()) != (polygon[j].Y() > p.Y())) && 
                        (p.X() < (polygon[j].X() - polygon[i].X()) * (p.Y() - polygon[i].Y()) / (polygon[j].Y() - polygon[i].Y()) + polygon[i].X())) {
                c = !c;
            }
        }
        return c;
    }
    return false;
}


// vim: ts=4:sw=4:sts=4:expandtab
