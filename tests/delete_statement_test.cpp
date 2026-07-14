#include <gtest/gtest.h>

#include "parser.hpp"

TEST(DeleteStatementTest, ParsesWithoutWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DELETE FROM Student;"));
}

TEST(DeleteStatementTest, ParsesWithoutSemicolon) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DELETE FROM Student"));
}

TEST(DeleteStatementTest, ParsesIntegerWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DELETE FROM Student WHERE id = 1;"));
}

TEST(DeleteStatementTest, ParsesStringWhereClause) {
    Parser parser;

    EXPECT_TRUE(
        parser.parse_query("DELETE FROM Student WHERE Name = 'Alfred Schmidt';"));
}

TEST(DeleteStatementTest, ParsesBooleanWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("DELETE FROM Student WHERE Active = true;"));
}

TEST(DeleteStatementTest, ParsesMixedCaseKeywordsAndBooleanLiteral) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "Delete From Student Where Active = False;"));
}

TEST(DeleteStatementTest, DiesWhenFromKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DELETE Student;"), "");
}

TEST(DeleteStatementTest, DiesWhenTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DELETE FROM;"), "");
}

TEST(DeleteStatementTest, DiesWhenWhereColumnIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DELETE FROM Student WHERE = 1;"), "");
}

TEST(DeleteStatementTest, DiesWhenWhereEqualsSignIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DELETE FROM Student WHERE id 1;"), "");
}

TEST(DeleteStatementTest, DiesWhenWhereValueIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DELETE FROM Student WHERE id =;"), "");
}

TEST(DeleteStatementTest, DiesWhenWhereValueIsNotSupportedLiteral) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query("DELETE FROM Student WHERE id = OtherId;"), "");
}

TEST(DeleteStatementTest, DiesWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("DELETE FROM Student EXTRA;"), "");
}
