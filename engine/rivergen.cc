#include "engine/rivergen.h"

Rivergen::Rivergen(const double (&hm)[255][255], const Rectangle rect)
	: hm(hm), rect(rect)
{
}


std::vector<Point> 
Rivergen::CreateRiver()
{
	return std::vector<Point>();
}
