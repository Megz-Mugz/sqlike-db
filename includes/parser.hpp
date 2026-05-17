#pragma once
#include <iostream>
#include <string_view>
#include <stdbool.h>

#include "../includes/lexer.hpp"

class Parser 
{
public:

    // Parser() = delete;

    bool parse_query(std::string_view query);

private:
    Lexer lexer;
    Token curr_lookahead;

    void match(const TokenType EXPECTED_TOK); // makes a call to lexer.get_next_token()

    void parse_create_statement();

};