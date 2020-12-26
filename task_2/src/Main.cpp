#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

int fruit_amount;

// get vector of indices of all true elements
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

// used to resolve fruit and stand names to enumerated numbers, starting by 0
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

class Skewer
{
private:
    // using flags for each fruit/stand
    // one bool per possible fruit/stand <- true when contains that fruit / uses that stand
    std::vector<bool> fruits;
    std::vector<bool> stands;

public:
    Skewer()
        : stands(fruit_amount), fruits(fruit_amount)
    {
        // create elements for each stand/fruit
        for (bool stand : stands)
            stand = false;

        for (bool fruit : fruits)
            fruit = false;
    }

    const std::vector<bool> get_fruits() const { return fruits; }
    const std::vector<bool> get_stands() const { return stands; }

    // todo: std::optional
    bool contains_fruit(int fruit) const
    {
        if (0 <= fruit && fruit < fruits.size())
            return fruits[fruit];
        return false;
    }
    // todo: std::optional
    bool uses_stand(int stand) const
    {
        if (0 <= stand && stand < stands.size())
            return stands[stand];
        return false;
    }

    // todo: any error needed?
    void add_fruit(int fruit)
    {
        if (0 <= fruit && fruit < fruits.size())
            fruits[fruit] = true;
    }
    // todo: any error needed?
    void add_stand(int stand)
    {
        if (0 <= stand && stand < stands.size())
            stands[stand] = true;
    }
};

class Stand
{
private:
    // using flags for each fruit/stand
    int id;
    // all fruit sets <- all fruits from all skewers using this stand
    // -> this stand's fruit must be in all of these skewers
    std::vector<std::vector<bool>> allowed_fruit_sets;
    // all the fruits on the skewers that didn't use this stand
    // -> this stand's fruit can't be in this list
    std::vector<bool> disallowed_fruits;
    // all the fruits this stand could contain
    std::vector<bool> legal_fruits;
    // true when this stand can satisfy the requested fruits
    bool selected;

public:
    Stand(int id)
        : id(id), disallowed_fruits(fruit_amount), legal_fruits(fruit_amount), selected(false)
    {
        for (bool fruit : legal_fruits)
            fruit = false;

        for (bool fruit : disallowed_fruits)
            fruit = false;
    }

    const int get_id() const { return id; }
    const std::vector<std::vector<bool>> get_allowed_fruit_sets() const { return allowed_fruit_sets; }
    const std::vector<bool> get_disallowed_fruits() const { return disallowed_fruits; }
    const std::vector<bool> get_legal_fruits() const { return legal_fruits; }
    const bool is_selected() const { return selected; }

    void add_allowed_fruit_set(std::vector<bool> fruits)
    {
        allowed_fruit_sets.push_back(fruits);
    }
    // using or on all possible fruits
    void add_disallowed_fruits(std::vector<bool> fruits)
    {
        for (int idx = 0; idx < fruit_amount; idx++)
            disallowed_fruits[idx] = disallowed_fruits[idx] || fruits[idx];
    }
    // todo: error needed?
    void add_legal_fruit(int fruit)
    {
        if (0 <= fruit && fruit < legal_fruits.size())
            legal_fruits[fruit] = true;
    }

    void set_selected(bool new_value = true)
    {
        selected = new_value;
    }
};

// create Skewer instances from input file
int read_file(const char *file_path, std::vector<bool> &requested_fruits, std::vector<Skewer> &skewers)
{
    std::fstream file;
    file.open(file_path);
    if (!file)
    {
        std::cout << "Can't open input file!" << std::endl;
        return 1;
    }

    // todo: what if can't be converted?
    // read amount of fruits
    std::string input_buffer;
    std::getline(file, input_buffer);
    fruit_amount = std::stoi(input_buffer);

    // bool for each fruit, true if requested
    requested_fruits = std::vector<bool>(fruit_amount);
    for (int idx = 0; idx < fruit_amount; idx++)
        requested_fruits[idx] = false;
    // read requested fruits
    std::getline(file, input_buffer);
    std::stringstream ss_input_buffer(input_buffer);
    std::string fruit_buffer;
    while (std::getline(ss_input_buffer, fruit_buffer, ' '))
    {
        // todo: add checks like everywhere
        // todo: should fruit_look_up be reset?
        // todo: how many checks are too much?
        requested_fruits[fruit_look_up.add_item(fruit_buffer)] = true;
    }

    std::getline(file, input_buffer);
    int amount_skewers = std::stoi(input_buffer);

    // get skewers
    for (int idx = 0; idx < amount_skewers; idx++)
    {
        // todo: add checks if amount_skewers is correct
        Skewer skewer;

        // todo: code duplication bad?
        // extract stands
        std::getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        std::string stand_buffer;
        while (std::getline(ss_input_buffer, stand_buffer, ' '))
        {
            // todo: add checks like everywhere
            skewer.add_stand(stand_look_up.add_item(stand_buffer));
        }

        // extract fruits
        std::getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        std::string fruit_buffer;
        while (std::getline(ss_input_buffer, fruit_buffer, ' '))
        {
            // todo: add checks like everywhere
            skewer.add_fruit(fruit_look_up.add_item(fruit_buffer));
        }

        skewers.push_back(skewer);
    }

    // create stands that have no occurrence in the file
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
        return 1;
    }

    if (fruit_look_up.get_amount() != fruit_amount)
    {
        // todo: better error please
        std::cout << "Warning while parsing file!" << std::endl;
        return 1;
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
    for (int fruit : get_true_indices(requested_fruits))
        std::cout << fruit << " ";
    std::cout << std::endl
              << std::endl;
#endif
    return 0;
}

