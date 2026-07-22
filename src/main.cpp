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
            UPDATE Student
            SET Age = 42, Active = true, Name = 'Alfred'
            WHERE Name = 'Alfred Schmidt';
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
