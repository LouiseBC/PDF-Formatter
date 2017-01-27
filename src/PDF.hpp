#ifndef PDF_hpp
#define PDF_hpp
#include <list>
#include "Fonts.hpp"
#include "PageSet.hpp"

class TxtFile;

class PDF {
public:
    PDF(const std::string& doc_title,
        const std::string& src_font_reg,
        const std::string& src_font_bold,
        const PageSet& page_settings);
    
    void make(const std::list<TxtFile>& recipes);
    void insert_file(const TxtFile& file, std::list<TxtFile>::const_iterator position);
    auto begin() const { return recipes.begin(); }
    
private:
    HPDF_Doc create_new();
    
    std::list<TxtFile> recipes;
    
    std::string doc_title;
    std::string src_font_reg;
    std::string src_font_bold;
    
    Fonts fonts;
    const PageSet page_settings;
};

#endif
