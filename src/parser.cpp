#include "lexer.hpp"
#include "parser.hpp"
#include "tokentype.hpp"

void Parser::match(const TokenType EXPECTED_TOK){
    
    if (curr_lookahead.token_type != EXPECTED_TOK){
        // TODO show proper error
        // std::println("failed on {}", curr_lookahead.text);
        exit(-1);
    } 
    // TODO quick line just to test tokenization
    // std::println("{} -> {}", curr_lookahead.text, static_cast<int>(curr_lookahead.token_type));
    curr_lookahead = lexer.get_next_token();
}

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

void Parser::parse_create_statement(){
    // TODO prior to beginning parsing, initialize the type of AST 
    // that this of parser will work with

    
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

bool Parser::parse_query(std::string_view query){

    lexer.set_user_query(query);

    // set initial current lookahead
    curr_lookahead = lexer.get_next_token();
    bool successful_parse = false;

    switch (curr_lookahead.token_type){

        case TokenType::CREATE_TOK:
            parse_create_statement();
            successful_parse = true;
            break;
        
        case TokenType::UPDATE_TOK:
            // parse_update_statement();
            successful_parse = true;
            break;

        case TokenType::SELECT_TOK:
            // parse_select_statement();
            successful_parse = true;
            break;

        case TokenType::DELETE_TOK:
            // parse_delete_statement();
            successful_parse = true;
            break;

        default:
            break;
    }
    
    return successful_parse;
}
