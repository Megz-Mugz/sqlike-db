#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"


void Parser::parse_columns_to_update(){
    auto column_name = curr_lookahead.text;
    match(IDENTIFIER_TOK);
    match(EQUAL_TOK);

    switch(curr_lookahead.token_type){
        case INT_LIT_TOK:
            update_statement_ast.values_to_set[column_name] = {curr_lookahead.text, Type::INT};
            match(INT_LIT_TOK);
            break;

        case STRING_LIT_TOK:
            update_statement_ast.values_to_set[column_name] = {curr_lookahead.text, Type::TEXT};
            match(STRING_LIT_TOK);
            break;

        case BOOL_LIT_TOK:
            update_statement_ast.values_to_set[column_name] = {curr_lookahead.text, Type::BOOL};
            match(BOOL_LIT_TOK);
            break;

        default:
            exit(-1);
    }

    if (curr_lookahead.token_type == COMMA_TOK){
        match(COMMA_TOK);
        parse_columns_to_update();
    }

}

AST Parser::parse_update_statement(){
    match(UPDATE_TOK);
    update_statement_ast.table_name = curr_lookahead.text;
    match(IDENTIFIER_TOK);
    match(SET_TOK);

    parse_columns_to_update();

    parse_where_condition(update_statement_ast.where_clause);

    return update_statement_ast;
}
