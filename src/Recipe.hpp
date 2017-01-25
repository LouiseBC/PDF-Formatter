#ifndef Recipe_hpp
#define Recipe_hpp

#include "TxtFile.hpp"

class Recipe : public TxtFile {
    std::string src_imagepath;
public:
    Recipe(std::string src, std::string img = std::string()) : TxtFile(src), src_imagepath(img) {}
};

#endif
