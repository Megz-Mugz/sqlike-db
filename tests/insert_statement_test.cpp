#include <gtest/gtest.h>

#include "parser.hpp"

TEST(InsertStatementTest, ParsesInsertIntoStudentWithValues) {
    Parser parser;

    std::string_view query = R"(
            INSERT INTO Student
            VALUES (282921, 'Chicago');
            )";

    EXPECT_TRUE(parser.parse_query(query));
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

TEST(InsertStatementTest, ParsesInsertWithBooleanLiteral) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(
            INSERT INTO Student (ID, NAME, ACTIVE)
            VALUES (1, 'Liam', 'true');
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

    EXPECT_TRUE(parser.parse_query(query));
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

TEST(InsertStatementTest, DiesWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(
            INSERT INTO Student
            VALUES (1, 'Liam') EXTRA;
            )"), "");
}
