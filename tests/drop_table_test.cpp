#include <gtest/gtest.h>

#include "parser.hpp"

TEST(DropTableTest, ParsesDropTable) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DROP TABLE Student;"));
}

TEST(DropTableTest, ParsesDropTableWithoutSemicolon) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DROP TABLE Student"));
}

TEST(DropTableTest, ParsesDropTableIfExists) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DROP TABLE IF EXISTS Student;"));
}

TEST(DropTableTest, ParsesMixedCaseKeywords) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("Drop Table If Exists Student;"));
}

TEST(DropTableTest, DiesWhenTableKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DROP Student;"), "");
}

TEST(DropTableTest, DiesWhenTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DROP TABLE;"), "");
}

TEST(DropTableTest, DiesWhenTableNameAfterIfExistsIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DROP TABLE IF EXISTS;"), "");
}

TEST(DropTableTest, DiesWhenExistsKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DROP TABLE IF Student;"), "");
}

TEST(DropTableTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(parser.parse_query("DROP TABLE Student EXTRA;"));
}
