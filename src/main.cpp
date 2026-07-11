#include <iostream>
#include <stdbool.h>
#include "parser.hpp"

int main(){

    Parser parser;

    std::string query;

    while (true) {
        std::getline(std::cin, query);

        std::println("Query is: {}", query);
        
        if (parser.parse_query(query))
        {
            std::println("successfully parsed statement");
        } else {
            std::println("failed :(");
        }
    }

    // query = R"(
    //         INSERT INTO Student
    //         VALUES (811487, 'Fairfax');
    //         )";

    // std::println("Query is: {}", query);

    
    
    // if (parser.parse_query(query))
    // {
    //     std::println("successfully parsed query");
    // } else {
    //     std::println("failed :(");
    // }


    return 0;
}
