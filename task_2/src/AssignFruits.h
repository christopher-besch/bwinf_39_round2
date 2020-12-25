#include <vector>
#include <iostream>

#include "LookupTable.h"
#include "Stand.h"
#include "Stick.h"

LookupTable fruit_look_up;
LookupTable stand_look_up;

int assign_fruits(std::vector<Stand> &stands, std::vector<Stick> &sticks)
{
    for (int stand_id = 0; stand_id < stand_look_up.get_amount(); stand_id++)
    {
        Stand this_stand(stand_id);

        for (Stick stick : sticks)
        {
            if (stick.contains_stand(stand_id))
                this_stand.add_allowed_fruits(stick.get_fruits());
            else
                for (int fruit : stick.get_fruits())
                    this_stand.add_disallowed_fruit(fruit);
        }

        stands.push_back(this_stand);
    }

#ifdef DEBUG
    std::cout << "Stands:" << std::endl;
    for (Stand stand : stands)
    {
        std::cout << stand.get_id() << " | \t| ";
        for (std::vector<int> stick : stand.get_allowed_fruits())
        {
            for (int fruit : stick)
                std::cout << fruit << " ";
            std::cout << " | ";
        }
        std::cout << "\t\t\t| ";
        for (int fruit : stand.get_disallowed_fruits())
            std::cout << fruit << " ";
        std::cout << std::endl;
    }
#endif
    return 0;
}
