#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"


void Parser::parse_delete_statement(){
    match(DELETE_TOK);
    match(FROM_TOK);
    delete_statement_ast.table_name = curr_lookahead.text;
    match(IDENTIFIER_TOK);
    parse_where_condition(delete_statement_ast);
}

void Parser::parse_drop_statement(){
    match(DROP_TOK);
    match(TABLE_TOK);
    if (curr_lookahead.token_type == TokenType::IF_TOK){
        match(IF_TOK);
        match(EXISTS_TOK);
        match(IDENTIFIER_TOK);
    } else if (curr_lookahead.token_type == TokenType::IDENTIFIER_TOK){
        match(IDENTIFIER_TOK);
    } else {
        exit(-1);
    }

    
}