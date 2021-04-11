#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <mutex>

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
int error_code = 63909;

enum class Direction
{
    left,
    right
};

struct Arrangement
{
    int place_a;
    int place_b;
    int place_c;
    // the smaller the better
    int score;

    bool operator<(const Arrangement &other) const
    {
        return score < other.score;
    }
    bool operator>(const Arrangement &other) const
    {
        return score > other.score;
    }
};

struct Lake
{
    int circumference;
    // addresses of houses
    std::vector<int> houses;
    // minimum amount of no-votes required to not replace the locations
    int min_nos;
    // not using bool to prevent space-efficient specialization
    // index represents address; 1 -> house exists; 0 -> house doesn't exist
    std::vector<uint8_t> houses_map;
    std::vector<Arrangement> best_arrangements;

    mutable std::mutex print_lock;
};

inline void checked_getline(std::istream &in_stream, std::string &out_str, char delimiter = '\n')
{
    if (!std::getline(in_stream, out_str, delimiter) || out_str.empty())
        raise_error("File Parsing Error: Not enough elements found!");
}

inline int checked_stoi(std::string str)
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

// insert element in correct location in ordered std::vector
template <typename T>
inline void insert(std::vector<T> &list, T &value)
{
    // get location of first element that is not less than value
    typename std::vector<T>::iterator it = std::lower_bound(list.begin(), list.end(), value);
    list.insert(it, value);
}
