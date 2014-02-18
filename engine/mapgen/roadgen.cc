#include "engine/mapgen/roadgen.h"

#include <vector>

Roadgen::Roadgen(const double (&hm)[255][255], const Rectangle rect,
            unsigned int& seedp)
    : Seggen(hm, rect, seedp)
{
}

std::vector<Point> 
Roadgen::CreateSegment()
{
    return std::vector<Point>();
}


// vim: ts=4:sw=4:sts=4:expandtab
