#include "log.h"
#include <parser_test.h>
#include <client_reply_test.h>
#include <config_file_test.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    LOGLog::reporting_level(LOGLog::NONE);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
