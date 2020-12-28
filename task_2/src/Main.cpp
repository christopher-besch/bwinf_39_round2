#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef DEBUG
#define raise_error(msg)                                                                                                  \
    {                                                                                                                     \
        std::cerr << msg << " (in: " << __FILE__ << ":" << __LINE__ << "; in function: " << __func__ << ")" << std::endl; \
        std::exit(EXIT_FAILURE);                                                                                          \
    }
#else
#define raise_error(msg)               \
    {                                  \
        std::cerr << msg << std::endl; \
        std::exit(EXIT_FAILURE);       \
    }
#endif

void checked_getline(std::istream &in_stream, std::string &out_str, char delimiter = '\n')
{
    if (!std::getline(in_stream, out_str, delimiter) || out_str.empty())
        raise_error("File Parsing Error: More lines required!");
}

int checked_stoi(std::string str)
{
    try
    {
        return std::stoi(str);
    }
    catch (std::invalid_argument ex)
    {
        raise_error("File Parsing Error: Can't convert \"" << str << "\" to int!");
    }
}

// get vector of indices of all true elements
std::vector<int> get_true_indices(std::vector<bool> items)
{
    std::vector<int> result;
    for (int idx = 0; idx < items.size(); idx++)
        if (items[idx])
            result.push_back(idx);
    return result;
}

// used to resolve fruit and bowl names to enumerated numbers, starting by 0
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
        if (value.empty())
            raise_error("Trying to add an empty string as value to a lookup table!");

        std::vector<std::string>::iterator match = std::find(items.begin(), items.end(), value);
        if (match == items.end())
        {
            items.push_back(value);
            return items.size() - 1;
        }
        return match - items.begin();
    }

    const std::string get_value(int key) const
    {
        if (key < 0 || key >= items.size())
            raise_error("Trying to lookup a non-existent key!");
        return items[key];
    }

    void reset()
    {
        items.resize(0);
    }

    int get_amount() const { return items.size(); }
};

LookupTable fruit_look_up;
LookupTable bowl_look_up;

class Skewer
{
private:
    // using flags for each fruit/bowl
    // one bool per possible fruit/bowl <- true when contains that fruit / uses that bowl
    std::vector<bool> fruits;
    std::vector<bool> bowls;

public:
    Skewer(int fruit_amount)
        : bowls(fruit_amount), fruits(fruit_amount) {}

    const std::vector<bool> get_fruits() const { return fruits; }
    const std::vector<bool> get_bowls() const { return bowls; }

    bool contains_fruit(int fruit) const
    {
        if (fruit < 0 || fruit >= fruits.size())
            raise_error("Trying to access a non-existent fruit!");
        return fruits[fruit];
    }
    bool uses_bowl(int bowl) const
    {
        if (bowl < 0 || bowl >= bowls.size())
            raise_error("Trying to access a non-existent bowl!");
        return bowls[bowl];
    }

    void add_fruit(int fruit)
    {
        if (fruit < 0 || fruit >= fruits.size())
            raise_error("Trying to add a non-existent fruit!");
        fruits[fruit] = true;
    }
    void add_bowl(int bowl)
    {
        if (bowl < 0 || bowl >= bowls.size())
            raise_error("Trying to add a non-existent bowl!");
        bowls[bowl] = true;
    }

    void resize(int new_amount)
    {
        bowls.resize(new_amount);
        fruits.resize(new_amount);
    }
};

class Bowl
{
private:
    // using flags for each fruit/bowl
    int id;
    // all fruit sets <- all fruits from all skewers using this bowl
    // -> this bowl's fruit must be in all of these skewers
    std::vector<std::vector<bool>> allowed_fruit_sets;
    // all the fruits on the skewers that didn't use this bowl
    // -> this bowl's fruit can't be in this list
    std::vector<bool> disallowed_fruits;
    // all the fruits this bowl could contain
    std::vector<bool> legal_fruits;
    // true when this bowl can satisfy the requested fruits
    bool selected;
    // true when this bowl could satisfy the requested fruits but there isn't enough information to be sure
    bool possible_selected;

public:
    Bowl(int id, int fruit_amount)
        : id(id), disallowed_fruits(fruit_amount), legal_fruits(fruit_amount), selected(false), possible_selected(false) {}

    const int get_id() const { return id; }
    const std::vector<std::vector<bool>> get_allowed_fruit_sets() const { return allowed_fruit_sets; }
    const std::vector<bool> get_disallowed_fruits() const { return disallowed_fruits; }
    const std::vector<bool> get_legal_fruits() const { return legal_fruits; }

    const bool is_selected() const { return selected; }
    const bool is_possible_selected() const { return possible_selected; }

    void add_allowed_fruit_set(std::vector<bool> fruits)
    {
        allowed_fruit_sets.push_back(fruits);
    }
    // using or on all possible fruits
    void add_disallowed_fruits(std::vector<bool> fruits)
    {
        if (disallowed_fruits.size() != fruits.size())
            raise_error("Amount of disallowed fruits to be added is invalid!");
        for (int idx = 0; idx < disallowed_fruits.size(); idx++)
            disallowed_fruits[idx] = disallowed_fruits[idx] || fruits[idx];
    }
    void add_legal_fruit(int fruit)
    {
        if (fruit < 0 || fruit >= legal_fruits.size())
            raise_error("Trying to add non-existent legal fruit!");
        legal_fruits[fruit] = true;
    }