// determine wich fruit could be at wich stand
int determine_legal_fruits(std::vector<Stand> &stands, std::vector<Skewer> &skewers)
{
    // create Stand objects
    // go through all stands and search for skewers using this stand
    for (int stand_id = 0; stand_id < fruit_amount; stand_id++)
    {
        Stand new_stand(stand_id);

        // search through all skewers
        for (Skewer skewer : skewers)
        {
            // when this skewer uses the current stand, one of the fruits on the skewer has to be this stand's one
            if (skewer.uses_stand(stand_id))
                new_stand.add_allowed_fruit_set(skewer.get_fruits());
            // otherwise all the fruits can't be the one
            else
                new_stand.add_disallowed_fruits(skewer.get_fruits());
        }

        stands.push_back(new_stand);
    }

    // find legal stands
    // todo: why is Stand stand : stands not working?
    for (int stand_idx = 0; stand_idx < stands.size(); stand_idx++)
    {
        for (int fruit_idx = 0; fruit_idx < fruit_amount; fruit_idx++)
        {
            // also true when no skewers given <- no info means everything is possible
            bool in_all = true;
            for (std::vector<bool> fruit_set : stands[stand_idx].get_allowed_fruit_sets())
                if (!fruit_set[fruit_idx])
                {
                    in_all = false;
                    break;
                }

            // for this to be a legal fruit, it has to be in every allowed fruit set and not a disallowed fruit
            if (!stands[stand_idx].get_disallowed_fruits()[fruit_idx] && in_all)
                stands[stand_idx].add_legal_fruit(fruit_idx);
        }
    }

#ifdef DEBUG
    std::cout << "Stands (allowed/disallowed fruits):" << std::endl;
    for (Stand stand : stands)
    {
        std::cout << stand.get_id() << " | \t| ";
        for (std::vector<bool> fruit_set : stand.get_allowed_fruit_sets())
        {
            for (int fruit : get_true_indices(fruit_set))
                std::cout << fruit << " ";
            std::cout << " | ";
        }
        std::cout << "\t\t\t| ";
        for (int fruit : get_true_indices(stand.get_disallowed_fruits()))
            std::cout << fruit << " ";
        std::cout << std::endl;
    }

    std::cout << std::endl
              << "Stands (legal fruits):" << std::endl;
    for (Stand stand : stands)
    {
        std::cout << stand.get_id() << " | \t| ";
        for (int fruit : get_true_indices(stand.get_legal_fruits()))
            std::cout << fruit << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    return 0;
}

int get_slected_stands(std::vector<Stand> &stands, std::vector<bool> &requested_fruits, bool &possible)
{
    // false when there isn't enough information to determine to wich stands to go to
    possible = true;
    // todo: why is Stand stand : stands not working?
    // test each stand: is this one providing one of the requested fruits?
    for (int stand_idx = 0; stand_idx < stands.size(); stand_idx++)
    {
        bool all_legal_are_requested = true;
        bool all_legal_are_not_requested = true;
        // go through all the fruits, one of which this stand provides
        for (int idx = 0; idx < fruit_amount; idx++)
        {
            if (stands[stand_idx].get_legal_fruits()[idx])
            {
                if (requested_fruits[idx])
                    all_legal_are_not_requested = false;
                else
                    all_legal_are_requested = false;
            }
        }

        // when there are stands, some of which legal fruits are required and others aren't, there isn't enough information
        if (!all_legal_are_requested && !all_legal_are_not_requested)
        {
            possible = false;
            return 0;
        }
        else if (all_legal_are_requested)
            stands[stand_idx].set_selected();
    }
    return 0;
}

int main()
{
    std::vector<bool> requested_fruits;
    std::vector<Skewer> skewers;
    if (read_file("examples/myspiesse1.txt", requested_fruits, skewers))
        return 1;

    std::vector<Stand> stands;
    if (determine_legal_fruits(stands, skewers))
        return 1;

    bool possible;
    if (get_slected_stands(stands, requested_fruits, possible))
        return 1;

    // todo: better output
    if (possible)
    {
        std::cout << "Selected Stands:" << std::endl;
        for (int idx = 0; idx < stands.size(); idx++)
            if (stands[idx].is_selected())
                std::cout << stand_look_up.get_value(stands[idx].get_id()) << std::endl;
    }
    else
    {
        std::cout << "Not enough info!" << std::endl;
    }

    return 0;
}

// todo: what if file doesn't make sense
// todo: make fruit_amount not global
