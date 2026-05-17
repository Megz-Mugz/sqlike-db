#pragma once

#include <unordered_map>
#include <string>

enum class TokenType {
    IDENTIFIER_TOK,
    CREATE_TOK,
    TABLE_TOK, 
    UPDATE_TOK, 
    SELECT_TOK, 
    DELETE_TOK, 

    PLUS_TOK, 
    MINUS_TOK, 
    MULTIPLY_TOK, 
    DIVIDE_TOK, 
    MODULO_TOK
};

const std::unordered_map<std::string, TokenType> keywords = {
    {"CREATE", TokenType::CREATE_TOK}, 
    {"TABLE", TokenType::TABLE_TOK}
};

const std::unordered_map<char, TokenType> operators = {
    {'+', TokenType::PLUS_TOK}
};