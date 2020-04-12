#ifndef _CLIENT_REPLY_H_
#define _CLIENT_REPLY_H_
#include <netinet/in.h>
#include <sys/socket.h>

#define CLIENT_REPLY_MSG_LEN 1088

extern "C" {

typedef struct {
    int sock_fd;
    socklen_t fromlen;
    struct sockaddr_in from;
    uint8_t buf[CLIENT_REPLY_MSG_LEN];
    uint16_t buf_len;
} client_reply_request_t;

}

void client_reply_handle(client_reply_request_t* request);

#endif // _CLIENT_REPLY_H_
