#ifndef _CONFIG_FILE_H_
#define _CONFIG_FILE_H_
#include <string>
#include <map>

namespace config {

class ConfigFile {
  public:
    ConfigFile(std::string file);
    ~ConfigFile(void);
    std::string Get(std::string key);

  private:
    void parse(void);

    std::string _file;
    std::map<std::string, std::string> _options;
};

} // namespace config

#endif // _CONFIG_FILE_H_
