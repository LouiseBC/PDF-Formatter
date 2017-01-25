#ifndef PDF_hpp
#define PDF_hpp
#include <vector>
#include "Fonts.hpp"
#include "PageSet.hpp"

class TxtFile;

class PDF {
public:
    PDF(const std::string& doc_title,
        const std::string& src_font_reg,
        const std::string& src_font_bold,
        const PageSet& page_settings);
    
    void make(const std::vector<TxtFile>& recipes);
    
private:
    HPDF_Doc create_new();
    
    // Document Constants
    std::string doc_title;
    std::string src_font_reg;
    std::string src_font_bold;
    Fonts fonts;
    const PageSet page_settings;
};

#endif
