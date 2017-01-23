#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Ingredient.hpp"
#include "Recipe.hpp"
#include "PDF.hpp"

int main() {
    Recipe ottolenghi("notes.txt", "assets/image2.jpg");
    
    std::vector<Recipe> recipes {ottolenghi};
    
    PDF pdf("MyRecipes", "assets/Helvetica-Light.ttf","assets/Didot-Bold.ttf");
    pdf.make(recipes);
}