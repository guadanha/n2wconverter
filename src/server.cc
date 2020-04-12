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

#include "log.h"
#include "server.h"
#include "client_reply.h"

bool stop = true;

Server::Server(std::string file) {
    LOG_INFO("server") << "Welcome to Server UDP Service!" << std::endl;
    _config = new config::ConfigFile(file);
    if (!test_config()) {
        throw std::invalid_argument("server.cc: Config file error");
    }
    _log_file = 0;
    LOGLog::reporting_level(LOGLog::DEBUG);
    if (_config->Get("log_file").compare("no")) {
        _log_file = fopen(_config->Get("log_file").c_str(), "a+");
        LOGLog::output_stream(_log_file);
    }

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
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((uint16_t)std::stoi(_config->Get("port")));

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
    delete _config;
}

void Server::Main(void) {
    while (stop) {
        client_reply_request_t *request = (client_reply_request_t*) malloc(sizeof(client_reply_request_t));
        memset(request, 0, sizeof(client_reply_request_t));
        request->fromlen = sizeof(struct sockaddr_in);
        request->sock_fd = accept (_sock_fd, (struct sockaddr *)&request->from,\
                &request->fromlen);
        if(request->sock_fd < 0) {
            LOG_ERROR("server") << "Problem with accept discard!!!!"
                << std::endl;
            free(request);
            continue;
        }
        std::thread(client_reply_handle, request).detach();
    }
}

bool Server::test_config(void) {
    if (_config->Get("port").size() == 0) return false;
    if (_config->Get("log_file").size() == 0) return false;
    return true;
}

void handle_signal(int) {
    stop = false;
}

void usage() {
    std::cout << "Server Service" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t-h: show help" << std::endl;
    std::cout << "\t-f <file>: configuration file" << std::endl;
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
            case 'f':
                file = optarg;
                break;
            default:
                usage();
                return 1;
            }
        }

        server = new Server(file);
        server->Main();
        ret = 0;
    } catch (const std::exception& e) {
        LOG_ERROR("server") << "Caught " << e.what() << std::endl;
        ret = 1;
    }
    if (server) delete server;
    LOG_DEBUG("server") << "Term program" << std::endl;
    return ret;
}
