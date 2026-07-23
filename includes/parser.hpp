#pragma once
#include <iostream>
#include <string_view>
#include <stdbool.h>

#include "lexer.hpp"
#include "ast.hpp"

class Parser 
{
public:

    std::optional<AST> parse_query(std::string_view query);

private:

    Lexer lexer;
    Token curr_lookahead;

    // the various ASTs that can exist at runtime
    CreateTableAST      create_table_ast;
    InsertStatementAST  insert_statement_ast;
    UpdateStatementAST  update_statement_ast;
    // SelectStatementAST  select_statement_ast;
    // DropStatementAST    drop_statement_ast;
    // DeleteStatementAST  delete_statement_ast


    // SymbolTable symbol_table;

    void match(const TokenType EXPECTED_TOK);

    // functions for parsing "create" statements
    void parse_column_constraint(ColumnData& col_data);
    void parse_column_datatype(ColumnData& col_data);
    AST parse_create_statement();
    void parse_create_column_statement();

    // functions for parsing "insert" statements
    void parse_insert_statement();
    void parse_values_to_insert();
    void parse_columns_to_insert_into();
    void parse_actual_value(std::vector<ColumnValue>& current_row);

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
