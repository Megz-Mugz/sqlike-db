#include "../includes/lexer.hpp"
#include "../includes/parser.hpp"
#include "../includes/tokentype.hpp"

void Parser::match(const TokenType EXPECTED_TOK){
    
    if (curr_lookahead.token_type != EXPECTED_TOK){
        // TODO show proper error
        exit(-1);
    }

    curr_lookahead = lexer.get_next_token();
}


void Parser::parse_create_statement(){
    match(TokenType::CREATE_TOK);
    match(TokenType::TABLE_TOK);
    match(TokenType::IDENTIFIER_TOK);

    std::println("successfully parsed query!");

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
            break;

        case TokenType::SELECT_TOK:
            break;

        case TokenType::DELETE_TOK:
            break;

        default:
            break;
    }

    return true;
}