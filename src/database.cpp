#include "database.hpp"

#define PRODUCTION false

void Database::launch_db(){
    schema.clear();

    Parser parser;
    std::string query;

    if (PRODUCTION){
        while (true) {
            std::getline(std::cin, query);

            std::println("Query is: {}", query);
            
            if (auto parsed_query = parser.parse_query(query))
            {
                std::visit(
                    [](const auto& statement) {
                        std::println(
                            "successfully parsed statement: table = {}\n",
                            statement.table_name);
                    },
                    *parsed_query);
            } else {
                std::println("failed :(");
            }
        }
    } else {
    const std::vector<std::string> queries{
        R"(CREATE TABLE USERS
            (ID INT PRIMARY KEY,
             NAME TEXT NOT NULL,
             EMAIL TEXT UNIQUE,
             ACTIVE BOOLEAN DEFAULT,
             ORG_ID INT FOREIGN KEY))",

        R"(CREATE TABLE ORGANIZATIONS
            (ID INT PRIMARY KEY,
             NAME TEXT NOT NULL))",

        R"(CREATE TABLE POSTS
            (ID INT PRIMARY KEY,
             TITLE TEXT NOT NULL,
             AUTHOR_ID INT FOREIGN KEY))"
    };

    for (const auto& query : queries) {
        std::println("Query is: {}", query);

        if (auto ast = parser.parse_query(query)) {
            typechecker.typecheck_ast(*ast, schema);
        } else {
            std::println("failed :(");
        }
    }
}
}
