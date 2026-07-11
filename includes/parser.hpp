#pragma once
#include <iostream>
#include <string_view>
#include <stdbool.h>

#include "lexer.hpp"

class Parser 
{
public:

    bool parse_query(std::string_view query);

private:
    Lexer lexer;
    Token curr_lookahead;

    void match(const TokenType EXPECTED_TOK); // makes a call to lexer.get_next_token()

    void parse_column_constraint();
    void parse_column_datatype();

    void parse_create_statement();

    void parse_create_column_statement();

    // CreateStatement create_statement;

};
