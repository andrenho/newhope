#ifndef ENGINE_RECTANGLE_H_
#define ENGINE_RECTANGLE_H_

#include "engine/point.h"

class Rectangle_ final {
public:
    Rectangle_()
        : p1(Point(0,0)), p2(Point(0,0)) {}
    Rectangle_(Point const& p1, Point const& p2)
        : p1(p1), p2(p2) {}
    
    Rectangle_(const Rectangle_&) = default;
    Rectangle_& operator=(const Rectangle_&) = delete;

    Point const& P1() const { return p1; }
    Point const& P2() const { return p2; }

    void setP1(Point const& p) { p1 = p; }
    void setP2(Point const& p) { p2 = p; }

    bool ContainsPoint(Point const& p) const {
        return p.X() >= p1.X() && p.Y() >= p1.Y() 
            && p.X() <= p2.X() && p.Y() <= p2.Y();
    }

    bool operator<(const Rectangle_& rhs) const {
        return (p1.Id()+p2.Id()) < (rhs.p1.Id()+rhs.p2.Id());
    }

private:
    Point p1, p2;
};

#endif  // ENGINE_RECTANGLE_H_


// vim: ts=4:sw=4:sts=4:expandtab
