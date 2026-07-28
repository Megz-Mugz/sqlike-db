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

TEST(SelectStatementTest, ParsesMultipleColumnsWithIntegerWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT column1, column2, column3
            FROM table_name
            WHERE col_name = 1;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "table_name");
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"column1", ""},
            {"column2", ""},
            {"column3", ""},
        }));
    ExpectWhereClause(select_ast.where_clause, "col_name", "1", Type::INT);
}

TEST(SelectStatementTest, ParsesAllColumnsWithIntegerWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT *
            FROM table_name
            WHERE col_name = 1;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "table_name");
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
        }));
    ExpectWhereClause(select_ast.where_clause, "col_name", "1", Type::INT);
}

TEST(SelectStatementTest, ParsesSingleColumnWithoutWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query("SELECT Name FROM Student");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "Student");
    ASSERT_EQ(select_ast.selected_cols.size(), 1U);
    EXPECT_EQ(select_ast.selected_cols[0].first, "Name");
    EXPECT_TRUE(select_ast.selected_cols[0].second.empty());
    EXPECT_TRUE(select_ast.where_clause.empty());
}

TEST(SelectStatementTest, ParsesAllColumnsWithoutWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query("SELECT * FROM Student;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "Student");
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
        }));
    EXPECT_TRUE(select_ast.where_clause.empty());
}

TEST(SelectStatementTest, ParsesStringLiteralInWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "SELECT Name, City FROM Student WHERE City = 'Frankfurt'");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"Name", ""},
            {"City", ""},
        }));
    ExpectWhereClause(select_ast.where_clause, "City", "Frankfurt", Type::TEXT);
}

TEST(SelectStatementTest, ParsesBooleanLiteralInWhereClause) {
    Parser parser;

    auto parsed_query =
        parser.parse_query("SELECT Name FROM Student WHERE Active = true");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    ExpectWhereClause(select_ast.where_clause, "Active", "true", Type::BOOL);
}

TEST(SelectStatementTest, ParsesMixedCaseKeywordsAndBooleanLiteral) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "Select Name From Student Where Active = False;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "Student");
    ExpectWhereClause(select_ast.where_clause, "Active", "False", Type::BOOL);
}

TEST(SelectStatementTest, ParsesJoinWithQualifiedSelectedColumns) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT table1.column1, table2.column2
            FROM table1
            JOIN table2
            ON table1.common_column = table2.common_column;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "table1");
    EXPECT_EQ(select_ast.join_table, "table2");
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"column1", "table1"},
            {"column2", "table2"},
        }));
    EXPECT_EQ(
        select_ast.join_cols.first,
        (std::pair<ColumnName, TableName>{"common_column", "table1"}));
    EXPECT_EQ(
        select_ast.join_cols.second,
        (std::pair<ColumnName, TableName>{"common_column", "table2"}));
}

TEST(SelectStatementTest, ParsesJoinWhenSelectingAllColumns) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT *
            FROM table1
            JOIN table2
            ON table1.common_column = table2.common_column;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
        }));
    EXPECT_EQ(select_ast.join_table, "table2");
    EXPECT_EQ(select_ast.join_cols.first.first, "common_column");
    EXPECT_EQ(select_ast.join_cols.first.second, "table1");
    EXPECT_EQ(select_ast.join_cols.second.first, "common_column");
    EXPECT_EQ(select_ast.join_cols.second.second, "table2");
}

TEST(SelectStatementTest, ParsesJoinFollowedByWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT *
            FROM table1
            JOIN table2
            ON table1.common_column = table2.common_column
            WHERE active = true;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
        }));
    EXPECT_EQ(select_ast.join_table, "table2");
    ExpectWhereClause(select_ast.where_clause, "active", "true", Type::BOOL);
}

TEST(SelectStatementTest, ParsesMixedCaseJoinKeywords) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "Select * From table1 Join table2 On table1.id = table2.table1_id");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(select_ast.table_name, "table1");
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
        }));
    EXPECT_EQ(select_ast.join_table, "table2");
    EXPECT_EQ(
        select_ast.join_cols.first,
        (std::pair<ColumnName, TableName>{"id", "table1"}));
    EXPECT_EQ(
        select_ast.join_cols.second,
        (std::pair<ColumnName, TableName>{"table1_id", "table2"}));
}

TEST(SelectStatementTest, ParsesMixedQualifiedAndUnqualifiedColumns) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT Users.name, weight, Users.age
            FROM Users
            JOIN Admins
            ON Users.id = Admins.user_id
            WHERE id = 1;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"name", "Users"},
            {"weight", ""},
            {"age", "Users"},
        }));
    ExpectWhereClause(select_ast.where_clause, "id", "1", Type::INT);
}

TEST(SelectStatementTest, ParsesUnqualifiedColumnBeforeQualifiedColumn) {
    Parser parser;

    auto parsed_query =
        parser.parse_query("SELECT weight, Users.name FROM Users");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& selected_cols =
        std::get<SelectStatementAST>(*parsed_query).selected_cols;
    EXPECT_EQ(
        selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"weight", ""},
            {"name", "Users"},
        }));
}

