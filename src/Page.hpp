#ifndef Page_h
#define Page_h
#include "hpdf.h"
#include "TxtFile.hpp"

class PageSet;
class Fonts;
class PDF;
class TxtFile;

class Page {
public:
    Page(PDF* currdoc, HPDF_Doc* pdf, const TxtFile& recipe, const Fonts* fonts, const PageSet* page_settings);
    
private:
    void writeToPage(const TxtFile& recipe, const HPDF_Page& page);
    
    void wrapText(const HPDF_Page& page,
                  const Line& line,
                  TxtFile& file,
                  std::vector<Paragraph>::iterator curr,
                  Paragraph::iterator position);
    
    float setMaxWidth(const HPDF_Page& page);
    
    bool moveCursor(const HPDF_Page& page);
    
    void pageBreak(const HPDF_Page &page,
                   std::vector<Paragraph>::const_iterator begin,
                   std::vector<Paragraph>::const_iterator end,
                   Paragraph::const_iterator par_begin,
                   Paragraph::const_iterator par_end);
    
    void setFont(const HPDF_Page& page, int type);
    
    void printPageCount(const HPDF_Page& page);
    
    void drawCentreStroke(const HPDF_Page& page);
    
    HPDF_Doc* pdf;
    PDF* currdoc;
    const Fonts* fonts;
    const PageSet* pageSet;
    int pageNum;
    
    const int Y_PADDING { 50 };
    const int X_PADDING { 20 };
};

#endif /* Page_h */
