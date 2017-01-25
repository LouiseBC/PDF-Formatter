#include <iostream>
#include "Page.hpp"
#include "TxtFile.hpp"
#include "Fonts.hpp"
#include "PageSet.hpp"

Page::Page(HPDF_Doc& pdf, const TxtFile& file, const Fonts& font, const PageSet& page_settings) {
    // Get document info
    fonts = &font;
    pageSet = &page_settings;
    
    // Create new page
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, page_settings.size, page_settings.orientation);
    
    writeToPage(file, page);
    //if (recipe.imagefilepath != std::string())
    //    pasteImage(recipe.imagefilepath, pdf, page);
    
    if (pageSet->orientation == HPDF_PAGE_LANDSCAPE)
        drawCentreStroke(page);
    printPageCount(page);
}

void Page::writeToPage(const TxtFile& file, const HPDF_Page& page)
{
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, X_PADDING, HPDF_Page_GetHeight(page)-Y_PADDING);
    
    for (auto p = file.begin(); p != file.end(); ++p) {
        // Set default body font
        HPDF_Page_SetFontAndSize(page, fonts->regular(), fonts->body_size());
        HPDF_Page_SetTextLeading(page, HPDF_Page_GetCurrentFontSize(page));
        
        // For every paragraph: Print lines
        for (auto ln = p->begin(); ln != p->end(); ++ln) {
            // Check for special title/subtitle cases
            if (ln == p->begin() || ln == std::next(p->begin())) {
                if (ln->title == true)
                    HPDF_Page_SetFontAndSize(page, fonts->bold(), fonts->title_size());
                
                else if (ln->subtitle == true)
                    HPDF_Page_SetFontAndSize(page, fonts->bold(), fonts->subtitle_size());
                
                HPDF_Page_SetTextLeading(page, HPDF_Page_GetCurrentFontSize(page));
            }
            if (ln->pageBreak == true) {
                pageBreak(page);
                continue;
            }
            
            // Print line to page
            wrapText(page, ln->line);
            
            // Reset fonts to body default
            if (ln == p->begin() || ln == std::next(p->begin()))
                HPDF_Page_SetFontAndSize(page, fonts->regular(), fonts->body_size());
            
        }
        // to do : moves to next line even after page break.
        HPDF_Page_MoveToNextLine(page);
    }
    
    HPDF_Page_EndText(page);
}

void Page::wrapText(const HPDF_Page& page, const std::string& line)
{
    // Sets max depending on page orientation
    static HPDF_REAL max_width = setMaxWidth(page);
    
    // Calculate roughly how many chars are allowed on the page
    HPDF_REAL line_width = HPDF_Page_TextWidth(page, line.c_str());
    double avg_char_width = line_width/line.size();
    unsigned int max_chars = max_width / avg_char_width;
    
    if (line_width > max_width) {
        // Find a suitable line break
        size_t pos = line.rfind(' ', max_chars);
        HPDF_Page_ShowText(page, line.substr(0, pos).c_str());
        HPDF_Page_MoveToNextLine(page);
        
        // End of page:
        if (HPDF_Page_GetCurrentTextPos(page).y <= Y_PADDING/2)
            pageBreak(page);

        // Wrap remainder of line
        wrapText(page, line.substr(pos+1));
    }
    else {
        HPDF_Page_ShowText(page, line.c_str());
        HPDF_Page_MoveToNextLine(page);
    }
}

float Page::setMaxWidth(const HPDF_Page& page)
{
    float temp;
    if (pageSet->orientation == HPDF_PAGE_LANDSCAPE)
        temp = (HPDF_Page_GetWidth(page)/2 - X_PADDING*1.5); // +half for margin of error ;)
    else
        temp = HPDF_Page_GetWidth(page) - X_PADDING*1.5;
    return temp;
}

void Page::pageBreak(const HPDF_Page &page)
{
    if (pageSet->orientation == HPDF_PAGE_LANDSCAPE && HPDF_Page_GetCurrentTextPos(page).x == X_PADDING)
        HPDF_Page_MoveTextPos(page,
                              HPDF_Page_GetWidth(page)/2,
                              HPDF_Page_GetHeight(page)-HPDF_Page_GetCurrentTextPos(page).y-Y_PADDING);
    // Alternative to do
    //else
    //    addPage();
}

void Page::addPage(const TxtFile &recipe, const Fonts &fonts, const PageSet &page_settings)
{
    
}

void Page::drawCentreStroke(const HPDF_Page& page)
{
    HPDF_Page_SetLineWidth (page, 0);
    HPDF_Page_MoveTo (page, HPDF_Page_GetWidth(page)/2, HPDF_Page_GetHeight(page) - Y_PADDING);
    HPDF_Page_LineTo (page, HPDF_Page_GetWidth(page)/2, Y_PADDING);
    HPDF_Page_Stroke (page);
}

void Page::printPageCount(const HPDF_Page& page)
{
    static int page_count = 0;
    ++page_count;
    std::string p_count = "- " + std::to_string(page_count) + " -";
    
    HPDF_Page_BeginText(page);
    // Messy method of centering the page count - to do
    HPDF_Page_MoveTextPos(page, HPDF_Page_GetWidth(page)/2 - 7, Y_PADDING/2);
    HPDF_Page_ShowText(page, p_count.c_str());
    HPDF_Page_EndText(page);
}