TEST(SelectStatementTest, ParsesUnqualifiedColumnAfterQualifiedColumn) {
    Parser parser;

    auto parsed_query =
        parser.parse_query("SELECT Users.name, weight FROM Users");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& selected_cols =
        std::get<SelectStatementAST>(*parsed_query).selected_cols;
    EXPECT_EQ(
        selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"name", "Users"},
            {"weight", ""},
        }));
}

TEST(SelectStatementTest, ParsesWildcardFollowedByQualifiedColumnInJoin) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            SELECT *, Users.weight
            FROM Users
            JOIN Admins
            ON Users.id = Admins.user_id
            WHERE id = 1;
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& select_ast = std::get<SelectStatementAST>(*parsed_query);
    EXPECT_EQ(
        select_ast.selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
            {"weight", "Users"},
        }));
    ExpectWhereClause(select_ast.where_clause, "id", "1", Type::INT);
}

TEST(SelectStatementTest, ParsesWildcardFollowedByUnqualifiedColumn) {
    Parser parser;

    auto parsed_query = parser.parse_query("SELECT *, weight FROM Users");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& selected_cols =
        std::get<SelectStatementAST>(*parsed_query).selected_cols;
    EXPECT_EQ(
        selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {ALL_COLUMNS, ""},
            {"weight", ""},
        }));
}

TEST(SelectStatementTest, ParsesQualifiedColumnFollowedByWildcard) {
    Parser parser;

    auto parsed_query = parser.parse_query("SELECT Users.weight, * FROM Users");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<SelectStatementAST>(*parsed_query));

    const auto& selected_cols =
        std::get<SelectStatementAST>(*parsed_query).selected_cols;
    EXPECT_EQ(
        selected_cols,
        (std::vector<std::pair<ColumnName, TableName>>{
            {"weight", "Users"},
            {ALL_COLUMNS, ""},
        }));
}

TEST(SelectStatementTest, DiesWhenSelectedColumnsAreMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT FROM Student"), "");
}

TEST(SelectStatementTest, DiesWhenColumnIsMissingAfterComma) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name, FROM Student"), "");
}

TEST(SelectStatementTest, DiesWhenCommaIsMissingBetweenColumns) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name City FROM Student"), "");
}

TEST(SelectStatementTest, DiesWhenFromKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name Student"), "");
}

TEST(SelectStatementTest, DiesWhenTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name FROM"), "");
}

TEST(SelectStatementTest, DiesWhenWhereColumnIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name FROM Student WHERE = 1"), "");
}

TEST(SelectStatementTest, DiesWhenWhereEqualsSignIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name FROM Student WHERE id 1"), "");
}

TEST(SelectStatementTest, DiesWhenWhereValueIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name FROM Student WHERE id ="), "");
}

TEST(SelectStatementTest, DiesWhenWhereValueIsNotSupportedLiteral) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Name FROM Student WHERE id = OtherId"), "");
}

TEST(SelectStatementTest, ReturnsFalseWhenSecondWhereConditionIsPresent) {
    Parser parser;

    EXPECT_FALSE(
        parser
            .parse_query(
                "SELECT Name FROM Student WHERE Active = true AND Enrolled = true")
            .has_value());
}

TEST(SelectStatementTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(
        parser.parse_query("SELECT Name FROM Student EXTRA").has_value());
}

TEST(SelectStatementTest, DiesWhenJoinedTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN ON table1.id = table2.table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenOnKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 table1.id = table2.table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenLeftJoinTableIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON .id = table2.table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenLeftJoinDotIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON table1 id = table2.table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenLeftJoinColumnIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON table1. = table2.table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenJoinEqualsSignIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON table1.id table2.table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenRightJoinTableIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON table1.id = .table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenRightJoinDotIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON table1.id = table2 table1_id"),
        "");
}

TEST(SelectStatementTest, DiesWhenRightJoinColumnIsMissing) {
    Parser parser;

    EXPECT_DEATH(
        parser.parse_query(
            "SELECT * FROM table1 JOIN table2 ON table1.id = table2."),
        "");
}

TEST(SelectStatementTest, DiesWhenQualifiedColumnStartsWithDot) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT .name FROM Users"), "");
}

TEST(SelectStatementTest, DiesWhenQualifiedColumnEndsWithDot) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Users. FROM Users"), "");
}

TEST(SelectStatementTest, DiesWhenQualifiedColumnContainsTwoDots) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Users..name FROM Users"), "");
}

TEST(SelectStatementTest, DiesWhenCommaIsMissingAfterQualifiedColumn) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Users.name weight FROM Users"), "");
}

TEST(SelectStatementTest, DiesWhenCommaIsMissingAfterWildcard) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT * Users.weight FROM Users"), "");
}
