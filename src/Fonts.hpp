#ifndef Fonts_hpp
#define Fonts_hpp
#include "hpdf.h"

class Fonts {
public:
    void setRegular(HPDF_Font f) { regular_font = f; }
    void setBold(HPDF_Font f) { bold_font = f; }
    
    HPDF_Font regular() const { return regular_font; }
    HPDF_Font bold() const { return bold_font; }
    
    const int title_size() const { return TITLE_SIZE; }
    const int subtitle_size() const { return SUBTITLE_SIZE; }
    const int body_size() const { return BODY_SIZE; }
private:
    HPDF_Font regular_font;
    HPDF_Font bold_font;
    
    const int TITLE_SIZE { 20 };
    const int SUBTITLE_SIZE { 11 };
    const int BODY_SIZE { 8 };
    
    const int TITLE_MAX_CHARS { 26 };
    const int BODY_MAX_CHARS { 63 };
};

namespace Font {
    enum type { title = 0, subtitle, body };
}

#endif