#include <iostream>
#include <stdbool.h>
#include "parser.hpp"

int main(){

    Parser parser;

    std::string query;

    // while (true) {
    //     std::getline(std::cin, query);

    //     std::println("Query is: {}", query);
        
    //     if (parser.parse_query(query))
    //     {
    //         std::println("successfully parsed statement");
    //     } else {
    //         std::println("failed :(");
    //     }
    // }

    query = R"(
            UPDATE Student
            SET Name = 'Alfred Schmidt', City= 29292, Admin  = False
            WHERE id = 1)";

    std::println("Query is: {}", query);

    
    
    if (parser.parse_query(query))
    {
        std::println("successfully parsed query");
    } else {
        std::println("failed :(");
    }


    return 0;
}
