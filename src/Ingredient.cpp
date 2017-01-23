#include "Ingredient.hpp"
#include <fstream>

std::ostream& operator<<(std::ostream& os, const Ingredient& ingr) {
    if (ingr.quantity == 0 && ingr.unit == "")
        return os << ingr.name;
    else {
        return os << std::to_string(ingr.quantity) << " " << ingr.unit << " " << ingr.name;
    }
}