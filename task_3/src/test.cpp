#include <math.h>
#include <iostream>
#include <string>

enum class Direction
{
    left,
    right
};

int get_abs_distance(int circumference, int this_place, int other_place, Direction direction)
{
    int direct_distance = direction == Direction::left ? this_place - other_place : other_place - this_place;
    // only correct when direct_distance is negative
    int indirect_distance = circumference + direct_distance;
    return direct_distance < 0 ? indirect_distance : direct_distance;
}

int main()
{
    int circumference = 20;
    int test_place = 10;
    int place_a = 15;
    int place_b = 6;

    int distance_a_left = get_abs_distance(circumference, test_place, place_a, Direction::left);
    int distance_b_left = get_abs_distance(circumference, test_place, place_b, Direction::left);
    int distance_a_right = get_abs_distance(circumference, test_place, place_a, Direction::right);
    int distance_b_right = get_abs_distance(circumference, test_place, place_b, Direction::right);

    int distance_left = distance_a_left < distance_b_left ? distance_a_left : distance_b_left;
    int distance_right = distance_a_left < distance_b_left ? distance_b_right : distance_a_right;

    for (int idx = 0; idx < circumference; ++idx)
    {
        std::cout << '-';
    }
    std::cout << std::endl;
    for (int idx = 0; idx < circumference; ++idx)
    {
        if (idx == test_place)
            std::cout << '^';
        else if (idx == place_a)
            std::cout << 'a';
        else if (idx == place_b)
            std::cout << 'b';
        else
            std::cout << ' ';
    }
    std::cout << std::endl;
    std::cout << "left:" << std::endl;
    std::cout << std::string(distance_left, '-') << std::endl;
    std::cout << "right:" << std::endl;
    std::cout << std::string(distance_right, '-') << std::endl;
}
