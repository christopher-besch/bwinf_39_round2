#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Stick.h"
#include "LookupTable.h"

extern LookupTable fruit_look_up;
extern LookupTable stand_look_up;

int read_file(const char *file_path, std::vector<int> &requested_fruits, std::vector<Stick> &sticks)
{
    std::fstream file;
    file.open(file_path);
    if (!file)
    {
        std::cout << "Can't open input file!" << std::endl;
        return 1;
    }

    // read amount of fruits
    std::string input_buffer;
    std::getline(file, input_buffer);
    int amount_fruits = std::stoi(input_buffer);
    // todo: check if amount_fruits is size of fruits

    // read requested fruits
    std::getline(file, input_buffer);
    std::stringstream ss_input_buffer(input_buffer);
    std::string fruit_buffer;
    while (std::getline(ss_input_buffer, fruit_buffer, ' '))
    {
        // todo: add checks like everywhere
        requested_fruits.push_back(fruit_look_up.add_item(fruit_buffer));
    }

    std::getline(file, input_buffer);
    int amount_sticks = std::stoi(input_buffer);

    // get sticks
    for (int idx = 0; idx < amount_sticks; idx++)
    {
        // todo: add checks if amount_sticks is correct
        Stick stick;

        // extract stands
        std::getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        std::string stand_buffer;
        while (std::getline(ss_input_buffer, stand_buffer, ' '))
        {
            // todo: add checks like everywhere
            stick.add_stand(stand_look_up.add_item(stand_buffer));
        }

        // extract fruits
        std::getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        std::string fruit_buffer;
        while (std::getline(ss_input_buffer, fruit_buffer, ' '))
        {
            // todo: add checks like everywhere
            stick.add_fruit(fruit_look_up.add_item(fruit_buffer));
        }

        sticks.push_back(stick);
    }

    for (int idx = stand_look_up.get_amount(); idx < amount_fruits; idx++)
    {
        // create new stand name
        int try_number = 0;
        while (true)
        {
            std::stringstream this_name;
            this_name << "new_stand" << try_number;
            if (stand_look_up.add_item(this_name.str()) == idx)
                break;
            try_number++;
        }
    }
    if (stand_look_up.get_amount() != amount_fruits)
    {
        // todo: better error please
        std::cout << "Warning while parsing file!" << std::endl;
    }

    if (fruit_look_up.get_amount() != amount_fruits)
    {
        // todo: better error please
        std::cout << "Warning while parsing file!" << std::endl;
    }

#ifdef DEBUG
    std::cout << "Fruit Lookup:" << std::endl;
    for (int idx = 0; idx < fruit_look_up.get_amount(); idx++)
        std::cout << idx << "\t" << fruit_look_up.get_value(idx) << std::endl;
    std::cout << std::endl;

    std::cout << "Stand Lookup:" << std::endl;
    for (int idx = 0; idx < stand_look_up.get_amount(); idx++)
        std::cout << idx << "\t" << stand_look_up.get_value(idx) << std::endl;
    std::cout << std::endl;

    std::cout << "Requested Fruits: ";
    for (int fruit : requested_fruits)
        std::cout << fruit << " ";
    std::cout << std::endl
              << std::endl;
#endif
    return 0;
}
