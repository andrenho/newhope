#include "engine/mapgen/roadgen.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
using namespace std;

#include "engine/city.h"

Roadgen::Roadgen(const Rectangle_ rect, unsigned int& seedp)
    : Seggen(rect, seedp)
{
}


vector<vector<Point>>
Roadgen::CreateRoads(vector<class City>& cities)
{
    decltype(Roadgen::CreateRoads(cities)) roads;

    vector<const City*> city_ptr;
    for_each(cities.begin(), cities.end(), [&](const City& city) { city_ptr.push_back(&city); });

    for(auto const& city1: cities) {
        Point p1(city1.X, city1.Y);

        decltype(city_ptr) cities_by_distance;

        // copy city array, excluding cities already connected
        for_each(begin(city_ptr), end(city_ptr),
                [&](const City* c) {
                    if(find(already_connected.begin(), already_connected.end(), make_pair(&city1, c)) == already_connected.end()) {
                        cities_by_distance.push_back(c);
                    }
                });

        // order by distance
        sort(cities_by_distance.begin(), cities_by_distance.end(),
                [&](const City* a, const City* b) {
                    return p1.Distance(Point(a->X, a->Y)) < p1.Distance(Point(b->X, b->Y));
                });

        // connect the closest roads
        for(unsigned int i=0; i<3; i++) {
            if(cities_by_distance.size() > i) {
                auto road = ConnectCities(city1, *cities_by_distance[i]);
                roads.push_back(road);
                AddSegment(road, 20);
            }
        }
    }

    return roads;
}


vector<Point> 
Roadgen::ConnectCities(class City const& city1, class City const& city2)
{
    already_connected.insert(make_pair(&city1, &city2));
    already_connected.insert(make_pair(&city2, &city1));
    return vector<Point> { Point(city1.X, city1.Y), Point(city2.X, city2.Y) };
}


// vim: ts=4:sw=4:sts=4:expandtab
