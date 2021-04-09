#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "utils.h"
#include "calculate_stable.h"

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

    // how many arrangements will be checked?
    int max_arrangements = 600;
    if (argc > 2)
        max_arrangements = checked_stoi(argv[2]);
    Lake lake = read_file(argv[1]);

    // take time of calculation
    auto begin = std::chrono::high_resolution_clock::now();
    // scored search
    std::cout << "calculating " << max_arrangements << " best arrangements..." << std::endl;
    do_scored_search(lake, max_arrangements);

    // stability_check
    std::cout << "checking found arrangements for stability..." << std::endl;
    int amount = 0;
    for (std::vector<Arrangement>::iterator arrangement = lake.best_arrangements.begin(); arrangement != lake.best_arrangements.end(); ++arrangement)
        if (is_stable(lake, *arrangement))
        {
            std::cout << "\t" << arrangement - lake.best_arrangements.begin() + 1 << ". place:\t" << arrangement->place_a << " " << arrangement->place_b << " " << arrangement->place_c << std::endl;
            ++amount;
        }

    // final report
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "found " << amount << " stable arrangements in ";
    long long delta_time = (end - begin).count();
    if (delta_time > 1e9)
        std::cout << std::to_string(delta_time / 1e9f) << " seconds";
    else if (delta_time > 1e6)
        std::cout << std::to_string(delta_time / 1e6f) << " milliseconds";
    else if (delta_time > 1e3)
        std::cout << std::to_string(delta_time / 1e3f) << " microseconds";
    else
        std::cout << std::to_string(delta_time) << " nanoseconds";
    std::cout << std::endl;
    if (amount == 0)
        std::cout << "This is not proof that there is no perfect arrangement; a bigger search may lead to one." << std::endl;
    return 0;
}
