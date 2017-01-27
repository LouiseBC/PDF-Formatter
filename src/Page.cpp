#include <iostream>
#include "Page.hpp"
#include "PDF.hpp"

Page::Page(PDF* current_doc, HPDF_Doc* pdf, const TxtFile& file, const Fonts* font, const PageSet* page_settings) {
    // Get document info
    fonts = font;
    pageSet = page_settings;
    currdoc = current_doc;
    
    // Create new page
    HPDF_Page page = HPDF_AddPage(*pdf);
    HPDF_Page_SetSize(page, page_settings->size, page_settings->orientation);
    
    // This needs to be here because page count function can't be moved before writetopage...
    static int page_count = 0;
    pageNum = ++page_count;
    
    if (pageSet->orientation == HPDF_PAGE_LANDSCAPE)
        drawCentreStroke(page);
    
    writeToPage(file, page);
    //if (recipe.imagefilepath != std::string()) && writeToPage == true
    //    pasteImage(recipe.imagefilepath, pdf, page);
    printPageCount(page);
}

void Page::writeToPage(const TxtFile& original_file, const HPDF_Page& page)
{
    TxtFile file = original_file;
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, X_PADDING, HPDF_Page_GetHeight(page)-Y_PADDING);
    
    for (auto p = file.begin(); p != file.end(); ++p) {
        
        for (auto ln = p->begin(); ln != p->end(); ++ln) {
            
            // Check if line indicates a page break: '\B'
            if (ln->pageBreak == true) {
                if(!moveCursor(page)) {
                    if (std::next(p) != file.end()) {
                        pageBreak(page, std::next(p), file.end(), std::next(ln), p->end());
                        return;
                    }
                }
            }
            
            // Set correct font
            if (ln == p->begin() || ln == std::next(p->begin())) {
                if (ln->title == true) setFont(page, Font::title);
                else if (ln->subtitle == true) setFont(page, Font::subtitle);
                else setFont(page, Font::body);
            }
            else setFont(page, Font::body);

            // Print line to page
            wrapText(page, *ln, file, p, ln);
            
            // End of page?
            if (HPDF_Page_GetCurrentTextPos(page).y <= Y_PADDING/2) {
                if(!moveCursor(page)) {
                    if (std::next(p) != file.end()) {
                        pageBreak(page, std::next(p), file.end(), std::next(ln), p->end());
                        return;
                    }
                }
            }
        }
        if (HPDF_Page_GetCurrentTextPos(page).y < HPDF_Page_GetHeight(page)-Y_PADDING)
            HPDF_Page_MoveToNextLine(page);
    }
    HPDF_Page_EndText(page);
}

void Page::wrapText(const HPDF_Page& page,
                    const Line& line,
                    TxtFile& file,
                    std::vector<Paragraph>::iterator currP,
                    Paragraph::iterator currLn)
{
    // Sets max depending on page orientation
    static HPDF_REAL max_width = setMaxWidth(page);
    HPDF_REAL line_width = HPDF_Page_TextWidth(page, line.line.c_str());
    
    if (line_width > max_width) {
        // Calculate roughly how many chars are allowed on the page
        double avg_char_width = line_width/line.line.size();
        unsigned int max_chars = max_width / avg_char_width;
        // Find a suitable line break
        size_t pos = line.line.rfind(' ', max_chars);
        HPDF_Page_ShowText(page, line.line.substr(0, pos).c_str());
        HPDF_Page_MoveToNextLine(page);
        
        // End of page:
        if (HPDF_Page_GetCurrentTextPos(page).y <= Y_PADDING/2) {
            if (pageSet->orientation == HPDF_PAGE_LANDSCAPE && HPDF_Page_GetCurrentTextPos(page).x == X_PADDING)
                moveCursor(page);
            
            else { /*
             Line remainingline = line;
             remainingline.line = line.line.substr(pos+1);
             currP->insert(std::next(currLn), remainingline);
             return; */
            }
        }
        // Wrap remainder of line
        wrapText(page, line.line.substr(pos+1), file, currP, currLn);
    }
    else {
        HPDF_Page_ShowText(page, line.line.c_str());
        HPDF_Page_MoveToNextLine(page);
    }
}

float Page::setMaxWidth(const HPDF_Page& page)
{
    float temp;
    if (pageSet->orientation == HPDF_PAGE_LANDSCAPE)
        temp = (HPDF_Page_GetWidth(page)/2 - X_PADDING*2);
    else
        temp = HPDF_Page_GetWidth(page) - X_PADDING*2;
    return temp;
}

bool Page::moveCursor(const HPDF_Page& page)
{
    // Landscape page: Move to next half
    if (pageSet->orientation == HPDF_PAGE_LANDSCAPE && HPDF_Page_GetCurrentTextPos(page).x == X_PADDING) {
        HPDF_Page_MoveTextPos(page,
                              HPDF_Page_GetWidth(page)/2,
                              HPDF_Page_GetHeight(page)-HPDF_Page_GetCurrentTextPos(page).y-Y_PADDING);
        return true;
    }
    return false;
}

void Page::pageBreak(const HPDF_Page &page,
                     std::vector<Paragraph>::const_iterator begin,
                     std::vector<Paragraph>::const_iterator end,
                     std::vector<Line>::const_iterator par_begin,
                     std::vector<Line>::const_iterator par_end)
{
    auto it = currdoc->begin();
    std::advance(it, pageNum);
    currdoc->insert_file(TxtFile{begin, end, par_begin, par_end}, it);
    HPDF_Page_EndText(page);
}

void Page::setFont(const HPDF_Page& page, int type)
{
    switch(type) {
        case Font::title: HPDF_Page_SetFontAndSize(page, fonts->bold(), fonts->title_size()); break;
        case Font::subtitle: HPDF_Page_SetFontAndSize(page, fonts->bold(), fonts->subtitle_size()); break;
        case Font::body: HPDF_Page_SetFontAndSize(page, fonts->regular(), fonts->body_size()); break;
    }
    HPDF_Page_SetTextLeading(page, HPDF_Page_GetCurrentFontSize(page));
}

void Page::printPageCount(const HPDF_Page& page)
{
    
    std::string p_count = "- " + std::to_string(pageNum) + " -";
    
    HPDF_Page_BeginText(page);
    // Messy method of centering the page count - to do
    HPDF_Page_MoveTextPos(page, HPDF_Page_GetWidth(page)/2 - 7, Y_PADDING/2);
    HPDF_Page_ShowText(page, p_count.c_str());
    HPDF_Page_EndText(page);
}

void Page::drawCentreStroke(const HPDF_Page& page)
{
    HPDF_Page_SetLineWidth (page, 0);
    HPDF_Page_MoveTo (page, HPDF_Page_GetWidth(page)/2, HPDF_Page_GetHeight(page) - Y_PADDING);
    HPDF_Page_LineTo (page, HPDF_Page_GetWidth(page)/2, Y_PADDING);
    HPDF_Page_Stroke (page);
}