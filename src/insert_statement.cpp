#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

void Parser::parse_columns_to_insert_into(){
    match(IDENTIFIER_TOK);

    if (curr_lookahead.token_type == COMMA_TOK){
        match(COMMA_TOK);
        parse_columns_to_insert_into();
    } else if (curr_lookahead.token_type == RIGHT_PAREN_TOK){
        match(RIGHT_PAREN_TOK);
    } else {
        exit(-1);
    }

    return;
}

void Parser::parse_actual_value(){
    switch (curr_lookahead.token_type) {
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
        parse_actual_value();
    } else if (curr_lookahead.token_type == RIGHT_PAREN_TOK){
        match(RIGHT_PAREN_TOK);
        return;
    } else {
        exit(-1);
    }
}

void Parser::parse_values_to_insert(){
    match(VALUES_TOK);

    if (curr_lookahead.token_type == LEFT_PAREN_TOK){
        match(LEFT_PAREN_TOK);
        parse_actual_value();
    } else {
        exit(-1);
    }

    while (curr_lookahead.token_type == COMMA_TOK){
        match(COMMA_TOK);

        if (curr_lookahead.token_type == LEFT_PAREN_TOK){
            match(LEFT_PAREN_TOK);
            parse_actual_value();
        } else {
            exit(-1);
        }
    }
}

void Parser::parse_insert_statement(){
    match(INSERT_INTO_TOK);
    match(IDENTIFIER_TOK);

    if (curr_lookahead.token_type == LEFT_PAREN_TOK){
        match(TokenType::LEFT_PAREN_TOK);
        parse_columns_to_insert_into();
    }

    if (curr_lookahead.token_type == VALUES_TOK){
        parse_values_to_insert();
    } else {
        exit(-1);
    }

}
