#pragma once

#include <vector>
#include <string>
#include <algorithm>

class LookupTable
{
private:
    // index is key, element is value
    std::vector<std::string> items;

public:
    // add value to the vector and return key
    // or when the value is already included only return the key
    int add_item(std::string value);

    const std::vector<std::string> &get_values() const { return items; }

    // todo: get std::optional working
    const std::string get_value(int key) const;

    int get_amount() const;
};
