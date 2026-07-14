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
            std::println("successfully parsed statement\n");
        } else {
            std::println("failed :(");
        }
    }

    // query = R"(
    //         Select *, Users.weight
    //         From Users
    //         Join Admins
    //         On Users.id = Admins.user_id
    //         WHERE id = 1)";

    // std::println("Query is: {}", query);

    
    
    // if (parser.parse_query(query))
    // {
    //     std::println("successfully parsed query");
    // } else {
    //     std::println("failed :(");
    // }


    return 0;
}
