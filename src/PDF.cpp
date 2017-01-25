#include <iostream>
#include "PDF.hpp"
#include "TxtFile.hpp"
#include "Page.hpp"

PDF::PDF(const std::string& doc_name, const std::string& src_reg, const std::string& src_bold, const PageSet& page_set)
: doc_title{doc_name+".pdf"}, src_font_reg{src_reg}, src_font_bold{src_bold}, page_settings{page_set} {}


void PDF::make(const std::vector<TxtFile>& recipes)
{
    // Set up new pdf document & load fonts
    HPDF_Doc pdf = create_new();
    
    // Add pages for each recipe (without storing)
    for (auto recipe : recipes)
        Page newpage{pdf, recipe, fonts, page_settings};
    
    // Save to PDF
    HPDF_SaveToFile(pdf, doc_title.c_str());
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
    fonts.setBold(HPDF_GetFont(pdf, boldfont, NULL));
    fonts.setRegular(HPDF_GetFont(pdf, regfont, NULL));
    
    return std::move(pdf);
}
