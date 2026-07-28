#include <gtest/gtest.h>

#include "parser.hpp"

TEST(DropTableTest, ParsesDropTable) {
    Parser parser;

    auto parsed_query = parser.parse_query("DROP TABLE Student;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DropStatementAST>(*parsed_query));
    EXPECT_EQ(std::get<DropStatementAST>(*parsed_query).table_name, "Student");
}

TEST(DropTableTest, ParsesDropTableWithoutSemicolon) {
    Parser parser;

    auto parsed_query = parser.parse_query("DROP TABLE Student");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DropStatementAST>(*parsed_query));
    EXPECT_EQ(std::get<DropStatementAST>(*parsed_query).table_name, "Student");
}

TEST(DropTableTest, ParsesDropTableIfExists) {
    Parser parser;

    auto parsed_query = parser.parse_query("DROP TABLE IF EXISTS Student;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DropStatementAST>(*parsed_query));
    EXPECT_EQ(std::get<DropStatementAST>(*parsed_query).table_name, "Student");
}

TEST(DropTableTest, ParsesMixedCaseKeywords) {
    Parser parser;

    auto parsed_query = parser.parse_query("Drop Table If Exists Student;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DropStatementAST>(*parsed_query));
    EXPECT_EQ(std::get<DropStatementAST>(*parsed_query).table_name, "Student");
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

    EXPECT_FALSE(
        parser.parse_query("DROP TABLE Student EXTRA;").has_value());
}
