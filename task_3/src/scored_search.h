#pragma once
#include <algorithm>

#include "utils.h"
#include "brute_force.h"

float get_score(Lake &lake, Arrangement arrangement)
{
    float sum = 0;
    for (auto house : lake.houses)
        sum += get_closest_route(lake, arrangement, house.location);
    return sum / lake.houses.size();
}

void do_scored_search(Lake lake)
{
    constexpr int max_arrangements = 1;
    std::vector<Arrangement> best_arrangements;
    Arrangement dummy{};
    // worst possible score
    dummy.score = lake.circumference;
    best_arrangements.reserve(max_arrangements + 1);
    best_arrangements.insert(best_arrangements.begin(), max_arrangements, dummy);

    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
    {
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
        {
            // test all possible locations for third ice
            for (int place_c = place_b; place_c < lake.circumference; ++place_c)
            {
                Arrangement this_arrangement{place_a, place_b, place_c};
                this_arrangement.score = get_score(lake, this_arrangement);

                insert(best_arrangements, this_arrangement);
                best_arrangements.pop_back();
            }
        }
    }

    std::cout << "checking found arrangements" << std::endl;
    int amount = 0;

    for (int idx = 0; idx < max_arrangements; ++idx)
    {
        if (test_arrangement(lake, best_arrangements[idx]))
        {
            lake.print_houses();
            lake.print_places(-1, best_arrangements[idx].place_a, best_arrangements[idx].place_b, best_arrangements[idx].place_c);
            std::cout << best_arrangements[idx].place_a << " " << best_arrangements[idx].place_b << " " << best_arrangements[idx].place_c << std::endl;
            std::cout << idx << std::endl;
            std::cout << std::endl;
            ++amount;
        }
    }
    std::cout << amount << std::endl;

    return;
}
