#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>
#include <string>

#include "utils.h"
#include "brute_force.h"
#include "scored_search.h"

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
    lake.min_nos = (amount_houses + 1) / 2;

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

    // optimized_test_arrangement(lake, {0, 0, 6});

    // do_brute_force(lake);
    test_test_algos(lake);
    // do_scored_search(lake);
    // Arrangement test_arrangement{7, 14, 0};
    // count_sector_nos(lake, test_arrangement, 1, 10);
    // test_arrangement = {7, 14, 0};
    // count_sector_nos(lake, test_arrangement, 1, 0);
    // test_arrangement = {7, 14, 0};
    // count_sector_nos(lake, test_arrangement, 1, 6);
    return 0;
}
