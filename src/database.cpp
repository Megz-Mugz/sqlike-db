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
        query = R"(
            SELECT *, Users.weight
            FROM Users
            JOIN Admins
            ON Users.id = Admins.user_id
            WHERE id = 1;
            )";

        std::println("Query is: {}", query);
        
        if (auto ast = parser.parse_query(query))
        {
            typechecker.typecheck_ast(*ast, schema);
        } else {
            std::println("failed :(");
        }
    }
}
