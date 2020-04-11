#ifndef _CONFIG_FILE_TEST_H_
#define _CONFIG_FILE_TEST_H_

#include <config_file.h>
#include <gtest/gtest.h>

TEST(ConfigTest, CheckConfig)
{
    config::ConfigFile config = config::ConfigFile("../../n2wconverter.conf");
    ASSERT_TRUE(config.Get("port").size());
    ASSERT_TRUE(config.Get("log_file").size());
    ASSERT_EQ("30000", config.Get("port"));
    ASSERT_EQ("/opt/certi/var/log/n2wconverter.log", config.Get("log_file"));
}
#endif // _CONFIG_FILE_TEST_H_