    void set_selected(bool new_value = true)
    {
        selected = new_value;
    }
    void set_possibly_selected(bool new_value = true)
    {
        possible_selected = new_value;
    }
};

// create Skewer instances from input file
void read_file(const char *file_path, std::vector<bool> &requested_fruits, std::vector<Skewer> &skewers)
{
    std::fstream file;
    file.open(file_path);
    if (!file)
        raise_error("Can't open input file!");

    // read amount of fruits
    std::string input_buffer;
    checked_getline(file, input_buffer);
    int fruit_amount = checked_stoi(input_buffer);

    fruit_look_up.reset();
    // bool for each fruit, true if requested
    requested_fruits.resize(fruit_amount);
    for (int idx = 0; idx < requested_fruits.size(); idx++)
        requested_fruits[idx] = false;
    // read requested fruits
    checked_getline(file, input_buffer);
    std::stringstream ss_input_buffer(input_buffer);
    for (std::string fruit_buffer; std::getline(ss_input_buffer, fruit_buffer, ' ');)
        requested_fruits[fruit_look_up.add_item(fruit_buffer)] = true;

    checked_getline(file, input_buffer);
    int amount_skewers = checked_stoi(input_buffer);

    bowl_look_up.reset();
    // get skewers
    for (int idx = 0; idx < amount_skewers; idx++)
    {
        Skewer new_skewer(fruit_amount);
        // extract bowls
        checked_getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        for (std::string bowl_buffer; std::getline(ss_input_buffer, bowl_buffer, ' ');)
            new_skewer.add_bowl(bowl_look_up.add_item(bowl_buffer));

        // extract fruits
        checked_getline(file, input_buffer);
        ss_input_buffer.clear();
        ss_input_buffer.str(input_buffer);
        for (std::string fruit_buffer; std::getline(ss_input_buffer, fruit_buffer, ' ');)
            new_skewer.add_fruit(fruit_look_up.add_item(fruit_buffer));

        skewers.push_back(new_skewer);
    }

    if (fruit_amount < fruit_look_up.get_amount())
        raise_error("File Parsing Error: The stated amount of fruits (" << fruit_amount << ") is smaller than the amount of used fruits (" << fruit_look_up.get_amount() << ")!");
    if (fruit_amount < bowl_look_up.get_amount())
        raise_error("File Parsing Error: There are more bowls (" << bowl_look_up.get_amount() << ") than the stated amount of fruits (" << fruit_amount << ")!");

    // create bowls that have no occurrence in any skewer
    for (int idx = bowl_look_up.get_amount(); idx < fruit_amount; idx++)
    {
        // create new bowl name
        int try_number = 0;
        while (true)
        {
            std::stringstream this_name;
            this_name << "new_bowl" << try_number;
            // when a new item has been created
            if (bowl_look_up.add_item(this_name.str()) == idx)
                break;
            try_number++;
        }
    }
    // create fruits that have no occurrence in any skewer
    for (int idx = fruit_look_up.get_amount(); idx < fruit_amount; idx++)
    {
        // create new fruit name
        int try_number = 0;
        while (true)
        {
            std::stringstream this_name;
            this_name << "new_fruit" << try_number;
            // when a new item has been created
            if (fruit_look_up.add_item(this_name.str()) == idx)
                break;
            try_number++;
        }
    }

#ifdef DEBUG
    std::cout << "Fruit Lookup:" << std::endl;
    for (int idx = 0; idx < fruit_look_up.get_amount(); idx++)
        std::cout << idx << "\t" << fruit_look_up.get_value(idx) << std::endl;
    std::cout << std::endl;

    std::cout << "Bowl Lookup:" << std::endl;
    for (int idx = 0; idx < bowl_look_up.get_amount(); idx++)
        std::cout << idx << "\t" << bowl_look_up.get_value(idx) << std::endl;
    std::cout << std::endl;

    std::cout << "Requested Fruits: ";
    for (int fruit : get_true_indices(requested_fruits))
        std::cout << fruit << " ";
    std::cout << std::endl
              << std::endl;
#endif
}

