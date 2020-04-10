#include <fstream>
#include <iostream>

#include "config_file.h"

namespace config {

ConfigFile::ConfigFile(std::string file) : _file(file) {
    parse();
}

ConfigFile::~ConfigFile(void) {

}

std::string ConfigFile::Get(std::string key) {
    if (_options.count(key)) return _options[key];
    else return "";
}

void ConfigFile::parse(void) {
    std::ifstream cfgfile(_file, std::ifstream::in);
    std::string line;
    while(std::getline(cfgfile, line)) {
        std::string token;
        std::size_t pos = line.find('=');
        if(pos != std::string::npos) {
            token = line.substr(0, pos);
            line.erase(0, pos + 1);
            _options[token] = line;
        }
    }
    cfgfile.close();

    std::cout << "Configuration:" << std::endl;
    for (std::map<std::string,std::string>::iterator it = _options.begin(); it!= _options.end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;
}

} // namespace config
