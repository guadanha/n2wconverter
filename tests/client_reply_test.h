#ifndef _CLIENT_REPLY_TEST_H_
#define _CLIENT_REPLY_TEST_H_

#include <client_reply.cc>
#include <gtest/gtest.h>

TEST(ClientTest, CheckClient)
{
    ASSERT_TRUE(is_number("100"));
    ASSERT_FALSE(is_number("asg12b"));
    ASSERT_FALSE(is_number("12b"));
    ASSERT_FALSE(is_number("a1"));
    ASSERT_EQ("cem", client_reply_get_number("GET /100"));
    ASSERT_EQ("GET cmd error", client_reply_get_number("100"));
    ASSERT_EQ("Invalid range", client_reply_get_number("GET /100000"));
    ASSERT_EQ("Invalid data", client_reply_get_number("GET /100a"));
}
#endif // _CLIENT_REPLY_TEST_H_
