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
    // required for a test position to be stable regarding some dummy position
    int min_nos;
    // not using bool to prevent space-efficient specialization
    // allows multiple houses in same location
    std::vector<uint8_t> houses_map;
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
    lake.min_nos = amount_houses - amount_houses / 2 - 1;

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

// get middle index between a and b; always prefer position closer to a when there are two possible middles
int get_middle(int test_place, int other)
{
    int distance = other - test_place;
    // add 1 to all odd negative distances, e.g. -5
    distance += distance % 2 && distance < 0;
    return test_place + distance / 2;
}

int get_shortest_route_middle_left(Lake lake, int test_place, int place_a, int place_b)
{
    // a
    if (place_a > test_place)
        place_a = lake.circumference - place_a;
    int middle_a = get_middle(test_place, place_a);
    // b
    if (place_b > test_place)
        place_b = lake.circumference - place_b;
    int middle_b = get_middle(test_place, place_b);
    return std::min(middle_a, middle_b);
}

int get_shortest_route_middle_right(Lake lake, int test_place, int place_a, int place_b)
{
    // a
    if (place_a < test_place)
        place_a = lake.circumference + place_a;
    int middle_a = get_middle(test_place, place_a);
    // b
    if (place_b < test_place)
        place_b = lake.circumference + place_b;
    int middle_b = get_middle(test_place, place_b);
    // when middle_b == lake.circumference
    return std::min(middle_a, middle_b) % lake.circumference;
}

void print_debug(Lake lake, int place_a, int place_b, int place_c)
{
    for (auto place : lake.houses_map)
    {
        if (place)
            std::cout << '|';
        else
            std::cout << '_';
    }
    std::cout << std::endl;
    for (int idx = 0; idx < lake.circumference; ++idx)
    {
        if (idx == place_a)
            std::cout << 'a';
        else if (idx == place_b)
            std::cout << 'b';
        else if (idx == place_c)
            std::cout << 'c';
        else
            std::cout << ' ';
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

bool test_place(Lake lake, int test_place, int place_a, int place_b)
{
    // get outer area of yes-voting houses
    int middle_left = get_shortest_route_middle_left(lake, test_place, place_a, place_b);
    int middle_right = get_shortest_route_middle_right(lake, test_place, place_a, place_b);

    // count all houses between a and b
    int yes = 0;
    for (int current_location = middle_left; current_location != middle_right;)
    {
        yes += lake.houses_map[current_location];
        ++current_location;
        // rollover
        if (current_location == lake.circumference)
            current_location = 0;
    }
    // print_debug(lake, place_a, place_b, test_place);
    return !(yes > lake.min_nos);
}

int main_()
{
    Lake lake;
    lake.circumference = 10;
    int middle_left = get_shortest_route_middle_left(lake, 5, 3, 7);
    int middle_right = get_shortest_route_middle_right(lake, 5, 3, 7);

    std::cout << middle_left << " " << middle_right << std::endl;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");

    Lake lake = read_file(argv[1]);

    // three ints each
    std::vector<int> result_sets;

    // go through all possible locations for first ice
    for (int ice_a = 0; ice_a < lake.circumference; ++ice_a)
    {
        std::cout << ice_a << std::endl;
        // go through all possible locations for second ice
        for (int ice_b = ice_a; ice_b < lake.circumference; ++ice_b)
        {
            // test all possible locations for third ice
            for (int ice_c = ice_b; ice_c < lake.circumference; ++ice_c)
            {
                // test all three locations for stability
                // each test expects the other locations to be stable
                bool a = test_place(lake, ice_a, ice_b, ice_c);
                bool b = test_place(lake, ice_b, ice_c, ice_a);
                bool c = test_place(lake, ice_c, ice_a, ice_b);

                if (ice_a != ice_b && ice_b != ice_c && ice_c != ice_a &&
                    test_place(lake, ice_a, ice_b, ice_c) &&
                    test_place(lake, ice_b, ice_c, ice_a) &&
                    test_place(lake, ice_c, ice_a, ice_b))
                {
                    result_sets.push_back(ice_a);
                    result_sets.push_back(ice_b);
                    result_sets.push_back(ice_c);
                    print_debug(lake, ice_a, ice_b, ice_c);
                }
            }
        }
    }

    std::cout << "Hello" << std::endl;

    return 0;
}
