#ifndef ENGINE_MAPGEN_RIVERGEN_H_
#define ENGINE_MAPGEN_RIVERGEN_H_

#include "engine/mapgen/seggen.h"

class Rivergen final : public Seggen {
public:
    Rivergen(const double (&hm)[255][255], const Rectangle rect,
            unsigned int& seedp);

    virtual std::vector<Point> CreateSegment() override;

private:
    Point RandomPoint() const;
    Point NextPoint(Point& p, std::vector<Point> ignore) const;
};

#endif  // ENGINE_MAPGEN_RIVERGEN_H_


// vim: ts=4:sw=4:sts=4:expandtab
