#include <iostream>
#include <stdexcept>
#include <thread>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "client_reply.h"

bool stop = true;

Server::Server(int port) {
    std::cout << "Welcome to Server UDP Service!" << std::endl;

    _sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock_fd < 0) {
        throw std::invalid_argument("server.cc: Cannot open socket");
    }

    int optval = 1;
    setsockopt(_sock_fd, SOL_SOCKET, SO_REUSEADDR,
        (const void *)&optval , sizeof(int));

    struct sockaddr_in serveraddr;
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons((uint16_t)port);

    if (bind(_sock_fd, (struct sockaddr *) &serveraddr,
        sizeof(serveraddr)) < 0) {
        throw std::invalid_argument("server.cc: Error on binding");
    }

    if (listen (_sock_fd, 1000) != 0) {
        throw std::invalid_argument("server.cc: Error on listen");
    }
}

Server::~Server(void) {
    close(_sock_fd);
}

void Server::Main(void) {
    int n;
    while (stop) {
        client_reply_request_t *request = (client_reply_request_t*) malloc(sizeof(client_reply_request_t));
        memset(request, 0, sizeof(client_reply_request_t));
        request->fromlen = sizeof(struct sockaddr_in);
        request->sock_fd = accept (_sock_fd, (struct sockaddr *)&request->from,\
                &request->fromlen);
        if(n < 0) {
            std::cout << "Problem with accept discard!!!!"
                << std::endl;
            free(request);
            continue;
        }
        n = recvfrom(request->sock_fd, (char *)request->buf, CLIENT_REPLY_MSG_LEN, 0,
            (struct sockaddr *)&request->from, &request->fromlen);

        if(n < 0) {
            std::cout << "Problem with recvfrom discard!!!!"
                << std::endl;
            free(request);
            continue;
        }
        request->buf_len = n;
        std::thread(client_reply_handle, request).detach();
    }
}

void handle_signal(int) {
    stop = false;
}

void usage() {
    std::cout << "Server TCP Service" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t-h: show help" << std::endl;
}

int main(int argc, char *argv[]) {
    int opt, ret = 0;
    std::string file;

    struct sigaction sig_handler;
    sig_handler.sa_handler = handle_signal;
    sigemptyset(&sig_handler.sa_mask);
    sig_handler.sa_flags = 0;
    sigaction(SIGINT, &sig_handler, NULL);

    Server* server = nullptr;
    try {
        while ((opt = getopt(argc, argv, "hf:")) != -1) {
            switch (opt) {
            case 'h':
                usage();
                return 0;
            default:
                break;
            }
        }

        server = new Server(3000);
        server->Main();
        ret = 0;
    } catch (const std::exception& e) {
        std::cout << std::endl;
        ret = 1;
    }
    if (server) delete server;
    std::cout << "Term program" << std::endl;
    return ret;
}
