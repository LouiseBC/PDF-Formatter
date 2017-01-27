#ifndef TxtFile_hpp
#define TxtFile_hpp

#include "Line.hpp"

using Paragraph = std::vector<Line>;

class TxtFile {
private:
    std::vector<Paragraph> sections;
public:
    // Construct Txtfile from file
    TxtFile(const std::string& src_file);
    
    // Copy a portion of another TxtFile
    TxtFile(std::vector<Paragraph>::const_iterator begin,
            std::vector<Paragraph>::const_iterator end,
            Paragraph::const_iterator par_begin,
            Paragraph::const_iterator par_end);
    
    const auto begin() const { return sections.begin(); }
    const auto end() const { return sections.end(); }
    auto begin() { return sections.begin(); }
    auto end() { return sections.end(); }
};

std::ostream& operator<<(std::ostream& os, const TxtFile& t);

#endif