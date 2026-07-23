#include "database.hpp"

#define PRODUCTION false

void Database::launch_db(){
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
        query = R"(CREATE TABLE USERS
                                    (ID INT
                                    NAME TEXT,
                                    ACTIVE BOOLEAN))";

        std::println("Query is: {}", query);
        
        if (auto ast = parser.parse_query(query))
        {
            if (std::holds_alternative<CreateTableAST>(*ast)) {
                std::println("Valid Create Statement");
            } else if (std::holds_alternative<InsertStatementAST>(*ast)) {
                std::println("Valid Insert Statement");
            } else if (std::holds_alternative<UpdateStatementAST>(*ast)) {
                std::println("Valid Update Statement");
            }
        } else {
            std::println("failed :(");
        }
    }
}
