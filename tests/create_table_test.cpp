#include <gtest/gtest.h>

#include "parser.hpp"

TEST(CreateTableTest, ParsesCreateTableUsers) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("CREATE TABLE USERS"));
}
