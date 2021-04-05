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

int count_yes(Lake &lake, Arrangement &current, Arrangement &check)
{
    int yes = 0;
    for (auto house : lake.houses)
        // make route shorter?
        yes += get_closest_route(lake, check, house) < get_closest_route(lake, current, house);
    // more yes than nos?
    return yes;
}

// use specified direction; automatically detects if indirect way is required
int get_abs_distance(int circumference, int this_place, int other_place, Direction direction)
{
    int direct_distance = direction == Direction::left ? this_place - other_place : other_place - this_place;
    // only correct when direct_distance is negative
    int indirect_distance = circumference + direct_distance;
    return direct_distance < 0 ? indirect_distance : direct_distance;
}

// count all houses from include_left to exclude_right - 1
int count_houses(Lake lake, int include_left, int exclude_right)
{
    int houses = 0;
    for (int idx = 0; idx < get_abs_distance(lake.circumference, include_left, exclude_right, Direction::right); idx++)
    {
        houses += lake.houses_map[(include_left + idx) % lake.circumference];
    }
    return houses;
}

// count no-votes between check ice cream parlors left_side and right_side
int count_sector_nos(Lake &lake, Arrangement &current, int left_side, int right_side)
{
    int to_right_distance_a = get_abs_distance(lake.circumference, left_side, current.place_a, Direction::right) % lake.circumference;
    int to_right_distance_b = get_abs_distance(lake.circumference, left_side, current.place_b, Direction::right) % lake.circumference;
    int to_right_distance_c = get_abs_distance(lake.circumference, left_side, current.place_c, Direction::right) % lake.circumference;
    // distances from the right to the nearest place from current arrangement
    int min_to_right_distance = std::min(to_right_distance_a, std::min(to_right_distance_b, to_right_distance_c));

    // when there is no place from the current arrangement in the sector, all of the houses will be delighted to change to the boundaries of the sector
    // -> 0 no-votes
    int sides_distance = get_abs_distance(lake.circumference, left_side, right_side, Direction::right);
    if (min_to_right_distance > sides_distance)
        return 0;

    int to_left_distance_a = get_abs_distance(lake.circumference, right_side, current.place_a, Direction::left) % lake.circumference;
    int to_left_distance_b = get_abs_distance(lake.circumference, right_side, current.place_b, Direction::left) % lake.circumference;
    int to_left_distance_c = get_abs_distance(lake.circumference, right_side, current.place_c, Direction::left) % lake.circumference;
    // distances from the left to the nearest place from current arrangement
    int min_to_left_distance = std::min(to_left_distance_a, std::min(to_left_distance_b, to_left_distance_c));

    // outermost places from current arrangement that are within the sector
    int left_boundary = left_side + min_to_right_distance;
    int right_boundary = right_side - min_to_left_distance;

    // get borders between which all no-voting houses reside
    int left_middle = left_side + (min_to_right_distance + 1) / 2;
    int right_middle = right_side - (min_to_left_distance - 1) / 2;

    // when the left_middle lays on left_side, it gets moved one to the right
    // <- first location in sector doesn't get included; last one does
    left_middle += min_to_right_distance == 0;
    // when the right_middle lays on right side, it gets moved one to the right
    right_middle += min_to_left_distance == 0;

    int nos = count_houses(lake, left_middle, right_middle);

    // lake.print_houses();
    // lake.print_places(-1, current.place_a, current.place_b, current.place_c);
    // std::cout << current << std::endl;
    // lake.print_marker(left_side, right_side);
    // std::cout << left_side << ' ' << right_side << std::endl;
    // // lake.print_marker(left_boundary, right_boundary);
    // // std::cout << left_boundary << ' ' << right_boundary << std::endl;
    // lake.print_marker(left_middle, right_middle);
    // std::cout << left_middle << ' ' << right_middle << std::endl;
    // std::cout << std::endl;
    return nos;
}

