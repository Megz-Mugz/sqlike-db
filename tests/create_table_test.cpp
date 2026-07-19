#include <gtest/gtest.h>

#include "parser.hpp"

TEST(CreateTableTest, ParsesCreateTableUsers) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("CREATE TABLE USERS"));
}

TEST(CreateTableTest, ParsesCreateTableUsersWithSemicolon) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("CREATE TABLE USERS;"));
}

TEST(CreateTableTest, ParsesCreateTableWithColumnTypes) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID INT,
                                    NAME TEXT,
                                    ACTIVE BOOLEAN))"));
}


TEST(CreateTableTestWithConstraints, ParsesCreateTableUsersWithColumnsAndConstraints1) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID INT PRIMARY KEY,
                                    NAME TEXT NOT NULL,
                                    EMAIL TEXT UNIQUE,
                                    ACTIVE BOOLEAN DEFAULT,
                                    ORG INT FOREIGN KEY))"));
}

TEST(CreateTableTestWithConstraints, ParsesEachSupportedColumnConstraint) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID INT PRIMARY KEY,
                                    NAME TEXT NOT NULL,
                                    EMAIL TEXT UNIQUE,
                                    ACTIVE BOOLEAN DEFAULT,
                                    ORG_ID INT FOREIGN KEY))"));
}

// after ID, missing datatype, column must have a datatype even if following by a constraint
TEST(CreateTableTestWithConstraintsAndNoConstraints, ParsesCreateTableUsersWithColumnsAndConstraints2) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID PRIMARY KEY,
                                    NAME TEXT,
                                    ACTIVE BOOLEAN,
                                    ORG INT FOREIGN KEY))"), "");
}

TEST(CreateTableTest, DiesWhenTableNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("CREATE TABLE"), "");
}

TEST(CreateTableTest, DiesWhenCreateKeywordIsMissingTableKeyword) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query("CREATE USERS"), "");
}

TEST(CreateTableTest, DiesWhenColumnNameIsMissing) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(CREATE TABLE USERS
                                    (INT))"), "");
}

TEST(CreateTableTest, DiesWhenCommaIsMissingBetweenColumns) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID INT
                                    NAME TEXT))"), "");
}

TEST(CreateTableTest, ReturnsFalseWhenExtraTokensFollowStatement) {
    Parser parser;

    EXPECT_FALSE(parser.parse_query("CREATE TABLE USERS EXTRA"));
}

// after ID, missing datatype, column must have a datatype 
TEST(CreateTableTestColumnNoDatatype, WeirdTest) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID,
                                    NAME TEXT,
                                    ACTIVE BOOLEAN,
                                    ORG INT FOREIGN KEY))"), "");
}
