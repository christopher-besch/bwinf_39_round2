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

    houses.resize(amount_houses);
    for (int idx = 0; idx < amount_houses; idx++)
    {
        checked_getline(file, input_buffer, ' ');
        houses[idx] = checked_stoi(input_buffer);
    }
}

int get_distance(int circumference, int place_a, int place_b)
{
    int direct_distance = std::abs(place_a - place_b);
    // take shortest way, direct or the other direction
    return std::min(direct_distance, circumference - direct_distance);
}

bool vote(int circumference, int house_place, int old_place, int new_place)
{
    // is new place better?
    if (get_distance(circumference, house_place, new_place) < get_distance(circumference, house_place, old_place))
        return true;
    return false;
}

bool is_stable(int circumference, std::vector<int> &houses, int test_place)
{
    // would any other place win an election against test_place?
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
    // go thorugh all possible places
    std::vector<int> result;
    for (int test_place = 0; test_place < circumference; test_place++)
        if (is_stable(circumference, houses, test_place))
            result.push_back(test_place);
    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");

    int circumference;
    std::vector<int> houses;
    read_file(argv[1], circumference, houses);

    std::vector<int> stable_places = get_stabel_places(circumference, houses);

    // print results
    if (stable_places.size())
    {
        std::cout << "These are all the stable places:" << std::endl;
        for (int stable_place : stable_places)
            std::cout << stable_place << " ";
        std::cout << std::endl;
    }
    else
        std::cout << "There are no stable places." << std::endl;

    return 0;
}
