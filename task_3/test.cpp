#include <vector>
#include <math.h>
#include <functional>
#include <iostream>

void insert(std::vector<int> &list, int value)
{
    // get location of first element that is not less than value
    std::vector<int>::iterator it = std::lower_bound(list.begin(), list.end(), value);
    list.insert(it, value);
}

int main()
{
    std::vector<int> ints = {0, 3, 4, 5};
    insert(ints, 2);
    ints.pop_back();
    for (int x : ints)
        std::cout << x << std::endl;
}
