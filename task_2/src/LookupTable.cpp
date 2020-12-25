#include "LookupTable.h"

int LookupTable::add_item(std::string value)
{
    std::vector<std::string>::iterator match = std::find(items.begin(), items.end(), value);
    if (match == items.end())
    {
        items.push_back(value);
        return items.size() - 1;
    }
    return match - items.begin();
}

const std::vector<std::string> &LookupTable::get_values() const
{
    return items;
}

// todo: get std::optional working
const std::string LookupTable::get_value(int key) const
{
    if (0 <= key && key < items.size())
        return items[key];
    return "";
}

int LookupTable::get_amount() const
{
    return items.size();
}
