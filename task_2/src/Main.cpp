#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

int fruit_amount;

std::vector<int> get_true_indices(std::vector<bool> items)
{
    std::vector<int> result;
    for (int idx = 0; idx < items.size(); idx++)
    {
        if (items[idx])
            result.push_back(idx);
    }
    return result;
}

class LookupTable
{
private:
    // index is key, element is value
    std::vector<std::string> items;

public:
    // add value to the vector and return key
    // or when the value is already included only return the key
    int add_item(std::string value)
    {
        std::vector<std::string>::iterator match = std::find(items.begin(), items.end(), value);
        if (match == items.end())
        {
            items.push_back(value);
            return items.size() - 1;
        }
        return match - items.begin();
    }

    const std::vector<std::string> &get_values() const { return items; }

    // todo: get std::optional working
    const std::string get_value(int key) const
    {
        if (0 <= key && key < items.size())
            return items[key];
        return "";
    }

    int get_amount() const { return items.size(); }
};

LookupTable fruit_look_up;
LookupTable stand_look_up;

class Stick
{
private:
    std::vector<bool> stands;
    std::vector<bool> fruits;

public:
    const std::vector<bool> get_stands() { return stands; }
    const std::vector<bool> get_fruits() { return fruits; }

    Stick()
        : stands(fruit_amount), fruits(fruit_amount)
    {
        for (bool stand : stands)
            stand = false;

        for (bool fruit : fruits)
            fruit = false;
    }

    bool contains_fruit(int fruit)
    {
        return fruits[fruit];
    }
    bool uses_stand(int stand)
    {
        return stands[stand];
    }

    void add_fruit(int fruit)
    {
        fruits[fruit] = true;
    }
    void add_stand(int stand)
    {
        stands[stand] = true;
    }
};

class Stand
{
private:
    int id;
    // list of all fruits on all sticks using this stand, sorted by stick
    // -> this stand's fruit must be in all of these sticks
    std::vector<std::vector<bool>> allowed_fruit_sets;
    // all the fruits on the sticks that didn't use this stand
    // -> this stand's fruit mustn't be in this list
    std::vector<bool> disallowed_fruits;
    // one bool for each fruit, true when this stand could contain this fruit
    std::vector<bool> legal_fruits;

public:
    const int get_id() const { return id; }
    const std::vector<std::vector<bool>> get_allowed_fruits() const { return allowed_fruit_sets; }
    const std::vector<bool> get_disallowed_fruits() const { return disallowed_fruits; }

    Stand(int id)
        : id(id), disallowed_fruits(fruit_amount), legal_fruits(fruit_amount)
    {
        for (bool fruit : legal_fruits)
            fruit = false;

        for (bool fruit : disallowed_fruits)
            fruit = false;
    }

    void add_allowed_fruit_set(std::vector<bool> fruits)
    {
        allowed_fruit_sets.push_back(fruits);
    }
    void add_disallowed_fruit(int fruit)
    {
        disallowed_fruits[fruit] = true;
    }
    // "bitwise" OR
    void add_disallowed_fruits(std::vector<bool> fruits)
    {
        for (int idx = 0; idx < fruit_amount; idx++)
        {
            disallowed_fruits[idx] = disallowed_fruits[idx] || fruits[idx];
        }
    }

    bool is_disallowed_fruit(int fruit)
    {
        return disallowed_fruits[fruit];
    }

    // void determine_legal_fruits(std::vector<bool> already_allocated_fruits)
    // {
    //     for (int idx = 0; idx < legal_fruit_flags.size(); idx++)
    //     {
    //         if (!already_allocated_fruits[idx] &&)
    //     }
    // }
};

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
    fruit_amount = std::stoi(input_buffer);
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

    for (int idx = stand_look_up.get_amount(); idx < fruit_amount; idx++)
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
    if (stand_look_up.get_amount() != fruit_amount)
    {
        // todo: better error please
        std::cout << "Warning while parsing file!" << std::endl;
    }

    if (fruit_look_up.get_amount() != fruit_amount)
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

int assign_fruits(std::vector<Stand> &stands, std::vector<Stick> &sticks)
{
    // go through all stands and search for sticks using this stand
    for (int stand_id = 0; stand_id < fruit_amount; stand_id++)
    {
        Stand this_stand(stand_id);

        // search through all sticks
        for (Stick stick : sticks)
        {
            // todo: apostrophe???
            // when this stick uses the current stand, one of the fruits on the stick has to be this stand's one
            if (stick.uses_stand(stand_id))
                this_stand.add_allowed_fruit_set(stick.get_fruits());
            // otherwise all the fruits can't be the one
            else
                this_stand.add_disallowed_fruits(stick.get_fruits());
        }

        stands.push_back(this_stand);
    }

#ifdef DEBUG
    std::cout << "Stands:" << std::endl;
    for (Stand stand : stands)
    {
        std::cout << stand.get_id() << " | \t| ";
        for (std::vector<bool> stick : stand.get_allowed_fruits())
        {
            for (int fruit : get_true_indices(stick))
                std::cout << fruit << " ";
            std::cout << " | ";
        }
        std::cout << "\t\t\t| ";
        for (int fruit : get_true_indices(stand.get_disallowed_fruits()))
            std::cout << fruit << " ";
        std::cout << std::endl;
    }
#endif

    
    return 0;
}

int main()
{
    std::vector<int> requested_fruits;
    std::vector<Stick> sticks;
    if (read_file("examples/myspiesse1.txt", requested_fruits, sticks))
        return 1;

    std::vector<Stand> stands;
    if (assign_fruits(stands, sticks))
        return 1;

    return 0;
}
