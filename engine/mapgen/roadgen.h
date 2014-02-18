#ifndef ENGINE_MAPGEN_ROADGEN_H_
#define ENGINE_MAPGEN_ROADGEN_H_

#include "engine/mapgen/seggen.h"

class Roadgen final : public Seggen {
public:
    Roadgen(const double (&hm)[255][255], const Rectangle rect,
            unsigned int& seedp);
    std::vector<Point> CreateSegment() override;

private:
    Roadgen(const Roadgen&);
    Roadgen& operator=(const Roadgen&);
};

#endif  // ENGINE_MAPGEN_ROADGEN_H_

// vim: ts=4:sw=4:sts=4:expandtab
