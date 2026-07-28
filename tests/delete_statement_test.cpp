#include <gtest/gtest.h>

#include "parser.hpp"

namespace {

void ExpectWhereClause(
    const WhereClause& where_clause,
    const ColumnName& column_name,
    const ColumnValue& value,
    Type type)
{
    ASSERT_EQ(where_clause.size(), 1U);

    const auto entry = where_clause.find(column_name);
    ASSERT_NE(entry, where_clause.end());
    EXPECT_EQ(entry->second.first, value);
    EXPECT_EQ(entry->second.second, type);
}

}

TEST(DeleteStatementTest, ParsesWithoutWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query("DELETE FROM Student;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DeleteStatementAST>(*parsed_query));

    const auto& delete_ast = std::get<DeleteStatementAST>(*parsed_query);
    EXPECT_EQ(delete_ast.table_name, "Student");
    EXPECT_TRUE(delete_ast.where_clause.empty());
}

TEST(DeleteStatementTest, ParsesWithoutSemicolon) {
    Parser parser;

    auto parsed_query = parser.parse_query("DELETE FROM Student");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DeleteStatementAST>(*parsed_query));

    const auto& delete_ast = std::get<DeleteStatementAST>(*parsed_query);
    EXPECT_EQ(delete_ast.table_name, "Student");
    EXPECT_TRUE(delete_ast.where_clause.empty());
}

TEST(DeleteStatementTest, ParsesIntegerWhereClause) {
    Parser parser;

    auto parsed_query =
        parser.parse_query("DELETE FROM Student WHERE id = 1;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DeleteStatementAST>(*parsed_query));

    const auto& delete_ast = std::get<DeleteStatementAST>(*parsed_query);
    EXPECT_EQ(delete_ast.table_name, "Student");
    ExpectWhereClause(delete_ast.where_clause, "id", "1", Type::INT);
}

TEST(DeleteStatementTest, ParsesStringWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "DELETE FROM Student WHERE Name = 'Alfred Schmidt';");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DeleteStatementAST>(*parsed_query));

    const auto& delete_ast = std::get<DeleteStatementAST>(*parsed_query);
    EXPECT_EQ(delete_ast.table_name, "Student");
    ExpectWhereClause(
        delete_ast.where_clause,
        "Name",
        "Alfred Schmidt",
        Type::TEXT);
}

TEST(DeleteStatementTest, ParsesBooleanWhereClause) {
    Parser parser;

    auto parsed_query =
        parser.parse_query("DELETE FROM Student WHERE Active = true;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DeleteStatementAST>(*parsed_query));

    const auto& delete_ast = std::get<DeleteStatementAST>(*parsed_query);
    EXPECT_EQ(delete_ast.table_name, "Student");
    ExpectWhereClause(delete_ast.where_clause, "Active", "true", Type::BOOL);
}

TEST(DeleteStatementTest, ParsesMixedCaseKeywordsAndBooleanLiteral) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "Delete From Student Where Active = False;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<DeleteStatementAST>(*parsed_query));

    const auto& delete_ast = std::get<DeleteStatementAST>(*parsed_query);
    EXPECT_EQ(delete_ast.table_name, "Student");
    ExpectWhereClause(delete_ast.where_clause, "Active", "False", Type::BOOL);
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

TEST(DeleteStatementTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(
        parser.parse_query("DELETE FROM Student EXTRA;").has_value());
}
