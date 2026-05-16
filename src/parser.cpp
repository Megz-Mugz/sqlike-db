#include "../includes/lexer.hpp"
#include "../includes/parser.hpp"


bool Parser::parse_query(std::string_view query){

    // set initial current lookahead
    Token curr_lookahead = get_next_token();

    switch (curr_lookahead.token_type){

        case CREATE_TOK:
            parse_create_statement();
            break;
        
        case UPDATE_TOK:
            break;

        case SELECT_TOK:
            break;

        case DELETE_TOK:
            break;

        default:
            break;
    }
    
    return true;
}