#include <string>
#include <iostream>
#include <unistd.h>

#include "client_reply.h"
#include "parser.h"

static void client_reply_send_msg_ack(std::string status, client_reply_request_t* request) {
    std::string data = "HTTP/1.1 200 OK\r\n\r\n { \"extenso\": \"" + status +"\" }\n";
    int n = sendto(request->sock_fd, (void *)data.c_str(), data.length(), 0, \
        (const struct sockaddr *)&request->from, request->fromlen);
    if (n < 0) {
        std::cout << "Cannot send ack_packet" << std::endl;
    }
}

void client_reply_handle(client_reply_request_t* request) {
    parser::ParserNumber* numb_word;
    try {
        std::string print((char *)request->buf);
        std::cout << "buffer " << print << std::endl;
        if (print.find("GET /") == std::string::npos) {
            std::cout << "no get command" << std::endl;
        };
        print.erase(0, 5);
        std::string value = print.substr(0, print.find(" "));

        std::cout << "buffer: " << value << " int: " << std::stoi(value) << std::endl;
        numb_word = new parser::ParserNumber(std::stoi(value));

        client_reply_send_msg_ack(numb_word->GetPt(), request);
        delete numb_word;
    }
    catch (const std::invalid_argument& ie) {
        std::cout << "invalid argument " << ie.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Caught " << e.what() << std::endl;
    }

    shutdown(request->sock_fd, SHUT_RDWR);
    close(request->sock_fd);
    free(request);
}
