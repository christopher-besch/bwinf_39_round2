#include <vector>

#include "ReadFile.h"
#include "AssignFruits.h"
// todo: use flags everywhere

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
