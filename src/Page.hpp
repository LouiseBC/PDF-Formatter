#ifndef Page_h
#define Page_h
#include "hpdf.h"
#include "Recipe.hpp"
#include "Fonts.hpp"

class Page {
    // Eventually calculate data within fonts struct
    const Fonts* fonts;
    const int TITLE_MAX_CHARS { 26 };
    const int BODY_MAX_CHARS { 63 };
    const int PADDING { 50 };
    
    void writeBody(const Recipe& recipe, const HPDF_Page& page);
    void writeTitles(const std::string title, const char* subtitle, const HPDF_Page& page);
    void writeIngredients(const std::vector<Ingredient>& ingredients, const HPDF_Page& page);
    void writeMethod(const std::vector<std::string>& method, const HPDF_Page &page);
    void pasteImage(const std::string& imagefilepath, const HPDF_Doc& doc, const HPDF_Page& page);
    
public:
    Page(HPDF_Doc& pdf, const Recipe& recipe, const Fonts& fonts);
};

#endif /* Page_h */
