#include <string>
#include <iostream>
#include <unistd.h>
#include <regex>

#include "client_reply.h"
#include "log.h"
#include "parser.h"

static void client_reply_send_msg_ack(std::string status, client_reply_request_t* request);
static std::string client_reply_get_number(std::string buf);
static bool is_number(const std::string &str);

static void client_reply_send_msg_ack(std::string status, client_reply_request_t* request) {
    std::string data = "HTTP/1.1 200 OK\r\n\r\n { \"extenso\": \"" + status +"\" }\n";
    int n = sendto(request->sock_fd, (void *)data.c_str(), data.length(), 0, \
        (const struct sockaddr *)&request->from, request->fromlen);
    if (n < 0) {
        LOG_ERROR("client_reply") << "Cannot send ack_packet" << std::endl;
    }
}

static std::string client_reply_get_number(std::string buf) {
    parser::ParserNumber* numb_word;
    std::string word =  "Error";
    int number;
    try {
        LOG_DEBUG("client_reply") << "recv: " << buf << std::endl;
        if (buf.find("GET /") == std::string::npos) {
            LOG_ERROR("client_reply") << "HTTP GET command error" << std::endl;
            return "GET cmd error";
        };
        buf.erase(0, 5);
        std::string value = buf.substr(0, buf.find(" "));

        if (!is_number(value)) {
            LOG_ERROR("client_reply") << "Data is not a number" << std::endl;
            return "Invalid data";
        }
        number = std::stoi(value);
        if (number > 99999 || number < -99999) {
            LOG_ERROR("client_reply") << "Invalid data range" << std::endl;
            return "Invalid range";
        }

        numb_word = new parser::ParserNumber(number);
        word = numb_word->GetPt();
        delete numb_word;
    }
    catch (const std::invalid_argument& ie) {
        LOG_ERROR("client_reply") << "invalid argument " << ie.what() << std::endl;
    }
    catch (const std::exception& e) {
        LOG_ERROR("client_reply") << "Caught " << e.what() << std::endl;
    }

    return word;
}

static bool is_number(const std::string &str) {
  return !str.empty() && str.find_first_not_of("-0123456789") == std::string::npos;
}

void client_reply_handle(client_reply_request_t* request) {
    int ret;
    std::string reply;
    try {
        ret = recvfrom(request->sock_fd, (char *)request->buf, CLIENT_REPLY_MSG_LEN, 0,
            (struct sockaddr *)&request->from, &request->fromlen);

        if(ret < 0) {
            LOG_ERROR("server") << "Problem with recvfrom discard!!!!"
                << std::endl;
            throw std::runtime_error("recvfrom discard");;
        }
        request->buf_len = ret;
        std::string rcv_msg((char *)request->buf);

        reply = client_reply_get_number(rcv_msg);
        client_reply_send_msg_ack(reply, request);
    }
    catch (const std::exception& e) {
        LOG_ERROR("client_reply") << "Caught " << e.what() << std::endl;
    }

    shutdown(request->sock_fd, SHUT_RDWR);
    close(request->sock_fd);
    free(request);
}
