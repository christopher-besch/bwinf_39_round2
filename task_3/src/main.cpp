#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>

#ifdef DEBUG
#define raise_error(msg)                                                                                                  \
    {                                                                                                                     \
        std::cerr << msg << " (in: " << __FILE__ << ":" << __LINE__ << "; in function: " << __func__ << ")" << std::endl; \
        std::exit(EXIT_FAILURE);                                                                                          \
    }
#else
#define raise_error(msg)               \
    {                                  \
        std::cerr << msg << std::endl; \
        std::exit(EXIT_FAILURE);       \
    }
#endif

void checked_getline(std::istream &in_stream, std::string &out_str, char delimiter = '\n')
{
    if (!std::getline(in_stream, out_str, delimiter) || out_str.empty())
        raise_error("File Parsing Error: More lines required!");
}

int checked_stoi(std::string str)
{
    try
    {
        return std::stoi(str);
    }
    catch (std::invalid_argument ex)
    {
        raise_error("File Parsing Error: Can't convert \"" << str << "\" to int!");
    }
}

struct Lake
{
    int circumference;
    std::vector<int> houses;
    // one more yes-votes and the test position is unstable regarding some dummy position
    int max_yes;
    // not using bool to prevent space-efficient specialization
    // allows multiple houses in same location
    std::vector<uint8_t> houses_map;

    // debug
    void print_houses() const
    {
        for (auto place : houses_map)
        {
            if (place)
                std::cout << '|';
            else
                std::cout << '_';
        }
        std::cout << std::endl;
    }
    void print_places(int test_place, int place_a, int place_b, int place_c = -1) const
    {
        for (int idx = 0; idx < circumference; ++idx)
        {
            if (idx == test_place)
                std::cout << '^';
            else if (idx == place_a)
                std::cout << 'a';
            else if (idx == place_b)
                std::cout << 'b';
            else if (idx == place_c)
                std::cout << 'c';
            else
                std::cout << ' ';
        }
        std::cout << std::endl;
    }
    void print_middles(int middle_left, int middle_right) const
    {
        for (int idx = 0; idx < circumference; ++idx)
        {
            if (idx == middle_left)
                std::cout << '|';
            else if (idx == middle_right)
                std::cout << '|';
            else
                std::cout << ' ';
        }
        std::cout << std::endl;
    }
};

struct Arrangement
{
    int place_a;
    int place_b;
    int place_c;
};

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

int get_shortest_distance(int circumference, int place_a, int place_b)
{
    int direct_distance = std::abs(place_a - place_b);
    // take shortest way, direct or the other direction
    return std::min(direct_distance, circumference - direct_distance);
}

int get_closest_route(Lake lake, Arrangement arrangement, int location)
{
    int distance_a = get_shortest_distance(lake.circumference, location, arrangement.place_a);
    int distance_b = get_shortest_distance(lake.circumference, location, arrangement.place_b);
    int distance_c = get_shortest_distance(lake.circumference, location, arrangement.place_c);
    return std::min(distance_a, std::min(distance_b, distance_c));
}

bool is_better(Lake lake, Arrangement current, Arrangement check)
{
    int yes = 0;
    for (auto house : lake.houses)
        // make route shorter?
        yes += get_closest_route(lake, check, house) < get_closest_route(lake, current, house);
    std::cout << std::endl;
    // more yes than nos?
    return yes > (lake.houses.size() - yes);
}

bool test_arrangement(Lake lake, Arrangement arrangement)
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

Arrangement get_better_arrangement(Lake lake, Arrangement arrangement)
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

float get_score(Lake lake, Arrangement arrangement)
{
    float sum = 0;
    for (auto house : lake.houses)
        sum += get_closest_route(lake, arrangement, house);
    return sum / lake.houses.size();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");

    Lake lake = read_file(argv[1]);

    // Arrangement arrangement = get_better_arrangement(lake, {3, 5, 16});
    // lake.print_houses();
    // lake.print_places(-1, arrangement.place_a, arrangement.place_b, arrangement.place_c);
    // std::cout << arrangement.place_a << " " << arrangement.place_b << " " << arrangement.place_c << std::endl;
    // std::cout << std::endl;

    // three ints each
    std::vector<int> result_sets;

    Arrangement best_arrangement;
    float best_score = -1.0f;

    // test_arrangement(lake, {6, 38, 47});

    // go through all possible locations for first ice
    for (int place_a = 0; place_a < lake.circumference; ++place_a)
    {
        std::cout << place_a << std::endl;
        // go through all possible locations for second ice
        for (int place_b = place_a; place_b < lake.circumference; ++place_b)
        {
            // std::cout << place_a << " " << place_b << std::endl;
            // test all possible locations for third ice
            for (int place_c = place_b; place_c < lake.circumference; ++place_c)
            {
                // std::cout << place_a << " " << place_b << " " << place_c << std::endl;
                // if (test_arrangement(lake, {place_a, place_b, place_c}))
                // {
                //     // debug
                //     lake.print_houses();
                //     lake.print_places(-1, place_a, place_b, place_c);
                //     std::cout << place_a << " " << place_b << " " << place_c << std::endl;
                //     std::cout << std::endl;
                //     result_sets.push_back(place_a);
                //     result_sets.push_back(place_b);
                //     result_sets.push_back(place_c);
                // }
                int score = get_score(lake, {place_a, place_b, place_c});
                if (score < best_score || best_score == -1)
                {
                    best_arrangement = {place_a,
                                        place_b,
                                        place_c};
                    best_score = score;
                }
            }
        }
    }
    std::cout << result_sets.size() / 3 << std::endl;
    return 0;
}
