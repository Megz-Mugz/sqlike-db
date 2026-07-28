#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

void Parser::parse_join_condition(){
    if (curr_lookahead.token_type == JOIN_TOK){
        match(JOIN_TOK);
        select_statement_ast.join_table = curr_lookahead.text;
        match(IDENTIFIER_TOK);
        match(ON_TOK);
        
        // first table name & column
        std::string table_name = "";
        std::string col_name = "";
        table_name = curr_lookahead.text;
        match(IDENTIFIER_TOK);
        
        match(DOT_TOK);

        col_name = curr_lookahead.text;
        match(IDENTIFIER_TOK);

        select_statement_ast.join_cols.first = std::make_pair(col_name, table_name);

        match(EQUAL_TOK);

        // second table name & column
        table_name = curr_lookahead.text;
        match(IDENTIFIER_TOK);

        match(DOT_TOK);

        col_name = curr_lookahead.text;
        match(IDENTIFIER_TOK);

        select_statement_ast.join_cols.second = std::make_pair(col_name, table_name);

    }
}

void Parser::parse_column_reference() {
    std::string col_or_table_name = "";
    std::string col_name = "";
    
    col_or_table_name = curr_lookahead.text;
    match(IDENTIFIER_TOK);

    if (curr_lookahead.token_type == DOT_TOK) {
        match(DOT_TOK);
        col_name = curr_lookahead.text;
        match(IDENTIFIER_TOK);
    }

    if (col_name.empty()){
        select_statement_ast.selected_cols.emplace_back(col_or_table_name, "");
    } else {
        select_statement_ast.selected_cols.emplace_back(col_name, col_or_table_name);
    }
}

void Parser::parse_select_columns() {
    if (curr_lookahead.token_type == STAR_TOK) {
        select_statement_ast.selected_cols.emplace_back(
            ALL_COLUMNS, ""
        );
        match(STAR_TOK);
    } else {
        parse_column_reference();
    }

    while (curr_lookahead.token_type == COMMA_TOK) {
        match(COMMA_TOK);

        if (curr_lookahead.token_type == STAR_TOK) {
            select_statement_ast.selected_cols.emplace_back(
                ALL_COLUMNS, ""
            );
            match(STAR_TOK);
        } else {
            parse_column_reference();
        }
    }
}

AST Parser::parse_select_statement(){
    match(SELECT_TOK);
    
    parse_select_columns();

    match(FROM_TOK);
    select_statement_ast.table_name = curr_lookahead.text;
    match(IDENTIFIER_TOK);

    parse_join_condition();

    parse_where_condition(select_statement_ast.where_clause);

    return select_statement_ast;
}
