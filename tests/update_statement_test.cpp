#include <gtest/gtest.h>

#include "parser.hpp"

TEST(UpdateStatementTest, ParsesMultipleStringAssignmentsWithIntegerWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            UPDATE Student
            SET Name = 'Alfred Schmidt', City = 'Frankfurt'
            WHERE id = 1
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<UpdateStatementAST>(*parsed_query));

    const auto& update_ast = std::get<UpdateStatementAST>(*parsed_query);
    EXPECT_EQ(update_ast.table_name, "Student");
    EXPECT_EQ(update_ast.values_to_set.at("Name"), "Alfred Schmidt");
    EXPECT_EQ(update_ast.values_to_set.at("City"), "Frankfurt");
    EXPECT_EQ(update_ast.where_clause.at("id"), "1");
}

TEST(UpdateStatementTest, ParsesEachSupportedAssignmentLiteralType) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            UPDATE Student
            SET Age = 42, Active = true, Name = 'Alfred Schmidt';
            )"));
}

TEST(UpdateStatementTest, ParsesStringLiteralInWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "UPDATE Student SET City = 'Frankfurt' WHERE Name = 'Alfred Schmidt'"));
}

TEST(UpdateStatementTest, ParsesBooleanLiteralInWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "UPDATE Student SET Active = false WHERE Enrolled = true"));
}

TEST(UpdateStatementTest, ParsesWithoutWhereClause) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("UPDATE Student SET Active = false"));
}

TEST(UpdateStatementTest, ParsesMixedCaseKeywordsAndBooleanLiteral) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(
        "Update Student Set Active = True Where Enrolled = False;"));
}

TEST(UpdateStatementTest, DiesWhenTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE SET Name = 'Alfred Schmidt'"), "");
}

TEST(UpdateStatementTest, DiesWhenSetKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student Name = 'Alfred Schmidt'"), "");
}

TEST(UpdateStatementTest, DiesWhenAssignmentColumnIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET = 'Alfred Schmidt'"), "");
}

TEST(UpdateStatementTest, DiesWhenAssignmentEqualsSignIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Name 'Alfred Schmidt'"), "");
}

TEST(UpdateStatementTest, DiesWhenAssignmentValueIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Name ="), "");
}

TEST(UpdateStatementTest, DiesWhenAssignmentValueIsNotSupportedLiteral) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Name = OtherName"), "");
}

TEST(UpdateStatementTest, ReturnsFalseWhenCommaIsMissingBetweenAssignments) {
    Parser parser;

    EXPECT_FALSE(parser
                     .parse_query(
                         "UPDATE Student SET Name = 'Alfred' City = 'Frankfurt'")
                     .has_value());
}

TEST(UpdateStatementTest, DiesWhenAssignmentIsMissingAfterComma) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Name = 'Alfred',"), "");
}

TEST(UpdateStatementTest, DiesWhenWhereColumnIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Active = true WHERE = 1"), "");
}

TEST(UpdateStatementTest, DiesWhenWhereEqualsSignIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Active = true WHERE id 1"), "");
}

TEST(UpdateStatementTest, DiesWhenWhereValueIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Active = true WHERE id ="), "");
}

TEST(UpdateStatementTest, DiesWhenWhereValueIsNotSupportedLiteral) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("UPDATE Student SET Active = true WHERE id = OtherId"), "");
}

TEST(UpdateStatementTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(
        parser
            .parse_query("UPDATE Student SET Active = true WHERE id = 1 EXTRA")
            .has_value());
}
