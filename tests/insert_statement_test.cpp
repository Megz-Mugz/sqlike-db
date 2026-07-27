#include <gtest/gtest.h>

#include "parser.hpp"

TEST(InsertStatementTest, ParsesInsertIntoStudentWithValues) {
    Parser parser;

    std::string_view query = R"(
            INSERT INTO Student
            VALUES (282921, 'Chicago');
            )";

    auto parsed_query = parser.parse_query(query);

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<InsertStatementAST>(*parsed_query));

    const auto& insert_ast = std::get<InsertStatementAST>(*parsed_query);
    EXPECT_EQ(insert_ast.table_name, "Student");
    ASSERT_EQ(insert_ast.rows_to_insert.size(), 1U);

    const auto& row = insert_ast.rows_to_insert[0];
    ASSERT_EQ(row.size(), 2U);
    EXPECT_EQ(row[0].col_name, "282921");
    EXPECT_EQ(row[0].m_type, Type::INT);
    EXPECT_EQ(row[0].m_constraint, Constraint::NONE);
    EXPECT_EQ(row[1].col_name, "Chicago");
    EXPECT_EQ(row[1].m_type, Type::TEXT);
    EXPECT_EQ(row[1].m_constraint, Constraint::NONE);
}

TEST(InsertStatementTest, ParsesInsertIntoStudentWithSeveralValues) {
    Parser parser;

    std::string_view query = R"(
            INSERT INTO Student
            VALUES (1, 'Liam', 'New York', 18);
            )";

    EXPECT_TRUE(parser.parse_query(query));
}

TEST(InsertStatementTest, ParsesInsertWithoutSemicolon) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (811487, 'Fairfax')
            )"));
}

TEST(InsertStatementTest, ParsesMixedCaseInsertKeywords) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("Insert Into Users Values (1, 2, 3)"));
}

TEST(InsertStatementTest, ParsesInsertWithBooleanLiteral) {
    Parser parser;

    auto parsed_query = parser.parse_query(R"(
            INSERT INTO Student (ID, NAME, ACTIVE)
            VALUES (1, 'Liam', true);
            )");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<InsertStatementAST>(*parsed_query));

    const auto& insert_ast = std::get<InsertStatementAST>(*parsed_query);
    ASSERT_EQ(insert_ast.columns.size(), 3U);
    EXPECT_EQ(insert_ast.columns[0].col_name, "ID");
    EXPECT_EQ(insert_ast.columns[1].col_name, "NAME");
    EXPECT_EQ(insert_ast.columns[2].col_name, "ACTIVE");

    ASSERT_EQ(insert_ast.rows_to_insert.size(), 1U);
    const auto& row = insert_ast.rows_to_insert[0];
    ASSERT_EQ(row.size(), 3U);
    EXPECT_EQ(row[0].m_type, Type::INT);
    EXPECT_EQ(row[1].m_type, Type::TEXT);
    EXPECT_EQ(row[2].col_name, "true");
    EXPECT_EQ(row[2].m_type, Type::BOOL);
}

TEST(InsertStatementTest, ParsesInsertWithMixedCaseBooleanLiteral) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            INSERT INTO Student (ID, NAME, ACTIVE)
            VALUES (2, 'Sophia', False);
            )"));
}

TEST(InsertStatementTest, ParsesInsertIntoStudentWithColumnList) {
    Parser parser;

    std::string_view query = R"(
            INSERT INTO Student (ROLL_NO, NAME, ADDRESS, AGE)
            VALUES (1, 'Liam', 'New York', 18);
            )";

    EXPECT_TRUE(parser.parse_query(query));
}

TEST(InsertStatementTest, ParsesInsertIntoStudentWithMultipleRows) {
    Parser parser;

    std::string_view query = R"(
            INSERT INTO Student (ROLL_NO, NAME, ADDRESS, PHONE, AGE)
            VALUES (1, 'Liam', 'New York', 'xxxxxxxxxx', 18),
                   (2, 'Sophia', 'Berlin', 'xxxxxxxxxx', 399),
                   (3, 'Akira', 'Tokyo', 'xxxxxxxxxx', 298191),
                   (4, 'Carlos', 'Tokyo', 'xxxxxxxxxx', 19191);
            )";

    auto parsed_query = parser.parse_query(query);

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<InsertStatementAST>(*parsed_query));

    const auto& insert_ast = std::get<InsertStatementAST>(*parsed_query);
    ASSERT_EQ(insert_ast.rows_to_insert.size(), 4U);

    for (const auto& row : insert_ast.rows_to_insert) {
        ASSERT_EQ(row.size(), 5U);
        EXPECT_EQ(row[0].m_type, Type::INT);
        EXPECT_EQ(row[1].m_type, Type::TEXT);
        EXPECT_EQ(row[2].m_type, Type::TEXT);
        EXPECT_EQ(row[3].m_type, Type::TEXT);
        EXPECT_EQ(row[4].m_type, Type::INT);
    }
}

TEST(InsertStatementTest, DiesWhenValuesListIsEmpty) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            VALUES ();
            )"), "");
}

TEST(InsertStatementTest, DiesWhenStringValueIsUnquoted) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (82902, Fairfax);
            )"), "");
}

TEST(InsertStatementTest, DiesWhenCommaIsMissingBetweenValues) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (2992292 'Fairfax');
            )"), "");
}

TEST(InsertStatementTest, DiesWhenValueIsMissingAfterComma) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (2992292,);
            )"), "");
}

TEST(InsertStatementTest, DiesWhenTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO
            VALUES (1, 'Liam');
            )"), "");
}

TEST(InsertStatementTest, DiesWhenValuesKeywordIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            (1, 'Liam');
            )"), "");
}

TEST(InsertStatementTest, DiesWhenColumnListIsEmpty) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student ()
            VALUES (1, 'Liam');
            )"), "");
}

TEST(InsertStatementTest, DiesWhenCommaIsMissingBetweenColumns) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student (ROLL_NO NAME)
            VALUES (1, 'Liam');
            )"), "");
}

TEST(InsertStatementTest, DiesWhenSecondValuesRowIsMissingLeftParen) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (1, 'Liam'), 2, 'Sophia';
            )"), "");
}

TEST(InsertStatementTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (1, 'Liam') EXTRA;
            )").has_value());
}
