#ifndef _PARSER_TEST_H_
#define _PARSER_TEST_H_

#include <parser.h>
#include <gtest/gtest.h>

class ParserTest : public parser::ParserNumber, public testing::Test
{
   // Empty - bridge to protected members for unit-testing
};

TEST_F(ParserTest, CheckConversion)
{
    ASSERT_EQ("zero", GetPt());
    SetNumber(-1234);
    ASSERT_EQ("menos mil e duzentos e trinta e quatro", GetPt());
    SetNumber(100);
    ASSERT_EQ("cem", GetPt());
    ASSERT_EQ("dois ", pt_number_convert(2, ""));
    ASSERT_EQ("trinta e", pt_number_convert(30, "e"));
    ASSERT_EQ("cento ", pt_hundred_number_convert(1));
    ASSERT_EQ("duzentos ", pt_hundred_number_convert(2));
}
#endif // _PARSER_TEST_H_
