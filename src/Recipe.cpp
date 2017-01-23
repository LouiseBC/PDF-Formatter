#include "Recipe.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

/////////////////////////////
// Recipe class functions ///
/////////////////////////////

Recipe::Recipe(const std::string& sourcefile, const std::string& imagefile)
    : imagefilepath{imagefile}
{
    read_title_and_servings(sourcefile, title, serving_size);
    ingredients = read_ingredients(sourcefile);
    method = read_method(sourcefile);
}

void Recipe::read_title_and_servings(const std::string& sourcefile, std::string& title, std::string& servings) {
    std::ifstream is {sourcefile};
    if (!is)
        std::cerr << "Error: unable to open " << sourcefile << std::endl;
    
    getline(is, title);
    getline(is, servings);
}

std::vector<Ingredient> Recipe::read_ingredients(const std::string& sourcefile) {
    // Open ingredients source file
    std::ifstream is {sourcefile};
    if (!is)
        std::cerr << "Error: unable to open " << sourcefile << std::endl;
    
    // Find beginning of ingredient list
    std::string str;
    do {
        getline(is, str);
    } while (!(str.substr(0, 11) == "Ingredients" || str.substr(0, 11) == "ingredients" || is.eof()));
    
    // Read each line as a separate ingredient
    std::vector<Ingredient> recipeingredients;
    while (getline(is, str)) {
        if (!str.empty() && str.substr(0, 6) != "Method" && str.substr(0, 6) != "method")
            recipeingredients.push_back(Ingredient(str));
        else break;
    }
    return recipeingredients;
}

std::vector<std::string> Recipe::read_method(const std::string& sourcefile) {
    // Open ingredients source file
    std::ifstream is {sourcefile};
    if (!is)
        std::cerr << "Error: unable to open " << sourcefile << std::endl;
    
    // Find beginning of method/instructions list
    std::string str;
    do {
        getline(is, str);
    } while (!(str.substr(0, 6) == "Method" || str.substr(0, 6) == "method"));
    
    // Read each line as a paragraph of the method
    std::vector<std::string> recipeinstructions;
    while (getline(is, str)){
        if (!str.empty())
            recipeinstructions.push_back(str);
    }
    return recipeinstructions;
}

