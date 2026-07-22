#include <iostream>
#include <stdbool.h>
#include "parser.hpp"


// FIXME - for time being just put this function here
void launch_db(){
    Parser parser;
    // SymbolTable symbol_table;

    std::string query;

    // while (true) {
    //     std::getline(std::cin, query);

    //     std::println("Query is: {}", query);
        
    //     if (parser.parse_query(query))
    //     {
    //         std::println("successfully parsed statement\n");
    //     } else {
    //         std::println("failed :(");
    //     }
    // }

    query = R"(
            INSERT INTO Student (ROLL_NO, NAME, ADDRESS, PHONE, AGE)
            VALUES (1, 'Liam', 'New York', 'xxxxxxxxxx', 18),
                   (2, 'Sophia', 'Berlin', 'xxxxxxxxxx', 399),
                   (3, 'Akira', 'Tokyo', 'xxxxxxxxxx', 298191),
                   (4, 'Carlos', 'Tokyo', 'xxxxxxxxxx', 19191);
            )";

    std::println("Query is: {}", query);

    
    
    if (parser.parse_query(query))
    {
        std::println("successfully parsed query");
    } else {
        std::println("failed :(");
    }
}

int main(){

    launch_db();

    return 0;
}
