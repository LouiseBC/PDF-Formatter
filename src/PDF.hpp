#ifndef PDF_hpp
#define PDF_hpp
#include <vector>
#include "Recipe.hpp"
#include "Page.hpp"
#include "Fonts.hpp"
#include "hpdf.h"

class PDF {
public:
    PDF(const std::string& doc_title, const std::string& src_reg, const std::string& src_bold);
    void make(const std::vector<Recipe>& recipes);
    
private:
    HPDF_Doc create_new();
    
    // Document Constants
    std::string doc_name;
    std::string src_font_reg;
    std::string src_font_bold;
    Fonts fonts;
};

#endif
