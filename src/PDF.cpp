#include "PDF.hpp"
#include <math.h>
#include <iostream>
#include <memory>

//////////////////////////
// PDF Class Functions ///
//////////////////////////

PDF::PDF(const std::string& doc_title, const std::string& src_reg, const std::string& src_bold)
{
    doc_name = doc_title + ".pdf";
    src_font_reg = src_reg;
    src_font_bold = src_bold;
}

void PDF::make(const std::vector<Recipe>& recipes)
{
    // Set up new pdf document & load fonts
    HPDF_Doc pdf = create_new();
    
    // Add pages to document
    for (auto recipe : recipes)
        Page newpage(pdf, recipe, fonts);
    
    // Save to PDF
    HPDF_SaveToFile(pdf, doc_name.c_str());
    HPDF_Free(pdf);
}

HPDF_Doc PDF::create_new()
{
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf)
        std::cerr << "Error: Cannot create pdf\n";
    
    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
    HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_OUTLINE);
    
    // Set fonts for this pdf
    const char* regfont = HPDF_LoadTTFontFromFile(pdf, src_font_reg.c_str(), HPDF_TRUE);
    const char* boldfont = HPDF_LoadTTFontFromFile(pdf, src_font_bold.c_str(), HPDF_TRUE);
    fonts.bold = HPDF_GetFont(pdf, boldfont, NULL);
    fonts.regular = HPDF_GetFont(pdf, regfont, NULL);
    
    return std::move(pdf);
}