bool is_better(Lake &lake, Arrangement &current, Arrangement check)
{
    int yes = count_yes(lake, current, check);
    // int nos_a = count_sector_nos(lake, current, check.place_a, check.place_b);
    // int nos_b = count_sector_nos(lake, current, check.place_b, check.place_c);
    // int nos_c = count_sector_nos(lake, current, check.place_c, check.place_a);
    // if ((yes + nos_a + nos_b + nos_c) != lake.houses.size())
    // {
    //     lake.print_houses();
    //     lake.print_places(-1, current.place_a, current.place_b, current.place_c);
    //     std::cout << current << std::endl;
    //     lake.print_places(-1, check.place_a, check.place_b, check.place_c);
    //     std::cout << check << std::endl;
    //     std::cout << std::endl;
    // }
    // more yes than nos?
    return yes > (lake.houses.size() - yes);
}

bool optimized_is_better(Lake &lake, Arrangement &current, Arrangement check)
{
    int nos_a = count_sector_nos(lake, current, check.place_a, check.place_b);
    int nos_b = count_sector_nos(lake, current, check.place_b, check.place_c);
    int nos_c = count_sector_nos(lake, current, check.place_c, check.place_a);
    return !((nos_a + nos_b + nos_c) >= lake.min_nos);
}

bool test_arrangement(Lake &lake, Arrangement arrangement)
{
    // get all other possible locations
    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
            // test all possible locations for third ice
            // todo: not good
            for (int place_c = place_b + 1; place_c < lake.circumference + 1; ++place_c)
                if (is_better(lake, arrangement, {place_a, place_b, place_c}))
                {
                    is_better(lake, arrangement, {place_a, place_b, place_c});
                    return false;
                }
    return true;
}

bool optimized_test_arrangement(Lake &lake, Arrangement arrangement)
{
    // get all other possible locations
    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
    {
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
        {
            int nos_sector_a = count_sector_nos(lake, arrangement, place_a, place_b);
            // when there are already so many no-votes between place_a and place_b,
            // the location of place_c won't change anything, this arrangement won't beat the test arrangement
            if (nos_sector_a >= lake.min_nos)
            {
                for (int place_c = place_b + 1; place_c < lake.circumference + 1; ++place_c)
                    if (is_better(lake, arrangement, {place_a, place_b, place_c}))
                        std::cout << "not good" << std::endl;
                break;
            }
            // test all possible locations for third ice
            for (int place_c = place_b + 1; place_c < lake.circumference + 1; ++place_c)
            {
                int nos_sector_b = count_sector_nos(lake, arrangement, place_b, place_c);
                int nos_sector_c = count_sector_nos(lake, arrangement, place_c, place_a);
                int nos = nos_sector_a + nos_sector_b + nos_sector_c;
                if (nos >= lake.min_nos)
                {
                    if (is_better(lake, arrangement, {place_a, place_b, place_c}))
                        std::cout << "not good" << std::endl;
                    break;
                }
                return false;
            }
        }
    }
    if (!test_arrangement(lake, arrangement))
    {
        test_arrangement(lake, arrangement);
    }
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
                if (optimized_test_arrangement(lake, {place_a, place_b, place_c}) != test_arrangement(lake, {place_a, place_b, place_c}))
                    std::cout << "not good" << std::endl;
                if (optimized_test_arrangement(lake, {place_a, place_b, place_c}))
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

void test_test_algos(Lake &lake)
{
    for (int arrangement_place_a = 0; arrangement_place_a < lake.circumference; ++arrangement_place_a)
        for (int arrangement_place_b = arrangement_place_a; arrangement_place_b < lake.circumference; ++arrangement_place_b)
            for (int arrangement_place_c = arrangement_place_b; arrangement_place_c < lake.circumference; ++arrangement_place_c)
            {
                for (int check_place_a = 0; check_place_a < lake.circumference; ++check_place_a)
                    for (int check_place_b = check_place_a; check_place_b < lake.circumference; ++check_place_b)
                        for (int check_place_c = check_place_b; check_place_c < lake.circumference; ++check_place_c)
                        {
                            Arrangement arrangement{arrangement_place_a, arrangement_place_b, arrangement_place_c};
                            Arrangement check{check_place_a, check_place_b, check_place_c};

                            if (is_better(lake, arrangement, check) != optimized_is_better(lake, arrangement, check))
                            {
                                std::cout << is_better(lake, arrangement, check) << " " << optimized_is_better(lake, arrangement, check) << std::endl;
                                std::cout
                                    << "not good" << std::endl;
                            }
                        }
            }
}
