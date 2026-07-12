#include "lexer.hpp"
#include "parser.hpp"
#include "tokentype.hpp"

/*
    Determines if the current token is what we expect
    and makes an request for the next token upon valid
    current token. 
*/
void Parser::match(const TokenType EXPECTED_TOK){
    
    if (curr_lookahead.token_type != EXPECTED_TOK){
        // TODO show proper error
        // std::println("failed on {}", curr_lookahead.text);
        exit(-1);
    } 

    // TODO: quick line just to test tokenization
    // std::println("{} -> {}", curr_lookahead.text, static_cast<int>(curr_lookahead.token_type));
    curr_lookahead = lexer.get_next_token();
}

/*
    This function decides which of the 5 types of statements we are parsing
        1. Create
        2. Insert
        3. Update
        4. Select
        5. Delete
        6. Drop
    If the statement doesn't start with any of these keywords, invalidate statement
*/
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
        
        case TokenType::INSERT_INTO_TOK:
            parse_insert_statement();
            successful_parse = true;
            break;
        
        case TokenType::UPDATE_TOK:
            parse_update_statement();
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

    if (successful_parse){
        if (curr_lookahead.token_type == TokenType::SEMICOLON_TOK){
            match(TokenType::SEMICOLON_TOK);
        }

        if (curr_lookahead.token_type != TokenType::END_OF_QUERY_TOK){
            exit(-1);
        }
    }
    
    return successful_parse;
}
