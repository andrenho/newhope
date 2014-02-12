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
        : x(static_cast<double>(floor(x*100)/100.0)),
          y(static_cast<double>(floor(y*100)/100.0)),
          id(static_cast<int64_t>(this->x * 100 * 100000 + this->y * 100)) {}

    Point& operator=(const Point& other) { 
        x = other.x; 
        y = other.y; 
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
        return fabs(x-p.x) + fabs(x-p.y);
    }

    inline double Distance(Point const& p) const {
        return sqrt(pow(x-p.x, 2) + pow(y-p.y, 2));
    }

    inline double X() const { return x; }
    inline double Y() const { return y; }

    inline int64_t Id() const { return id; }

private:
    double x, y;
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
