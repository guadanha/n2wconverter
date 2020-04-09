#include <string>
#include <iostream>
#include <unistd.h>
#include "client_reply.h"

static void client_reply_send_msg_ack(std::string status, client_reply_request_t* request) {
    std::string data = "HTTP/1.1 200 OK\r\n\r\n { \"extenso\": \"" + status +"\" }\n";
    int n = sendto(request->sock_fd, (void *)data.c_str(), data.length(), 0, \
        (const struct sockaddr *)&request->from, request->fromlen);
    if (n < 0) {
        std::cout << "Cannot send ack_packet" << std::endl;
    }
}

void client_reply_handle(client_reply_request_t* request) {
    try {

        std::string print((char *)request->buf);
        std::cout << "buffer " << print << std::endl;
        client_reply_send_msg_ack("vinte e quatro", request);
    } catch (const std::exception& e) {
        std::cout << "Cannot send resply" << std::endl;
    }
    shutdown(request->sock_fd, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(request->sock_fd);
    free(request);
}
