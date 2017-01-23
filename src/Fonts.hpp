#ifndef Fonts_hpp
#define Fonts_hpp
#include "hpdf.h"

struct Fonts {
    HPDF_Font regular;
    HPDF_Font bold;
    
    const int TITLE_SIZE { 20 };
    const int SUBTITLE_SIZE { 11 };
    const int BODY_SIZE { 8 };
};

#endif
