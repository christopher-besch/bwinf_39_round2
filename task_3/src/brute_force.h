#pragma once

#include <math.h>

#include "utils.h"

int get_shortest_distance(int circumference, int place_a, int place_b)
{
    int direct_distance = std::abs(place_a - place_b);
    // take shortest way, direct or the other direction
    return std::min(direct_distance, circumference - direct_distance);
}

int get_closest_route(Lake &lake, Arrangement &arrangement, int location)
{
    int distance_a = get_shortest_distance(lake.circumference, location, arrangement.place_a);
    int distance_b = get_shortest_distance(lake.circumference, location, arrangement.place_b);
    int distance_c = get_shortest_distance(lake.circumference, location, arrangement.place_c);
    return std::min(distance_a, std::min(distance_b, distance_c));
}

bool is_better(Lake &lake, Arrangement &current, Arrangement check)
{
    int yes = 0;
    for (auto house : lake.houses)
        // make route shorter?
        yes += get_closest_route(lake, check, house) < get_closest_route(lake, current, house);
    // more yes than nos?
    return yes > (lake.houses.size() - yes);
}

bool test_arrangement(Lake &lake, Arrangement arrangement)
{
    // get all other possible locations
    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
            // test all possible locations for third ice
            for (int place_c = place_b; place_c < lake.circumference; ++place_c)
                if (is_better(lake, arrangement, {place_a, place_b, place_c}))
                    return false;
    return true;
}

int do_brute_force(Lake &lake)
{
    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
    {
        std::cout << place_a << std::endl;
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
        {
            // test all possible locations for third ice
            for (int place_c = place_b; place_c < lake.circumference; ++place_c)
            {
                if (test_arrangement(lake, {place_a, place_b, place_c}))
                {
                    // debug
                    lake.print_houses();
                    lake.print_places(-1, place_a, place_b, place_c);
                    std::cout << place_a << " " << place_b << " " << place_c << std::endl;
                    std::cout << std::endl;
                }
            }
        }
    }
    return 0;
}
