#ifndef Line_hpp
#define Line_hpp
#include <string>
#include <vector>

// Simple struct to keep track of whether a line is a heading/subheading
struct Line {
    std::string line;
    bool title;
    bool subtitle;
    bool pageBreak;
};

#endif