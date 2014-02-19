#ifndef ENGINE_MAPGEN_RIVERGEN_H_
#define ENGINE_MAPGEN_RIVERGEN_H_

#include <vector>
using namespace std;

#include "engine/mapgen/seggen.h"

class Rivergen final : public Seggen {
public:
    Rivergen(const double (&hm)[255][255], const Rectangle rect,
            unsigned int& seedp);

    vector<Point> CreateRiver();

private:
    void CalculateAltitudes();
    Point RandomPoint() const;
    Point NextPoint(Point& p, vector<Point> ignore) const;

    const double (&hm)[255][255];
    map<Point,double> altitude;
};

#endif  // ENGINE_MAPGEN_RIVERGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
