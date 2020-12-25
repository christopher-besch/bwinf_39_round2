#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class FruitLookup
{
private:
    // element is fruit name, index is reference key (called fruit)
    static std::vector<std::string> fruits;

public:
    FruitLookup() = delete;

    // add fruit name to the vector and return key
    // or when the fruit is already included only return the key
    static int add_fruit(std::string fruit_name)
    {
        std::vector<std::string>::iterator match = std::find(fruits.begin(), fruits.end(), fruit_name);
        if (match == fruits.end())
        {
            fruits.push_back(fruit_name);
            return fruits.size() - 1;
        }
        return match - fruits.begin();
    }

    static std::string get_fruit_name(int fruit)
    {
        return fruits[fruit];
    }

    static int get_amount()
    {
        return fruits.size();
    }
};

std::vector<std::string> FruitLookup::fruits = std::vector<std::string>();

// todo: use flags everywhere
struct Stick
{
    std::vector<int> stands;
    std::vector<int> fruits;

    bool contains_fruit(int fruit)
    {
        return std::find(fruits.begin(), fruits.end(), fruit) != fruits.end();
    }

    bool contains_stand(int stand)
    {
        return std::find(stands.begin(), stands.end(), stand) != stands.end();
    }
};

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
    Stand(int id)
        : id(id), legal_fruit_flags(FruitLookup::get_amount())
    {
        for (bool fruit : legal_fruit_flags)
            fruit = false;
    }

    void add_allowed_fruits(std::vector<int> fruits)
    {
        allowed_fruits.push_back(fruits);
    }

    void add_disallowed_fruit(int fruit)
    {
        if (!is_disallowed_fruit(fruit))
            disallowed_fruits.push_back(fruit);
    }

    bool is_disallowed_fruit(int fruit)
    {
        return std::find(disallowed_fruits.begin(), disallowed_fruits.end(), fruit) != disallowed_fruits.end();
    }

    const int get_id() const { return id; }
    const std::vector<std::vector<int>> get_allowed_fruits() const { return allowed_fruits; }
    const std::vector<int> get_disallowed_fruits() const { return disallowed_fruits; }

    // void determine_legal_fruits(std::vector<bool> already_allocated_fruits)
    // {
    //     for (int idx = 0; idx < legal_fruit_flags.size(); idx++)
    //     {
    //         if (!already_allocated_fruits[idx] &&)
    //     }
    // }
};

int main()
{
    std::fstream file;
    file.open("examples/myspiesse1.txt");
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
    std::vector<int> requested_fruits;
    while (std::getline(ss_input_buffer, fruit_buffer, ' '))
    {
        // todo: add checks like everywhere
        requested_fruits.push_back(FruitLookup::add_fruit(fruit_buffer));
    }

    std::getline(file, input_buffer);
    int amount_sticks = std::stoi(input_buffer);

    // get sticks
    std::vector<Stick> sticks(amount_sticks);
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
            stick.stands.push_back(std::stoi(stand_buffer));
        }

        // extract fruits
        std::getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        std::string fruit_buffer;
        while (std::getline(ss_input_buffer, fruit_buffer, ' '))
        {
            // todo: add checks like everywhere
            stick.fruits.push_back(FruitLookup::add_fruit(fruit_buffer));
        }

        sticks[idx] = stick;
    }

    if (FruitLookup::get_amount() != amount_fruits)
    {
        // todo: better error please
        std::cout << "Warning while parsing file!" << std::endl;
    }

#ifdef DEBUG
    std::cout << "Fruit Lookup:" << std::endl;
    for (int idx = 0; idx < FruitLookup::get_amount(); idx++)
        std::cout << idx << "\t" << FruitLookup::get_fruit_name(idx) << std::endl;
    std::cout << std::endl;

    std::cout << "Requested Fruits: ";
    for (int fruit : requested_fruits)
        std::cout << fruit << " ";
    std::cout << std::endl
              << std::endl;
#endif

    std::vector<Stand> stands;
    for (int stand_id = 1; stand_id <= FruitLookup::get_amount(); stand_id++)
    {
        Stand this_stand(stand_id);

        for (Stick stick : sticks)
        {
            if (stick.contains_stand(stand_id))
                this_stand.add_allowed_fruits(stick.fruits);
            else
                for (int fruit : stick.fruits)
                    this_stand.add_disallowed_fruit(fruit);
        }

        stands.push_back(this_stand);
    }

#if 1
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
