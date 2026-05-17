#include <iostream>
#include "../includes/parser.hpp"

int main(){

    Parser parser;

    if (parser.parse_query("    'rohan' CREATE;")){
        std::println("successfully created table");
    } else {
        std::println("failed :(");
    }

    return 0;
}