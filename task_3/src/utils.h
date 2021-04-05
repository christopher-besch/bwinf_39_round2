#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>
#include <string>

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

void inline checked_getline(std::istream &in_stream, std::string &out_str, char delimiter = '\n')
{
    if (!std::getline(in_stream, out_str, delimiter) || out_str.empty())
        raise_error("File Parsing Error: More lines required!");
}

int inline checked_stoi(std::string str)
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

template <typename T>
void inline insert(std::vector<T> &list, T &value)
{
    // get location of first element that is not less than value
    typename std::vector<T>::iterator it = std::lower_bound(list.begin(), list.end(), value);
    list.insert(it, value);
}

enum class Direction
{
    left,
    right
};

struct Lake
{
    int circumference;
    std::vector<int> houses;
    // minimum amount of no-votes required to not replace the locations
    int min_nos;
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
            if (idx == (test_place % circumference))
                std::cout << '^';
            else if (idx == (place_a % circumference))
                std::cout << 'a';
            else if (idx == (place_b % circumference))
                std::cout << 'b';
            else if (idx == (place_c % circumference))
                std::cout << 'c';
            else
                std::cout << ' ';
        }
        std::cout << std::endl;
    }
    void print_marker(int place_a, int place_b) const
    {
        for (int idx = 0; idx < circumference; ++idx)
        {
            if (idx == (place_a % circumference))
                std::cout << '|';
            else if (idx == (place_b % circumference))
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

    float score;

    bool operator<(const Arrangement &other) const
    {
        return score < other.score;
    }
    bool operator>(const Arrangement &other) const
    {
        return score > other.score;
    }
};

std::ostream &operator<<(std::ostream &os, Arrangement const &arrangement)
{
    return os << arrangement.place_a << ' ' << arrangement.place_b << ' ' << arrangement.place_c;
}
