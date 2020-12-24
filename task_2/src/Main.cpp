#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

struct Stick
{
    std::vector<int> stands;
    std::vector<int> fruits;
};

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
        std::vector<std::string>::iterator match = std::find(FruitLookup::fruits.begin(), fruits.end(), fruit_name);
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

int main()
{
    std::fstream input_file;
    input_file.open("examples/spiesse1.txt");
    if (!input_file)
    {
        std::cout << "Can't open input file!" << std::endl;
        return 1;
    }

    // read amount of fruits
    std::string input_buffer;
    std::getline(input_file, input_buffer);
    int amount_fruits = std::stoi(input_buffer);
    // todo: check if amount_fruits is size of fruits

    // read requested fruits
    std::getline(input_file, input_buffer);
    std::stringstream ss_input_buffer(input_buffer);
    std::string fruit_buffer;
    std::vector<std::string> requested_fruits;
    while (std::getline(ss_input_buffer, fruit_buffer, ' '))
    {
        // todo: add checks like everywhere
        requested_fruits.push_back(fruit_buffer);
        FruitLookup::add_fruit(fruit_buffer);
    }

    std::getline(input_file, input_buffer);
    int amount_sticks = std::stoi(input_buffer);

    // get sticks
    std::vector<Stick> sticks(amount_sticks);
    for (int idx = 0; idx < amount_sticks; idx++)
    {
        Stick stick;

        // extract stands
        std::getline(input_file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        std::string stand_buffer;
        while (std::getline(ss_input_buffer, stand_buffer, ' '))
        {
            // todo: add checks like everywhere
            stick.stands.push_back(std::stoi(stand_buffer));
        }

        // extract fruits
        std::getline(input_file, input_buffer);
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
        std::cout << "Error while parsing file!" << std::endl;
    }

    return 0;
}
