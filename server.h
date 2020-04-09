#ifndef _SERVER_H_
#define _SERVER_H_
#include <cstdio>

class Server {
  public:
    Server(int port);
    ~Server(void);

    void Main(void);

  private:

    int _sock_fd;
};

#endif // _SERVER_H_
