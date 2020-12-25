#pragma once

#include <vector>
#include <algorithm>

#include "LookupTable.h"

extern LookupTable fruit_look_up;
extern LookupTable stand_look_up;

class Stand
{
private:
    int id;
    // list of all fruits on all sticks using this stand, sorted by stick
    // -> this stand's fruit must be in all of these sticks
    std::vector<std::vector<int>> allowed_fruits;
    // all the fruits on the sticks that didn't use this stand
    // -> this stand's fruit mustn't be in this list
    std::vector<int> disallowed_fruits;
    // one bool for each fruit, true when this stand could contain this fruit
    std::vector<bool> legal_fruit_flags;

public:
    const int get_id() const { return id; }

    Stand(int id);

    void add_allowed_fruits(std::vector<int> fruits);
    void add_disallowed_fruit(int fruit);

    bool is_disallowed_fruit(int fruit);

    const std::vector<std::vector<int>> get_allowed_fruits() const { return allowed_fruits; }
    const std::vector<int> get_disallowed_fruits() const { return disallowed_fruits; }
};
