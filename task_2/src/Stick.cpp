#include "Stick.h"

bool Stick::contains_fruit(int fruit)
{
    return std::find(fruits.begin(), fruits.end(), fruit) != fruits.end();
}

bool Stick::contains_stand(int stand)
{
    return std::find(stands.begin(), stands.end(), stand) != stands.end();
}

void Stick::add_fruit(int fruit)
{
    fruits.push_back(fruit);
}

void Stick::add_stand(int stand)
{
    stands.push_back(stand);
}
