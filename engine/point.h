#ifndef ENGINE_POINT_H_
#define ENGINE_POINT_H_

#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <functional>

class Point final {
public:
    Point(double x, double y)
        : X(static_cast<double>(floor(x*100)/100.0)),
          Y(static_cast<double>(floor(y*100)/100.0)),
          id(static_cast<int64_t>(X * 100 * 100000 + Y * 100)) {}

    // automatic copy constructor
    Point& operator=(const Point& other) { 
        X = other.X; 
        Y = other.Y; 
        id = other.Id();
        return *this;
    } 

    inline bool operator<(const Point& rhs) const {
        return id < rhs.id;
    }

    inline bool operator==(const Point& rhs) const {
        return id == rhs.id;
    }

    inline bool operator!=(const Point& rhs) const {
        return id != rhs.id;
    }

    inline double MH_Distance(Point const& p) const {
        return fabs(x-p.X) + fabs(y-p.Y);
    }

    inline double Distance(Point const& p) const {
        return sqrt(pow(x-p.X, 2) + pow(y-p.Y, 2));
    }

    double X, Y;
    inline int64_t Id() const { return id; }

private:
    int64_t id;
};

namespace std {
    template <>
    struct hash<Point> {
    public:
        int64_t operator()(Point const& p) const noexcept {
            return p.Id();
        }
    };
}

#endif  // ENGINE_POINT_H_


// vim: ts=4:sw=4:sts=4:expandtab
