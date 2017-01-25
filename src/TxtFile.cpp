#include "TxtFile.hpp"
#include <fstream>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const TxtFile& t)
{
    if (t.begin() == t.end()) { std::cerr << "Error: Empty text file\n"; return os; }
    
    for (auto paragraph = t.begin(); paragraph != t.end(); ++paragraph) {
        for (auto line : *paragraph)
            os << line.line << std::endl;
        os << std::endl;
    }
    return os;
}

TxtFile::TxtFile(const std::string& src_file)
{
    std::ifstream is {src_file};
    if (!is)
        std::cerr << "Error: Unable to open " << src_file << std::endl;
    
    Paragraph p;
    for (std::string line; getline(is, line);) {
        // Add lines to paragraph until newline is found
        // N.B. only the first two lines are checked for (sub)headers
        while (!line.empty()) {
            if (line.substr(0, 2) == "\\T") {
                p.push_back(Line{line.substr(2), true, false});
            }
            else if (line.substr(0, 2) == "\\S") {
                p.push_back(Line{line.substr(2), false, true});
            }
            else if (line.substr(0, 2) == "\\B") {
                p.push_back(Line{line.substr(2), false, false, true});
                break;
            }
            else p.push_back(Line{line});
            
            getline(is, line);
            if (is.eof()) break;
        }
        // Newline: Start new paragraph
        sections.push_back(p);
        p.clear();
    }
}
