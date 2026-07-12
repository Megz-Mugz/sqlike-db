#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

void Parser::parse_join_condition(){
    if (curr_lookahead.token_type == JOIN_TOK){
        match(JOIN_TOK);
        match(IDENTIFIER_TOK);
        match(ON_TOK);
        
        // first table name & column
        match(IDENTIFIER_TOK);
        match(DOT_TOK);
        match(IDENTIFIER_TOK);

        match(EQUAL_TOK);

        // second table name & column
        match(IDENTIFIER_TOK);
        match(DOT_TOK);
        match(IDENTIFIER_TOK);

    }
}

void Parser::parse_column_reference() {
    match(IDENTIFIER_TOK);

    if (curr_lookahead.token_type == DOT_TOK) {
        match(DOT_TOK);
        match(IDENTIFIER_TOK);
    }
}

void Parser::parse_select_columns() {
    if (curr_lookahead.token_type == STAR_TOK) {
        match(STAR_TOK);
        return;
    }

    parse_column_reference();

    while (curr_lookahead.token_type == COMMA_TOK) {
        match(COMMA_TOK);
        parse_column_reference();
    }
}

void Parser::parse_select_statement(){
    match(SELECT_TOK);
    
    parse_select_columns();

    match(FROM_TOK);
    match(IDENTIFIER_TOK);

    parse_join_condition();

    parse_where_condition();
}