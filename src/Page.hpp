#ifndef Page_h
#define Page_h
#include "hpdf.h"
#include "TxtFile.hpp"

class PageSet;
class Fonts;
//class TxtFile;

class Page {
public:
    Page(HPDF_Doc& pdf, const TxtFile& recipe, const Fonts& fonts, const PageSet& page_settings);
    
private:
    void writeToPage(const TxtFile& recipe, const HPDF_Page& page);
    void wrapText(const HPDF_Page& page, const std::string& line);
    float setMaxWidth(const HPDF_Page& page);
    
    void pageBreak(const HPDF_Page& page);
    void addPage(const TxtFile& recipe, const Fonts& fonts, const PageSet& page_settings);
    
    void printPageCount(const HPDF_Page& page);
    void drawCentreStroke(const HPDF_Page& page);
    
    const HPDF_Doc* pdf;
    const Fonts* fonts;
    const PageSet* pageSet;
    
    const int Y_PADDING { 50 };
    const int X_PADDING { 20 };
};

#endif /* Page_h */
