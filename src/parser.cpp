#include "../includes/lexer.hpp"
#include "../includes/parser.hpp"
#include "../includes/tokentype.hpp"

void Parser::match(const TokenType EXPECTED_TOK){
    
    if (curr_lookahead.token_type != EXPECTED_TOK){
        // TODO show proper error
        std::println("failed on {}", curr_lookahead.text);
        exit(-1);
    } 
    std::println("{} -> {}", curr_lookahead.text, static_cast<int>(curr_lookahead.token_type));
    curr_lookahead = lexer.get_next_token();
}

void Parser::parse_column_constraint(){
    switch(curr_lookahead.token_type){
        case TokenType::PRIMARY_KEY_CONSTRAINT_TOK:
            match(TokenType::PRIMARY_KEY_CONSTRAINT_TOK);
            break;

        case TokenType::NOT_NULL_CONSTRAINT_TOK:
            match(TokenType::NOT_NULL_CONSTRAINT_TOK);
            break;

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
            std::println("failed on {}", curr_lookahead.text);
            exit(-1);
    }
}

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
        return;
    } else {
        // TODO better error handling
        exit(-1);
    }
}

void Parser::parse_create_statement(){
    match(TokenType::CREATE_TOK);
    match(TokenType::TABLE_TOK);
    match(TokenType::IDENTIFIER_TOK);

    if (curr_lookahead.token_type == TokenType::LEFT_PAREN_TOK){
        match(TokenType::LEFT_PAREN_TOK);
        parse_create_column_statement();
    }

    match(TokenType::RIGHT_PAREN_TOK);
    return;
}

bool Parser::parse_query(std::string_view query){

    lexer.set_user_query(query);

    // set initial current lookahead
    curr_lookahead = lexer.get_next_token();

    switch (curr_lookahead.token_type){

        case TokenType::CREATE_TOK:
            parse_create_statement();
            break;
        
        case TokenType::UPDATE_TOK:
            // parse_update_statement();
            break;

        case TokenType::SELECT_TOK:
            // parse_select_statement();
            break;

        case TokenType::DELETE_TOK:
            // parse_delete_statement();
            break;

        default:
            break;
    }
    
    std::println("successfully parsed query!");
    return true;
}
