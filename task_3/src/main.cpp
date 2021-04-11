#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <mutex>

#include "utils.h"
#include "calculate_stable.h"

void read_file(const char *file_path, Lake &lake)
{
    std::fstream file;
    file.open(file_path);
    if (!file)
        raise_error("Can't open input file!");

    std::string input_buffer;
    checked_getline(file, input_buffer, ' ');
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
}

int main(int argc, char *argv[])
{
    //////////////////
    // read console //
    //////////////////
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");
    int max_arrangements = 600;
    if (argc > 2)
        max_arrangements = checked_stoi(argv[2]);
    int thread_amount = max_arrangements;
    if (argc > 3)
        thread_amount = checked_stoi(argv[3]);
    Lake lake;
    read_file(argv[1], lake);

    // take time of calculation
    auto begin = std::chrono::high_resolution_clock::now();

    ///////////////////
    // scored search //
    ///////////////////
    std::cout << "calculating " << max_arrangements << " best arrangements..." << std::endl;
    Arrangement dummy{};
    // fill best arrangements with worse than possible score
    dummy.score = lake.circumference * lake.houses.size();
    lake.best_arrangements.reserve(max_arrangements + 1);
    lake.best_arrangements.insert(lake.best_arrangements.begin(), max_arrangements, dummy);
    do_scored_search(lake, max_arrangements);

    /////////////////////
    // stability check //
    /////////////////////
    std::cout << "running stability check with " << thread_amount << " threads..." << std::endl;
    std::vector<std::thread> thread_pool;
    thread_pool.resize(thread_amount);
    int arrangements_per_thread = max_arrangements / thread_amount;
    // these have to be evenly handed to the threads
    int arrangements_rest = max_arrangements % thread_amount;
    int rest_arrangements_left = arrangements_rest;
    int stable_amount = 0;
    for (int i = 0; i < thread_amount; ++i)
    {
        int offset = i * arrangements_per_thread + arrangements_rest - rest_arrangements_left;
        int length;
        // when there are rest arrangements left to be assigned
        if (rest_arrangements_left)
        {
            --rest_arrangements_left;
            length = arrangements_per_thread + 1;
        }
        else
            length = arrangements_per_thread;
        thread_pool[i] = std::thread(&test_arrangements, std::ref(lake), offset, length, std::ref(stable_amount));
    }
    // let's all be happy together
    for (int i = 0; i < thread_amount; ++i)
        thread_pool[i].join();

    //////////////////
    // final report //
    //////////////////
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "found " << stable_amount << " stable arrangements in ";
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
    if (stable_amount == 0)
        std::cout << "This is not proof that there is no perfect arrangement; a bigger search may lead to one." << std::endl;
    return 0;
}
