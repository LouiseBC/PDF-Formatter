#include <iostream>
#include "PDF.hpp"
#include "Txtfile.hpp"

int main()
{
    TxtFile test("assets/text/puttanesca.txt");
    TxtFile test2("assets/text/orzo.txt");
    std::vector<TxtFile> recipes {test2};
    
    PDF pdf("Title", "assets/fonts/Helvetica-Light.ttf", "assets/fonts/Didot-Bold.ttf", PageSet(HPDF_PAGE_SIZE_US5x7, HPDF_PAGE_PORTRAIT));
    
    pdf.make(recipes);
    
    //std::cout << test2;
}