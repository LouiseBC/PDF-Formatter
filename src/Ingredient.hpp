#ifndef Ingredient_hpp
#define Ingredient_hpp
#include <string>

class Ingredient {
public:
    Ingredient(std::string ingr_name)
    : name{ingr_name} {}
    Ingredient(int amount, std::string measure_unit, std::string ingr_name)
    : quantity{amount}, unit{measure_unit}, name{ingr_name} {}

    std::string name;
    int quantity;
    std::string unit;
};

std::ostream& operator<<(std::ostream& os, const Ingredient& ingr);

#endif
