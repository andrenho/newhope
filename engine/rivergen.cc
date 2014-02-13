#include "engine/rivergen.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>

#include "./globals.h"
#include "engine/world.h"

Rivergen::Rivergen(const double (&hm)[255][255], const Rectangle rect, unsigned int& seedp)
    : hm(hm), rect(rect), seedp(seedp), points({}), altitude({}), segments({})
{
    double prop_w = rect.P1().X() / (rect.P2().X() - rect.P1().X()),
           prop_h = rect.P1().Y() / (rect.P2().Y() - rect.P1().Y());
    for(int i=0; i<1000; i++) {
        // create point
        double x = world->Random() * (-rect.P1().X()+rect.P2().X()) + rect.P1().X();
        double y = world->Random() * (-rect.P1().Y()+rect.P2().Y()) + rect.P1().Y();
        Point p = Point(x, y);
        points.push_back(p);

        // find altitude
        int prop_x = static_cast<int>((p.X() / (rect.P2().X() - rect.P1().X()) - prop_w) * 255),
            prop_y = static_cast<int>((p.Y() / (rect.P2().Y() - rect.P1().Y()) - prop_h) * 255);
        altitude[p] = hm[prop_x][prop_y];
    }
}


std::vector<Point> 
Rivergen::CreateRiver()
{
    std::vector<Point> river;

try_again:
    Point p = RandomPoint();
    while(altitude[p] < 0) {
        p = RandomPoint();
    }

    river.push_back(p);
    while(altitude[p] > 0)
    {
        p = NextPoint(p, river);
        river.push_back(p);
    }
    if(river.size() < 5) {
        river.clear();
        goto try_again;
    }

    // create segments
    int width = static_cast<int>(world->Random() * 10 + 10);
    for(unsigned int i=0; i<river.size()-1; i++) {
        segments.push_back(RiverSegment(river[i], river[i+1], width));
    }

    return river;
}


bool 
Rivergen::TileIsRiver(int x, int y) const
{
    for(auto const& seg: segments) {
        if(seg.TileIsRiver(x, y)) {
            return true;
        }
    }
    return false;
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


RiverSegment::RiverSegment(Point const& p1, Point const& p2, int width)
    : p1(p1), p2(p2), width(width), 
      rect(Rectangle(
                  Point(std::min(p1.X()-width, p2.X()-width), std::min(p1.Y()-width, p2.Y()-width)),
                  Point(std::max(p1.X()+width, p2.X()+width), std::max(p1.Y()+width, p2.Y()+width)))),
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
RiverSegment::TileIsRiver(int x, int y) const
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