// determine which fruit could be at which bowl
void determine_legal_fruits(std::vector<Bowl> &bowls, std::vector<Skewer> &skewers)
{
    // create Bowl objects
    // go through all bowls and search for skewers using this bowl
    for (int bowl_id = 0; bowl_id < bowl_look_up.get_amount(); bowl_id++)
    {
        Bowl new_bowl(bowl_id, fruit_look_up.get_amount());
        // search through all skewers
        for (Skewer &skewer : skewers)
        {
            // when this skewer uses the current bowl, one of the fruits on the skewer has to be this bowl's one
            if (skewer.uses_bowl(bowl_id))
                new_bowl.add_allowed_fruit_set(skewer.get_fruits());
            // otherwise all the fruits can't be the one
            else
                new_bowl.add_disallowed_fruits(skewer.get_fruits());
        }
        bowls.push_back(new_bowl);
    }

    // find legal fruits
    for (Bowl &bowl : bowls)
    {
        for (int fruit_idx = 0; fruit_idx < fruit_look_up.get_amount(); fruit_idx++)
        {
            // also true when no skewers given <- no info means everything is possible
            bool in_all = true;
            for (const std::vector<bool> &fruit_set : bowl.get_allowed_fruit_sets())
                if (!fruit_set[fruit_idx])
                {
                    in_all = false;
                    break;
                }

            // for this to be a legal fruit, it has to be in every allowed fruit set and not a disallowed fruit
            if (!bowl.get_disallowed_fruits()[fruit_idx] && in_all)
                bowl.add_legal_fruit(fruit_idx);
        }
    }

#ifdef DEBUG
    std::cout << "Bowls (allowed/disallowed fruits):" << std::endl;
    for (Bowl &bowl : bowls)
    {
        std::cout << bowl.get_id() << " | \t| ";
        for (const std::vector<bool> &fruit_set : bowl.get_allowed_fruit_sets())
        {
            for (int fruit : get_true_indices(fruit_set))
                std::cout << fruit << " ";
            std::cout << " | ";
        }
        std::cout << "\t\t\t| ";
        for (int fruit : get_true_indices(bowl.get_disallowed_fruits()))
            std::cout << fruit << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl
              << "Bowls (legal fruits):" << std::endl;
    for (Bowl &bowl : bowls)
    {
        std::cout << bowl.get_id() << " | \t| ";
        for (int fruit : get_true_indices(bowl.get_legal_fruits()))
            std::cout << fruit << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif
}

int get_selected_bowls(std::vector<Bowl> &bowls, std::vector<bool> &requested_fruits, bool &possible)
{
    // false when there isn't enough information to determine which bowls to select
    possible = true;
    // test each bowl: is this one providing one of the requested fruits?
    for (Bowl &bowl : bowls)
    {
        bool all_legal_are_requested = true;
        bool all_legal_are_not_requested = true;
        // go through all the fruits, one of which this bowl provides
        for (int idx = 0; idx < fruit_look_up.get_amount(); idx++)
        {
            if (bowl.get_legal_fruits()[idx])
            {
                if (requested_fruits[idx])
                    all_legal_are_not_requested = false;
                else
                    all_legal_are_requested = false;
            }
        }

        // when some of this bowls legal fruits are requested and others aren't, there isn't enough information
        if (!all_legal_are_requested && !all_legal_are_not_requested)
        {
            possible = false;
            bowl.set_possibly_selected();
        }
        else if (all_legal_are_requested)
            bowl.set_selected();
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        raise_error("Please specify the input file as the first console parameter.");

    std::vector<bool> requested_fruits;
    std::vector<Skewer> skewers;
    read_file(argv[1], requested_fruits, skewers);

    std::vector<Bowl> bowls;
    determine_legal_fruits(bowls, skewers);

    bool possible;
    get_selected_bowls(bowls, requested_fruits, possible);

    std::cout << "Bowl-Fruit Lookup:" << std::endl;
    std::cout << "bowl\tfruit (multiple (all possible fruits) when there isn't enough information to precisely determine the fruit)" << std::endl;
    for (Bowl &bowl : bowls)
    {
        std::cout << bowl_look_up.get_value(bowl.get_id()) << "\t";
        for (int fruit_id : get_true_indices(bowl.get_legal_fruits()))
            std::cout << fruit_look_up.get_value(fruit_id) << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // printing results
    std::cout << "Requested Fruits:" << std::endl;
    for (int fruit : get_true_indices(requested_fruits))
        std::cout << fruit_look_up.get_value(fruit) << " ";
    std::cout << std::endl
              << std::endl;
    if (possible)
    {
        std::cout << "There is enough information to precicely determine the right bowls." << std::endl;
        std::cout << "Selected Bowls:" << std::endl;
        for (Bowl &bowl : bowls)
            if (bowl.is_selected())
                std::cout << bowl_look_up.get_value(bowl.get_id()) << " ";
        std::cout << std::endl;
        std::cout << "These bowls contain every and only the requested fruits." << std::endl;
    }
    else
    {
        std::cout << "There isn't enough information to precicely determine the right bowls." << std::endl;
        std::cout << "These bowls are definitely containing the requested fruits:" << std::endl;
        int amount_unfulfilled_requests = get_true_indices(requested_fruits).size();
        for (Bowl &bowl : bowls)
            if (bowl.is_selected())
            {
                std::cout << bowl_look_up.get_value(bowl.get_id()) << " ";
                amount_unfulfilled_requests--;
            }
        std::cout << std::endl;
        std::cout << amount_unfulfilled_requests << " of these bowls contain(s) requested fruits but more information is requested to precicely determine which one:" << std::endl;
        for (Bowl &bowl : bowls)
            if (bowl.is_possible_selected())
                std::cout << bowl_look_up.get_value(bowl.get_id()) << " ";
        std::cout << std::endl;
    }

    return 0;
}
