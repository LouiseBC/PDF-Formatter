#include <iostream>
#include <list>
#include "PDF.hpp"
#include "Txtfile.hpp"

int main()
{
    TxtFile orzo("assets/text/orzo.txt");
    TxtFile lorem("assets/text/lorem.txt");
    TxtFile alice("assets/text/alice.txt");
    std::list<TxtFile> recipes { lorem, alice, orzo };
    
    PDF pdf("New", "assets/fonts/Helvetica-Light.ttf", "assets/fonts/Didot-Bold.ttf", PageSet());
    
    pdf.make(recipes);
    
}