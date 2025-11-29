#include "ini.h"

void ini::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
        
    std::string current_section = "Global";
    std::string line;
        
    while (std::getline(file, line)) {
        line = trim(line);
            
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;
            
        if (line[0] == '[' && line.back() == ']') {
            current_section = trim(line.substr(1, line.length() - 2));
        }
        else if (size_t pos = line.find('='); pos != std::string::npos) {
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));
            ini::data[current_section][key] = value;
        }
    }
}

std::string ini::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}