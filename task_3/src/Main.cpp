#include <iostream>
#include <fstream>
#include <vector>
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

void read_file(const char *file_path, int &circumference, std::vector<int> &houses)
{
    std::fstream file;
    file.open(file_path);
    if (!file)
        raise_error("Can't open input file!");

    std::string input_buffer;
    checked_getline(file, input_buffer, ' ');
    circumference = checked_stoi(input_buffer);

    checked_getline(file, input_buffer, '\n');
    int amount_houses = checked_stoi(input_buffer);

    // todo: might cause a degfault, to bad
    houses.resize(amount_houses);
    for (int idx = 0; std::getline(file, input_buffer, ' '); idx++)
        houses[idx] = checked_stoi(input_buffer);
}

int get_min_distance(int circumference, int place_a, int place_b)
{
    int direct_distance = std::abs(place_a - place_b);
    // when it's shorter to go the other way
    if (direct_distance > circumference / 2)
        return circumference - direct_distance;
    return direct_distance;
}

bool vote(int circumference, int house_place, int old_place, int new_place)
{
    // is new place better?
    if (get_min_distance(circumference, house_place, new_place) < get_min_distance(circumference, house_place, old_place))
        return true;
    return false;
}

bool is_stable(int circumference, std::vector<int> &houses, int test_place)
{
    for (int other_place = 0; other_place < circumference; other_place++)
    {
        int trues = 0;
        for (int house : houses)
            if (vote(circumference, house, test_place, other_place))
                trues++;

        if (trues > houses.size() - trues)
            return false;
    }
    return true;
}

std::vector<int> get_stabel_places(int circumference, std::vector<int> &houses)
{
    std::vector<int> result;
    for (int test_place = 0; test_place < circumference; test_place++)
        if (is_stable(circumference, houses, test_place))
            result.push_back(test_place);
    return result;
}

int main()
{
    int circumference;
    std::vector<int> houses;
    read_file("examples/eisbuden7.txt", circumference, houses);

    std::vector<int> stable_places = get_stabel_places(circumference, houses);

    // print results
    for (int stable_place : stable_places)
        std::cout << stable_place << " ";
    std::cout << std::endl;

    return 0;
}
