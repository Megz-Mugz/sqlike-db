#include <iostream>
#include "../includes/parser.hpp"

int main(){

    Parser parser;

    std::string query = R"(
        create table Persons (
            PersonID int PRIMARY KEY ,
            LastName text UNIQUE,
            FirstName text,
            Address text NOT NULL,
            City text
        );)";

    if (parser.parse_query(query))
    {
        std::println("successfully created table");
    } else {
        std::println("failed :(");
    }

    return 0;
}