#ifndef _SERVER_H_
#define _SERVER_H_
#include <cstdio>

#include "config_file.h"

class Server {
  public:
    Server(std::string file);
    ~Server(void);

    void Main(void);

  private:
    bool test_config(void);

    config::ConfigFile* _config;

    int _sock_fd;
    FILE* _log_file;
};

#endif // _SERVER_H_
