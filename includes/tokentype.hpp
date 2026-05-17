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
    MODULO_TOK,

    STRING_LIT_TOK, 
    COMMA_TOK,
    LEFT_PAREN_TOK, 
    RIGHT_PAREN_TOK, 

    INT_TOK, 
    BOOLEAN_TOK,
    TEXT_TOK,

    // constraint toks
    PRIMARY_KEY_CONSTRAINT_TOK,
    NOT_NULL_CONSTRAINT_TOK,
    DEFAULT_CONSTRAINT_TOK,
    UNIQUE_CONSTRAINT_TOK,
    FOREIGN_KEY_CONSTRAINT_TOK
};

constexpr char QUOTE_CHAR = '\'';

const std::unordered_map<std::string, TokenType> keywords = {
    {"CREATE", TokenType::CREATE_TOK}, 
    {"TABLE", TokenType::TABLE_TOK},
    {"INT", TokenType::INT_TOK}, 
    {"BOOLEAN", TokenType::BOOLEAN_TOK}, 
    {"TEXT", TokenType::TEXT_TOK},

    {"PRIMARY KEY", TokenType::PRIMARY_KEY_CONSTRAINT_TOK},
    {"NOT NULL", TokenType::NOT_NULL_CONSTRAINT_TOK},
    {"DEFAULT", TokenType::DEFAULT_CONSTRAINT_TOK},
    {"UNIQUE", TokenType::UNIQUE_CONSTRAINT_TOK},
    {"FOREIGN KEY", TokenType::FOREIGN_KEY_CONSTRAINT_TOK}

};

const std::unordered_map<char, TokenType> single_char_tokens = {
    {',', TokenType::COMMA_TOK},
    {'+', TokenType::PLUS_TOK},
    {'(', TokenType::LEFT_PAREN_TOK},
    {')', TokenType::RIGHT_PAREN_TOK}
};