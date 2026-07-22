#include "database.hpp"

#define PRODUCTION false

void Database::launch_db(){
    Parser parser;
    std::string query;

    if (PRODUCTION){
        while (true) {
            std::getline(std::cin, query);

            std::println("Query is: {}", query);
            
            if (parser.parse_query(query))
            {
                std::println("successfully parsed statement\n");
            } else {
                std::println("failed :(");
            }
        }
    } else {
        query = R"(
            UPDATE Student
            SET Age = 42, Active = true, Name = 'Alfred'
            WHERE Name = 'Alfred Schmidt';
            )";

        std::println("Query is: {}", query);
        
        if (parser.parse_query(query))
        {
            
        } else {
            std::println("failed :(");
        }
    }
}