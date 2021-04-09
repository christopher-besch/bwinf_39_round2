#pragma once

#include <math.h>

#include "utils.h"

// use specified direction; detects if indirect way is required
inline int get_abs_distance(int circumference, int this_place, int other_place, Direction direction)
{
    int left_distance = this_place - other_place;
    int right_distance = other_place - this_place;
    int direct_distance = direction == Direction::left ? left_distance : right_distance;
    // only correct when direct_distance is negative
    int indirect_distance = circumference + direct_distance;
    return direct_distance < 0 ? indirect_distance : direct_distance;
}

// count all houses from include_left inclusively to exclude_right exclusively
inline int count_houses(Lake &lake, int include_left, int exclude_right)
{
    int houses = 0;
    for (int idx = 0; idx < get_abs_distance(lake.circumference, include_left, exclude_right, Direction::right); ++idx)
        houses += lake.houses_map[(include_left + idx) % lake.circumference];
    return houses;
}

// count no-votes between check ice cream parlors left_side and right_side
// indirect -> take longer route around lake <- necessary example when left_side > right_side
// exclude possible no-vote from house on left_side
// include possible no-vote form house on right_side
inline int count_sector_nos(Lake &lake, Arrangement &test_arrangement, int left_side, int right_side, bool indirect = false)
{
// optimization: not used in this version
#ifdef DEBUG
    right_side += indirect ? lake.circumference : 0;
#endif

    int to_right_distance_a = get_abs_distance(lake.circumference, left_side, test_arrangement.place_a, Direction::right);
    int to_right_distance_b = get_abs_distance(lake.circumference, left_side, test_arrangement.place_b, Direction::right);
    int to_right_distance_c = get_abs_distance(lake.circumference, left_side, test_arrangement.place_c, Direction::right);
    // distance from the right to the nearest ice from test-arrangement
    int min_to_right_distance = std::min(to_right_distance_a, std::min(to_right_distance_b, to_right_distance_c));

    // when there is no place from the current arrangement in the sector, all of the houses will be delighted to change to the boundaries of the sector
    // -> 0 no-votes
    int sides_distance = get_abs_distance(lake.circumference, left_side, right_side, Direction::right);
    if (min_to_right_distance > sides_distance)
        return 0;

// optimization: not necessary in this version <- right_side doesn't get increased
#ifdef DEBUG
    int to_left_distance_a = get_abs_distance(lake.circumference, right_side, test_arrangement.place_a, Direction::left) % lake.circumference;
    int to_left_distance_b = get_abs_distance(lake.circumference, right_side, test_arrangement.place_b, Direction::left) % lake.circumference;
    int to_left_distance_c = get_abs_distance(lake.circumference, right_side, test_arrangement.place_c, Direction::left) % lake.circumference;
#else
    int to_left_distance_a = get_abs_distance(lake.circumference, right_side, test_arrangement.place_a, Direction::left);
    int to_left_distance_b = get_abs_distance(lake.circumference, right_side, test_arrangement.place_b, Direction::left);
    int to_left_distance_c = get_abs_distance(lake.circumference, right_side, test_arrangement.place_c, Direction::left);
#endif

    // distance from the left to the nearest ice from test-arrangement
    int min_to_left_distance = std::min(to_left_distance_a, std::min(to_left_distance_b, to_left_distance_c));

    // get borders between which all no-voting houses reside
    int left_nos = left_side + (min_to_right_distance + 1) / 2;
    int right_nos_exclude = right_side - (min_to_left_distance - 1) / 2;

    // when the left_middle lays on left_side, it gets moved one to the right
    // <- first location in sector doesn't get included; last one does
    left_nos += min_to_right_distance == 0;
    // when the right_middle lays on right side, it gets moved one to the right
    right_nos_exclude += min_to_left_distance == 0;

    return count_houses(lake, left_nos, right_nos_exclude);
}

inline int get_shortest_distance(int circumference, int place_a, int place_b)
{
    int direct_distance = std::abs(place_a - place_b);
    // take shortest way, direct or the other direction
    return std::min(direct_distance, circumference - direct_distance);
}

inline int get_ice_cream_distance(Lake &lake, Arrangement &arrangement, int location)
{
    int distance_a = get_shortest_distance(lake.circumference, location, arrangement.place_a);
    int distance_b = get_shortest_distance(lake.circumference, location, arrangement.place_b);
    int distance_c = get_shortest_distance(lake.circumference, location, arrangement.place_c);
    return std::min(distance_a, std::min(distance_b, distance_c));
}

inline bool is_better(Lake &lake, Arrangement &check_arrangement)
{
    int yes = 0;
    for (auto house : lake.houses)
        // makes route shorter?
        yes += get_ice_cream_distance(lake, check_arrangement, house.location) < house.closest_route;
    // more yes than nos?
    return yes > (lake.houses.size() - yes);
}

inline bool is_stable(Lake &lake, Arrangement &arrangement)
{
    // best routes for test-arrangement
    for (auto &house : lake.houses)
        house.closest_route = get_ice_cream_distance(lake, arrangement, house.location);

    // get all other possible locations
    // multiple ice cream parlors in same location are a waste
    Arrangement check_arrangement;
    for (check_arrangement.place_a = 0; check_arrangement.place_a < lake.circumference; ++check_arrangement.place_a)
        for (check_arrangement.place_b = check_arrangement.place_a + 1; check_arrangement.place_b < lake.circumference; ++check_arrangement.place_b)
        {
            // when there are already so many no-votes between place_a and place_b,
            // the location of place_c won't change anything -> this arrangement won't beat the test-arrangement
            if (count_sector_nos(lake, arrangement, check_arrangement.place_a, check_arrangement.place_b, false) >= lake.min_nos)
                break;
            // test all possible locations for third ice
            for (check_arrangement.place_c = check_arrangement.place_b + 1; check_arrangement.place_c < lake.circumference; ++check_arrangement.place_c)
                if (is_better(lake, check_arrangement))
                    return false;
            // alternative: calculate nos with count_sector_nos between place_b and place_c, place_c and place_a
            // <- not used <- is actually slower
        }
    return true;
}

// go through all possible arrangements and pick the <max_arrangements> best
// result gets stored in lake
inline void do_scored_search(Lake &lake, int max_arrangements)
{
    Arrangement dummy{};
    // worst possible score
    dummy.score = lake.circumference * lake.houses.size();
    lake.best_arrangements.reserve(max_arrangements + 1);
    lake.best_arrangements.insert(lake.best_arrangements.begin(), max_arrangements, dummy);

    // go through all possible test-arrangements
    // optimization: two ice creams on the same location are a waste
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
        for (int place_b = place_a + 1; place_b < lake.circumference; ++place_b)
            for (int place_c = place_b + 1; place_c < lake.circumference; ++place_c)
            {
                Arrangement test_arrangement{place_a, place_b, place_c};
                // calculate score
                // optimization: sum of closest routes instead of average shortest route <- only relative value important
                for (const auto &house : lake.houses)
                    test_arrangement.score += get_ice_cream_distance(lake, test_arrangement, house.location);
                // keep std::vector sorted at all time
                insert(lake.best_arrangements, test_arrangement);
                lake.best_arrangements.pop_back();
            }
}
