#include "engine/mapgen/rivergen.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>

#include "./main.h"
#include "engine/world.h"

Rivergen::Rivergen(const double (&hm)[255][255], const Rectangle rect, unsigned int& seedp)
    : Seggen(hm, rect, seedp)
{
}


std::vector<Point> 
Rivergen::CreateSegment()
{
    std::vector<Point> segment;

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

    // create segments
    int width = static_cast<int>(world->Random() * 10 + 10);
    for(unsigned int i=0; i<segment.size()-1; i++) {
        segments.push_back(Segment(segment[i], segment[i+1], width));
    }

    return segment;
}


Point 
Rivergen::RandomPoint() const
{
    return points.at(static_cast<std::vector<Point>::size_type>(world->Random() * static_cast<double>(points.size())));
}


Point 
Rivergen::NextPoint(Point& p, std::vector<Point> ignore) const
{
    double r = 1000; // radius
    std::vector<Point> pts_in_circle;

    // find all points in a circle
    while(pts_in_circle.empty()) {
        for(auto& pt : points) {
            if(pow(pt.X() - p.X(), 2) + pow(pt.Y() - p.Y(), 2) < pow(r, 2)) {
                if(std::find(ignore.begin(), ignore.end(), pt) == ignore.end()) {
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
