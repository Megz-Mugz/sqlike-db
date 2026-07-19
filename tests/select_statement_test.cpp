#include <gtest/gtest.h>

#include "parser.hpp"

TEST(SelectStatementTest, ParsesMultipleColumnsWithIntegerWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            SELECT column1, column2, column3
            FROM table_name
            WHERE col_name = 1;
            )"));
}

TEST(SelectStatementTest, ParsesAllColumnsWithIntegerWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            SELECT *
            FROM table_name
            WHERE col_name = 1;
            )"));
}

TEST(SelectStatementTest, ParsesSingleColumnWithoutWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("SELECT Name FROM Student"));
}

TEST(SelectStatementTest, ParsesAllColumnsWithoutWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("SELECT * FROM Student;"));
}

TEST(SelectStatementTest, ParsesStringLiteralInWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "SELECT Name, City FROM Student WHERE City = 'Frankfurt'"));
}

TEST(SelectStatementTest, ParsesBooleanLiteralInWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "SELECT Name FROM Student WHERE Active = true"));
}

TEST(SelectStatementTest, ParsesMixedCaseKeywordsAndBooleanLiteral) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "Select Name From Student Where Active = False;"));
}

TEST(SelectStatementTest, ParsesJoinWithQualifiedSelectedColumns) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            SELECT table1.column1, table2.column2
            FROM table1
            JOIN table2
            ON table1.common_column = table2.common_column;
            )"));
}

TEST(SelectStatementTest, ParsesJoinWhenSelectingAllColumns) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            SELECT *
            FROM table1
            JOIN table2
            ON table1.common_column = table2.common_column;
            )"));
}

TEST(SelectStatementTest, ParsesJoinFollowedByWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            SELECT *
            FROM table1
            JOIN table2
            ON table1.common_column = table2.common_column
            WHERE active = true;
            )"));
}

TEST(SelectStatementTest, ParsesMixedCaseJoinKeywords) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "Select * From table1 Join table2 On table1.id = table2.table1_id"));
}

TEST(SelectStatementTest, ParsesMixedQualifiedAndUnqualifiedColumns) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            SELECT Users.name, weight, Users.age
            FROM Users
            JOIN Admins
            ON Users.id = Admins.user_id
            WHERE id = 1;
            )"));
}

TEST(SelectStatementTest, ParsesUnqualifiedColumnBeforeQualifiedColumn) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "SELECT weight, Users.name FROM Users"));
}

TEST(SelectStatementTest, ParsesUnqualifiedColumnAfterQualifiedColumn) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "SELECT Users.name, weight FROM Users"));
}

TEST(SelectStatementTest, ParsesWildcardFollowedByQualifiedColumnInJoin) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            SELECT *, Users.weight
            FROM Users
            JOIN Admins
            ON Users.id = Admins.user_id
            WHERE id = 1;
            )"), "");
}

TEST(SelectStatementTest, ParsesWildcardFollowedByUnqualifiedColumn) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT *, weight FROM Users"), "");
}

TEST(SelectStatementTest, ParsesQualifiedColumnFollowedByWildcard) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("SELECT Users.weight, * FROM Users"), "");
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
        parser.parse_query(
            "SELECT Name FROM Student WHERE Active = true AND Enrolled = true"));
}

TEST(SelectStatementTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(parser.parse_query("SELECT Name FROM Student EXTRA"));
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
