#pragma once

#include <unordered_map>
#include <string>
#include <unordered_set>

enum class Constraint{
    NONE,
    PRIMARY_KEY, 
    FOREIGN_KEY, 
    NOT_NULL,
    UNIQUE, 
    DEFAULT
};

enum class Type{
    UNKNOWN,
    INT, 
    TEXT, 
    BOOL
};

enum TokenType {
    IDENTIFIER_TOK,
    CREATE_TOK,
    TABLE_TOK, 
    UPDATE_TOK,
    SELECT_TOK, 
    DELETE_TOK, 
    INSERT_INTO_TOK,
    VALUES_TOK,
    PLUS_TOK, 
    MINUS_TOK, 
    MULTIPLY_TOK, 
    DIVIDE_TOK, 
    MODULO_TOK,
    INT_LIT_TOK, 
    BOOL_LIT_TOK, 
    STRING_LIT_TOK, 
    COMMA_TOK,
    SEMICOLON_TOK,
    LEFT_PAREN_TOK, 
    RIGHT_PAREN_TOK, 
    INT_TOK, 
    BOOLEAN_TOK,
    TEXT_TOK,
    EQUAL_TOK,
    WHERE_TOK,
    SET_TOK,
    STAR_TOK,
    FROM_TOK,
    JOIN_TOK,
    ON_TOK,
    DOT_TOK,
    IF_TOK,
    EXISTS_TOK,
    DROP_TOK,
    PRIMARY_KEY_CONSTRAINT_TOK,
    NOT_NULL_CONSTRAINT_TOK,
    DEFAULT_CONSTRAINT_TOK,
    UNIQUE_CONSTRAINT_TOK,
    FOREIGN_KEY_CONSTRAINT_TOK,
    END_OF_QUERY_TOK
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
    {"FOREIGN KEY", TokenType::FOREIGN_KEY_CONSTRAINT_TOK},
    {"INSERT INTO", TokenType::INSERT_INTO_TOK},
    {"DEFAULT", TokenType::DEFAULT_CONSTRAINT_TOK},
    {"UNIQUE", TokenType::UNIQUE_CONSTRAINT_TOK},
    {"VALUES", TokenType::VALUES_TOK}, 
    {"UPDATE", TokenType::UPDATE_TOK}, 
    {"WHERE", TokenType::WHERE_TOK}, 
    {"SET", TokenType::SET_TOK},
    {"FROM", TokenType::FROM_TOK},
    {"SELECT", TokenType::SELECT_TOK},
    {"JOIN", TokenType::JOIN_TOK},
    {"ON", TokenType::ON_TOK},
    {"DELETE", TokenType::DELETE_TOK},
    {"DROP", TokenType::DROP_TOK},
    {"IF", TokenType::IF_TOK},
    {"EXISTS", TokenType::EXISTS_TOK}
    
};

const std::unordered_set<std::string> boolean_key_words = {
    "TRUE", 
    "FALSE"
};

const std::unordered_set<std::string> first_word_of_compound_keyword = {
    "PRIMARY", 
    "NOT", 
    "FOREIGN", 
    "INSERT"
};

const std::unordered_map<char, TokenType> single_char_tokens = {
    {',', TokenType::COMMA_TOK},
    {';', TokenType::SEMICOLON_TOK},
    {'+', TokenType::PLUS_TOK},
    {'*', TokenType::STAR_TOK},
    {'(', TokenType::LEFT_PAREN_TOK},
    {')', TokenType::RIGHT_PAREN_TOK}, 
    {'=', TokenType::EQUAL_TOK},
    {'.', TokenType::DOT_TOK}
};
