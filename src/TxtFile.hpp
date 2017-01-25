#ifndef TxtFile_hpp
#define TxtFile_hpp

#include <vector>
#include "Line.hpp"

class TxtFile {
private:
    using Paragraph = std::vector<Line>;
    std::vector<Paragraph> sections;
public:
    TxtFile(const std::string& src_file);
    
    const auto begin() const { return sections.begin(); }
    const auto end() const { return sections.end(); }
};

std::ostream& operator<<(std::ostream& os, const TxtFile& t);

#endif