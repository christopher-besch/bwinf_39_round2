#include "Stand.h"

Stand::Stand(int id)
    : id(id), legal_fruit_flags(fruit_look_up.get_amount())
{
    for (bool fruit : legal_fruit_flags)
        fruit = false;
}

void Stand::add_allowed_fruits(std::vector<int> fruits)
{
    allowed_fruits.push_back(fruits);
}

void Stand::add_disallowed_fruit(int fruit)
{
    if (!is_disallowed_fruit(fruit))
        disallowed_fruits.push_back(fruit);
}

bool Stand::is_disallowed_fruit(int fruit)
{
    return std::find(disallowed_fruits.begin(), disallowed_fruits.end(), fruit) != disallowed_fruits.end();
}

// void determine_legal_fruits(std::vector<bool> already_allocated_fruits)
// {
//     for (int idx = 0; idx < legal_fruit_flags.size(); idx++)
//     {
//         if (!already_allocated_fruits[idx] &&)
//     }
// }