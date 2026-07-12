#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

// FIXME for the time being, only support one clause, not AND/OR yet
void Parser::parse_where_condition(){
    if (curr_lookahead.token_type == WHERE_TOK){
        match(WHERE_TOK);
        match(IDENTIFIER_TOK);
        match(EQUAL_TOK);
        switch(curr_lookahead.token_type){
            case INT_LIT_TOK:
                match(INT_LIT_TOK);
                break;

            case STRING_LIT_TOK:
                match(STRING_LIT_TOK);
                break;

            case BOOL_LIT_TOK:
                match(BOOL_LIT_TOK);
                break;

            default:
                exit(-1);
        }
    }

    return;
}
void Parser::parse_columns_to_update(){
    match(IDENTIFIER_TOK);
    match(EQUAL_TOK);

    switch(curr_lookahead.token_type){
        case INT_LIT_TOK:
            match(INT_LIT_TOK);
            break;

        case STRING_LIT_TOK:
            match(STRING_LIT_TOK);
            break;

        case BOOL_LIT_TOK:
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



void Parser::parse_update_statement(){
    match(UPDATE_TOK);
    match(IDENTIFIER_TOK);
    match(SET_TOK);

    parse_columns_to_update();

    parse_where_condition();

}