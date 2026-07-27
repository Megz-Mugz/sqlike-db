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
    ASSERT_EQ(update_ast.values_to_set.size(), 2U);
    EXPECT_EQ(update_ast.values_to_set.at("Name").first, "Alfred Schmidt");
    EXPECT_EQ(update_ast.values_to_set.at("Name").second, Type::TEXT);
    EXPECT_EQ(update_ast.values_to_set.at("City").first, "Frankfurt");
    EXPECT_EQ(update_ast.values_to_set.at("City").second, Type::TEXT);

    ASSERT_EQ(update_ast.where_clause.size(), 1U);
    EXPECT_EQ(update_ast.where_clause.at("id").first, "1");
    EXPECT_EQ(update_ast.where_clause.at("id").second, Type::INT);
}

TEST(UpdateStatementTest, ParsesEachSupportedAssignmentLiteralType) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            UPDATE Student
            SET Age = 42, Active = true, Name = 'Alfred Schmidt';
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<UpdateStatementAST>(*parsed_query));

    const auto& values = std::get<UpdateStatementAST>(*parsed_query).values_to_set;
    ASSERT_EQ(values.size(), 3U);
    EXPECT_EQ(values.at("Age").first, "42");
    EXPECT_EQ(values.at("Age").second, Type::INT);
    EXPECT_EQ(values.at("Active").first, "true");
    EXPECT_EQ(values.at("Active").second, Type::BOOL);
    EXPECT_EQ(values.at("Name").first, "Alfred Schmidt");
    EXPECT_EQ(values.at("Name").second, Type::TEXT);
}

TEST(UpdateStatementTest, ParsesStringLiteralInWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "UPDATE Student SET City = 'Frankfurt' WHERE Name = 'Alfred Schmidt'");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<UpdateStatementAST>(*parsed_query));

    const auto& update_ast = std::get<UpdateStatementAST>(*parsed_query);
    EXPECT_EQ(update_ast.values_to_set.at("City").first, "Frankfurt");
    EXPECT_EQ(update_ast.values_to_set.at("City").second, Type::TEXT);
    EXPECT_EQ(update_ast.where_clause.at("Name").first, "Alfred Schmidt");
    EXPECT_EQ(update_ast.where_clause.at("Name").second, Type::TEXT);
}

TEST(UpdateStatementTest, ParsesBooleanLiteralInWhereClause) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "UPDATE Student SET Active = false WHERE Enrolled = true");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<UpdateStatementAST>(*parsed_query));

    const auto& update_ast = std::get<UpdateStatementAST>(*parsed_query);
    EXPECT_EQ(update_ast.values_to_set.at("Active").first, "false");
    EXPECT_EQ(update_ast.values_to_set.at("Active").second, Type::BOOL);
    EXPECT_EQ(update_ast.where_clause.at("Enrolled").first, "true");
    EXPECT_EQ(update_ast.where_clause.at("Enrolled").second, Type::BOOL);
}

TEST(UpdateStatementTest, ParsesWithoutWhereClause) {
    Parser parser;

    auto parsed_query =
        parser.parse_query("UPDATE Student SET Active = false");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<UpdateStatementAST>(*parsed_query));

    const auto& update_ast = std::get<UpdateStatementAST>(*parsed_query);
    EXPECT_EQ(update_ast.values_to_set.at("Active").first, "false");
    EXPECT_EQ(update_ast.values_to_set.at("Active").second, Type::BOOL);
    EXPECT_TRUE(update_ast.where_clause.empty());
}

TEST(UpdateStatementTest, ParsesMixedCaseKeywordsAndBooleanLiteral) {
    Parser parser;

    auto parsed_query = parser.parse_query(
        "Update Student Set Active = True Where Enrolled = False;");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<UpdateStatementAST>(*parsed_query));

    const auto& update_ast = std::get<UpdateStatementAST>(*parsed_query);
    EXPECT_EQ(update_ast.values_to_set.at("Active").first, "True");
    EXPECT_EQ(update_ast.values_to_set.at("Active").second, Type::BOOL);
    EXPECT_EQ(update_ast.where_clause.at("Enrolled").first, "False");
    EXPECT_EQ(update_ast.where_clause.at("Enrolled").second, Type::BOOL);
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
