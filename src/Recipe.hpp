#ifndef Recipe_hpp
#define Recipe_hpp
#include <string>
#include <vector>
#include "Ingredient.hpp"

class Recipe {
public:
    Recipe(const std::string& sourcefile, const std::string& imagefile = std::string());

    std::string title;
    std::string serving_size;
    std::vector<Ingredient> ingredients;
    std::vector<std::string> method;
    const std::string imagefilepath;
private:
    std::vector<Ingredient> read_ingredients(const std::string& sourcefile);
    std::vector<std::string> read_method(const std::string& sourcefile);
    void read_title_and_servings(const std::string& sourcefile, std::string& title, std::string& servings);
};

#endif
