#include "Page.hpp"
#include <memory>
#include <iostream>

///////////////////////
// Helper Functions ///
///////////////////////

void wrapText(std::vector<std::string>& lines, std::string line, const int& maxchars)
{
    // Break line at the end of a word
    if (line.size() > maxchars) {
        int breakindex = maxchars;
        while (line[breakindex] != ' ')
            --breakindex;
        
        // Keep breaking up line until it fits
        std::string temp;
        for (int i = 0; i <= breakindex; ++i)
            temp += line[i];
        lines.push_back(temp);
        line.erase(0, breakindex+1);
        wrapText(lines, line, maxchars);
    }
    // line fits, push back and return.
    else lines.push_back(line);
}

void getScaleAndPosition(unsigned int& W, unsigned int& H, unsigned int& X,
                         const unsigned int& maxwidth, const unsigned int& maxheight)
{
    float ratio = (maxheight/H) + ((maxheight%H) * 0.001);
    W = W * ratio;
    H = maxheight;
    X = (maxwidth - W) /2;
}

/////////////////////
// Page Functions ///
/////////////////////

Page::Page(HPDF_Doc& pdf, const Recipe& recipe, const Fonts& font) {
    // Update page count
    static int page_count = 0;
    ++page_count;
    
    // Get document fonts
    fonts = &font;
    
    // Create new page
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_US5x7, HPDF_PAGE_LANDSCAPE);
    
    writeBody(recipe, page);
    if (recipe.imagefilepath != std::string())
        pasteImage(recipe.imagefilepath, pdf, page);
    
    // Add centre stroke
    HPDF_Page_SetLineWidth (page, 0);
    HPDF_Page_MoveTo (page, HPDF_Page_GetWidth(page)/2, HPDF_Page_GetHeight(page)- 4*fonts->TITLE_SIZE - 25);
    HPDF_Page_LineTo  (page, HPDF_Page_GetWidth(page)/2, PADDING);
    HPDF_Page_Stroke (page);
    
    // Print page count
    HPDF_Page_MoveTo(page, HPDF_Page_GetWidth(page)/2+HPDF_Page_GetWidth(page)/4, HPDF_Page_GetHeight(page)-50);
    HPDF_Page_ShowText(page, std::to_string(page_count).c_str());
}

void Page::writeBody(const Recipe& recipe, const HPDF_Page& page)
{
    HPDF_Page_BeginText(page);
    
    writeTitles(recipe.title, recipe.serving_size.c_str(), page);
    writeIngredients(recipe.ingredients, page);
    writeMethod(recipe.method, page);
    
    HPDF_Page_EndText(page);
}

void Page:: writeTitles(const std::string title, const char* subtitle, const HPDF_Page& page)
{
    // Set title propertie and position
    HPDF_Page_SetFontAndSize(page, fonts->bold, fonts->TITLE_SIZE);
    HPDF_Page_MoveTextPos(page, HPDF_Page_GetWidth(page)/2 + PADDING/4, HPDF_Page_GetHeight(page) - 2*fonts->TITLE_SIZE);
    
    // Wrap title to appropriate page/section
    std::vector<std::string> lines;
    wrapText(lines, title, TITLE_MAX_CHARS);
    
    // Print each line of title to page
    for (int i = 0; i < lines.size(); ++i) {
        HPDF_Page_ShowText(page, lines[i].c_str());
        HPDF_Page_MoveTextPos(page, 0, -fonts->TITLE_SIZE+4);
    }
    // Print subtitle to page
    HPDF_Page_MoveTextPos(page, 0, fonts->SUBTITLE_SIZE/2);
    HPDF_Page_SetFontAndSize(page, fonts->regular, fonts->SUBTITLE_SIZE);
    HPDF_Page_ShowText(page, subtitle);
}

void Page::writeIngredients(const std::vector<Ingredient>& ingredients, const HPDF_Page &page)
{
    HPDF_Page_SetFontAndSize(page, fonts->bold, fonts->BODY_SIZE+1);
    HPDF_Page_MoveTextPos(page, 0, - 2*fonts->SUBTITLE_SIZE);
    HPDF_Page_ShowText(page, "Ingredients:");
    
    HPDF_Page_SetFontAndSize(page, fonts->regular, fonts->BODY_SIZE);
    HPDF_Page_MoveTextPos(page, 0, -fonts->BODY_SIZE);
    for (int i = 0; i < ingredients.size(); ++i) {
        HPDF_Page_ShowText(page, ingredients[i].name.c_str());
        HPDF_Page_MoveTextPos(page, 0, - fonts->BODY_SIZE );
    }
}

void Page::writeMethod(const std::vector<std::string>& method, const HPDF_Page &page)
{
    HPDF_Page_MoveTextPos(page, 0, - fonts->SUBTITLE_SIZE);
    HPDF_Page_SetFontAndSize(page, fonts->bold, fonts->BODY_SIZE+1);
    HPDF_Page_ShowText(page, "Method:");
    
    HPDF_Page_SetFontAndSize(page, fonts->regular, fonts->BODY_SIZE);
    HPDF_Page_MoveTextPos(page, 0, -fonts->BODY_SIZE);
    
    // Add line breaks at correct position
    std::vector<std::string> lines;
    for (int i = 0; i < method.size(); ++i) {
        wrapText(lines, method[i], BODY_MAX_CHARS);
        lines.push_back("");
    }
    
    for (int i = 0; i < lines.size(); ++i) {
        HPDF_Page_ShowText(page, lines[i].c_str());
        HPDF_Page_MoveTextPos(page, 0, -fonts->BODY_SIZE);
    }
}

void Page::pasteImage(const std::string &imagefilepath, const HPDF_Doc& doc, const HPDF_Page& page)
{
    std::string copy = imagefilepath;
    HPDF_Image photo { HPDF_LoadJpegImageFromFile(doc, copy.c_str()) };
    if (!photo)
        std::cerr << "Unable to load image \"" << imagefilepath << "\"\n" << HPDF_GetError(doc);
    unsigned int W {HPDF_Image_GetWidth(photo)};
    unsigned int H {HPDF_Image_GetHeight(photo)};
    unsigned int X {0};
    getScaleAndPosition(W, H, X, HPDF_Page_GetWidth(page)/2, HPDF_Page_GetHeight(page));
    HPDF_Page_DrawImage(page, photo, X, 0, W, H);
}