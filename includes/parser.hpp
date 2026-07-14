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

    void match(const TokenType EXPECTED_TOK);

    // functions for parsing "create" statements
    void parse_column_constraint();
    void parse_column_datatype();
    void parse_create_statement();
    void parse_create_column_statement();

    // functions for parsing "insert" statements
    void parse_insert_statement();
    void parse_values_to_insert();
    void parse_columns_to_insert_into();
    void parse_actual_value();

    // functions for parsing "update" statements
    void parse_update_statement();
    void parse_columns_to_update();
    void parse_where_condition();

    // functions for parsing "select" statements
    void parse_select_statement();
    void parse_select_columns();
    void parse_join_condition();
    void parse_column_reference();

    // functions for parsing "delete" statements
    void parse_delete_statement();

    // functions for parsing "drop statements"
    void parse_drop_statement();
    
};
