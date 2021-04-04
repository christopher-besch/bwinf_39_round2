#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>
#include <string>

#include "utils.h"
#include "brute_force.h"
#include "scored_search.h"

Arrangement get_better_arrangement(Lake &lake, Arrangement arrangement)
{
    // get all other possible locations
    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
    {
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
        {
            // test all possible locations for third ice
            for (int place_c = place_b; place_c < lake.circumference; ++place_c)
            {
                if (is_better(lake, arrangement, {place_a, place_b, place_c}))
                {
                    return get_better_arrangement(lake, {place_a, place_b, place_c});
                    std::cout << "hit" << std::endl;
                }
            }
        }
    }
    return arrangement;
}

Lake read_file(const char *file_path)
{
    std::fstream file;
    file.open(file_path);
    if (!file)
        raise_error("Can't open input file!");

    std::string input_buffer;
    checked_getline(file, input_buffer, ' ');
    Lake lake;
    lake.circumference = checked_stoi(input_buffer);

    checked_getline(file, input_buffer, '\n');
    int amount_houses = checked_stoi(input_buffer);
    lake.max_yes = amount_houses / 2;

    lake.houses.resize(amount_houses);
    lake.houses_map.resize(lake.circumference);
    for (int idx = 0; idx < amount_houses; idx++)
    {
        checked_getline(file, input_buffer, ' ');
        int house_location = checked_stoi(input_buffer);
        lake.houses[idx] = house_location;
        ++lake.houses_map[house_location];
    }
    return lake;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");

    Lake lake = read_file(argv[1]);

    // do_brute_force(lake);
    do_scored_search(lake);
    return 0;
}
