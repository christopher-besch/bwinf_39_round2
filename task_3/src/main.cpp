#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>
#include <string>
#include <chrono>

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
        lake.houses[idx] = {house_location, -1};
        ++lake.houses_map[house_location];
    }
    return lake;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");

    Lake lake = read_file(argv[1]);

    auto begin = std::chrono::high_resolution_clock::now();
    do_scored_search(lake);

    // test_test_algos(lake);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Finished in ";
    long long deltaTime = (end - begin).count();
    if (deltaTime > 1e9)
        std::cout << std::to_string(deltaTime / 1e9f) << " seconds";
    else if (deltaTime > 1e6)
        std::cout << std::to_string(deltaTime / 1e6f) << " milliseconds";
    else if (deltaTime > 1e3)
        std::cout << std::to_string(deltaTime / 1e3f) << " microseconds";
    else
        std::cout << std::to_string(deltaTime) << " nanoseconds";
    std::cout << std::endl;
    return 0;
}
