#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

void Parser::parse_actual_value(std::vector<ColumnValue>& current_row){
    switch (curr_lookahead.token_type) {
        case INT_LIT_TOK:
            current_row.push_back(curr_lookahead.text);
            match(INT_LIT_TOK);
            break;

        case STRING_LIT_TOK:
            current_row.push_back(curr_lookahead.text);
            match(STRING_LIT_TOK);
            break;

        case BOOL_LIT_TOK:
            current_row.push_back(curr_lookahead.text);
            match(BOOL_LIT_TOK);
            break;

        default:
            exit(-1);
    }
    
    if (curr_lookahead.token_type == COMMA_TOK){
        match(COMMA_TOK);
        parse_actual_value(current_row);
    } else if (curr_lookahead.token_type == RIGHT_PAREN_TOK){
        match(RIGHT_PAREN_TOK);
        return;
    } else {
        exit(-1);
    }
}

void Parser::parse_values_to_insert(){
    match(VALUES_TOK);

    std::vector<ColumnValue> current_row;

    if (curr_lookahead.token_type == LEFT_PAREN_TOK){
        match(LEFT_PAREN_TOK);
        parse_actual_value(current_row);
    } else {
        exit(-1);
    }

    insert_statement_ast.rows_to_insert.push_back(current_row);

    while (curr_lookahead.token_type == COMMA_TOK){
        std::vector<ColumnValue> current_row;

        match(COMMA_TOK);

        if (curr_lookahead.token_type == LEFT_PAREN_TOK){
            match(LEFT_PAREN_TOK);
            parse_actual_value(current_row);
            insert_statement_ast.rows_to_insert.push_back(current_row);
        } else {
            exit(-1);
        }
    }
}


void Parser::parse_columns_to_insert_into(){
    insert_statement_ast.columns.push_back(curr_lookahead.text);
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

AST Parser::parse_insert_statement(){
    match(INSERT_INTO_TOK);
    insert_statement_ast.table_name = curr_lookahead.text;
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

    return insert_statement_ast;

}
