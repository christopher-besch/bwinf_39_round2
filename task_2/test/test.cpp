#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
    std::vector<bool> items1;
    items1.push_back(true);
    items1.push_back(true);
    items1.push_back(true);
    items1.push_back(true);
    items1.push_back(true);
    items1.push_back(true);
    items1.push_back(true);
    items1.push_back(false);
    items1.push_back(false);

    std::vector<bool> items2;
    items2.push_back(false);
    items2.push_back(false);
    items2.push_back(false);
    items2.push_back(false);
    items2.push_back(false);
    items2.push_back(false);
    items2.push_back(true);
    items2.push_back(true);
    items2.push_back(true);

    items1 = items1 | items2;

    for (bool t : items1)
        std::cout << t << std::endl;
}
