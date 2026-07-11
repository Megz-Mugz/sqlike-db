#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

/*
    Parses column constraints that will be might optionally 
    be present after the column's datatype
*/
void Parser::parse_column_constraint(){
    switch(curr_lookahead.token_type){
        case TokenType::PRIMARY_KEY_CONSTRAINT_TOK:
            match(TokenType::PRIMARY_KEY_CONSTRAINT_TOK);
            break;

        case TokenType::NOT_NULL_CONSTRAINT_TOK:
            match(TokenType::NOT_NULL_CONSTRAINT_TOK);
            break;
        
        // TODO will be handled later in code, but these are the defaults
        // for the following datatypes int -> 0, text -> "", boolean -> false
        case TokenType::DEFAULT_CONSTRAINT_TOK:
            match(TokenType::DEFAULT_CONSTRAINT_TOK);
            break;

        case TokenType::UNIQUE_CONSTRAINT_TOK:
            match(TokenType::UNIQUE_CONSTRAINT_TOK);
            break;

        case TokenType::FOREIGN_KEY_CONSTRAINT_TOK:
            match(TokenType::FOREIGN_KEY_CONSTRAINT_TOK);
            break;

        case TokenType::COMMA_TOK:
        case TokenType::RIGHT_PAREN_TOK:
            break;
        
        default:
            // TODO better error handling
            // std::println("failed on {}", curr_lookahead.text);
            exit(-1);
    }
}

/*
    Parses for the datatype of the column, 
    which can either be an integer, boolean, or TEXT
*/
void Parser::parse_column_datatype(){
    switch(curr_lookahead.token_type){
        case TokenType::INT_TOK:
            match(TokenType::INT_TOK);
            break;

        case TokenType::BOOLEAN_TOK:
            match(TokenType::BOOLEAN_TOK);
            break;

        case TokenType::TEXT_TOK:
            match(TokenType::TEXT_TOK);
            break;
        
        default:
            // TODO better error handling
            exit(-1);
    }
}

/*
    Begins parsing for the column to be created. 
*/
void Parser::parse_create_column_statement(){
    
    match(TokenType::IDENTIFIER_TOK);
    parse_column_datatype();
    parse_column_constraint();

    if (curr_lookahead.token_type == TokenType::COMMA_TOK){
        match(TokenType::COMMA_TOK);
        parse_create_column_statement();
    } else if (curr_lookahead.token_type == TokenType::RIGHT_PAREN_TOK){
        match(TokenType::RIGHT_PAREN_TOK);
    } else {
        // TODO better error handling
        exit(-1);
    }
}

/*
    Begins parsing for the entire create statement. 
*/
void Parser::parse_create_statement(){

    match(TokenType::CREATE_TOK);
    match(TokenType::TABLE_TOK);
    match(TokenType::IDENTIFIER_TOK);

    if (curr_lookahead.token_type == TokenType::LEFT_PAREN_TOK){
        match(TokenType::LEFT_PAREN_TOK);
        parse_create_column_statement();
    }
    // TODO lookahead for end of line, if no end of line, throw error
    
    return;
}
