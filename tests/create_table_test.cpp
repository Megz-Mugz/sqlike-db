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

    auto parsed_query = parser.parse_query(R"(CREATE TABLE USERS
                                              (ID INT,
                                              NAME TEXT,
                                              ACTIVE BOOLEAN))");

    ASSERT_TRUE(parsed_query.has_value());
    ASSERT_TRUE(std::holds_alternative<CreateTableAST>(*parsed_query));

    const auto& create_ast = std::get<CreateTableAST>(*parsed_query);
    EXPECT_EQ(create_ast.table_name, "USERS");
    ASSERT_EQ(create_ast.columns.size(), 3U);
    EXPECT_EQ(create_ast.columns[0].col_name, "ID");
    EXPECT_EQ(create_ast.columns[0].m_type, Type::INT);
    EXPECT_EQ(create_ast.columns[1].col_name, "NAME");
    EXPECT_EQ(create_ast.columns[1].m_type, Type::TEXT);
    EXPECT_EQ(create_ast.columns[2].col_name, "ACTIVE");
    EXPECT_EQ(create_ast.columns[2].m_type, Type::BOOL);
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

    EXPECT_FALSE(parser.parse_query("CREATE TABLE USERS EXTRA").has_value());
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
