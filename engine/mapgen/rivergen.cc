#include "engine/mapgen/rivergen.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include "./main.h"
#include "engine/world.h"

Rivergen::Rivergen(const double (&hm)[255][255], const Rectangle_ rect, unsigned int& seedp)
    : Seggen(rect, seedp), hm(hm)
{
    CalculateAltitudes();
}


void
Rivergen::CalculateAltitudes()
{
    double prop_w = rect.P1().X() / (rect.P2().X() - rect.P1().X()),
           prop_h = rect.P1().Y() / (rect.P2().Y() - rect.P1().Y());
    for(auto const& p : points) {
        // find altitude
        int prop_x = static_cast<int>((p.X() / (rect.P2().X() - rect.P1().X()) - prop_w) * 255),
            prop_y = static_cast<int>((p.Y() / (rect.P2().Y() - rect.P1().Y()) - prop_h) * 255);
        altitude[p] = hm[prop_x][prop_y];
    }
}


vector<Point> 
Rivergen::CreateRiver()
{
    vector<Point> segment;

try_again:
    Point p = RandomPoint();
    while(altitude[p] < 0) {
        p = RandomPoint();
    }

    segment.push_back(p);
    while(altitude[p] > 0)
    {
        p = NextPoint(p, segment);
        segment.push_back(p);
    }
    if(segment.size() < 5) {
        segment.clear();
        goto try_again;
    }

    int width = static_cast<int>(world->Random() * 10 + 10);
    AddSegment(segment, width);
    return segment;
}


Point 
Rivergen::RandomPoint() const
{
    return points.at(static_cast<vector<Point>::size_type>(world->Random() * static_cast<double>(points.size())));
}


Point 
Rivergen::NextPoint(Point& p, vector<Point> ignore) const
{
    double r = 1000; // radius
    vector<Point> pts_in_circle;

    // find all points in a circle
    while(pts_in_circle.empty()) {
        for(auto& pt : points) {
            if(pow(pt.X() - p.X(), 2) + pow(pt.Y() - p.Y(), 2) < pow(r, 2)) {
                if(find(ignore.begin(), ignore.end(), pt) == ignore.end()) {
                    pts_in_circle.push_back(pt);
                }
            }
        }
        r *= 2;
    }

    // find lowest point
    double min_altitude = DBL_MAX;
    Point next = p;
    for(auto& pt : pts_in_circle) {
        if(pt != p && altitude.at(pt) < min_altitude) {
            min_altitude = altitude.at(pt);
            next = pt;
        }
    }

    return next;
}
