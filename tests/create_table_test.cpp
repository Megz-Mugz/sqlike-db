#include <gtest/gtest.h>

#include "parser.hpp"

TEST(CreateTableTest, ParsesCreateTableUsers) {
    Parser parser;

    EXPECT_TRUE(parser.parse_query("CREATE TABLE USERS"));
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

// after ID, missing datatype, column must have a datatype even if following by a constraint
TEST(CreateTableTestWithConstraintsAndNoConstraints, ParsesCreateTableUsersWithColumnsAndConstraints2) {
    Parser parser;

    EXPECT_DEATH(parser.parse_query(R"(CREATE TABLE USERS
                                    (ID PRIMARY KEY,
                                    NAME TEXT,
                                    ACTIVE BOOLEAN,
                                    ORG INT FOREIGN KEY))"), "");
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