#ifndef PageSet_h
#define PageSet_h
#include "hpdf.h"

struct PageSet {
    // Will eventually provide support for different page types
    HPDF_PageSizes size;
    HPDF_PageDirection orientation;
    
    PageSet(HPDF_PageSizes sz, HPDF_PageDirection orient)
    : size{sz}, orientation{orient} {}
    
    PageSet()
    : size{HPDF_PAGE_SIZE_US5x7}, orientation{HPDF_PAGE_LANDSCAPE} {}
};

#endif