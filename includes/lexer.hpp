#pragma once

#include <optional>
#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>

#include "tokentype.hpp"

struct Token {
    TokenType token_type;
    std::string text;
};


class Lexer 
{

public:
    Token get_next_token();

    void set_user_query(std::string_view query);

private:

    std::string user_query;
    size_t cursor = 0;

    // helper functions to tokenize query

    void skip_whitespace();
    std::optional<Token> tokenize_operators();
    std::optional<Token> tokenize_keywords_and_identifiers();
    std::optional<Token> tokenize_strings();



};
