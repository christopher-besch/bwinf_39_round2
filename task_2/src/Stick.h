#pragma once

#include <vector>
#include <algorithm>

#include "LookupTable.h"

extern LookupTable fruit_look_up;
extern LookupTable stand_look_up;

class Stick
{
private:
    std::vector<int> stands;
    std::vector<int> fruits;

public:
    const std::vector<int> get_stands() { return stands; }
    const std::vector<int> get_fruits() { return fruits; }

    bool contains_fruit(int fruit);
    bool contains_stand(int stand);

    void add_fruit(int fruit);
    void add_stand(int stand);
};
