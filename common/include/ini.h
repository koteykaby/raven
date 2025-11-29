// sexyini.h - ultra minimalistic .ini parser
#pragma once

#include <unordered_map>
#include <string>
#include <fstream>

class ini {
public:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
    void parse(const std::string& filename);
private:
    std::string trim(const std::string& str);
};