#include "lexer.hpp"
#include "parser.hpp"
#include "tokentype.hpp"


// FIXME for the time being, only support one clause, not AND/OR yet
void Parser::parse_where_condition(AST& current_ast){
    if (curr_lookahead.token_type == WHERE_TOK){
        match(WHERE_TOK);
        auto column_name = curr_lookahead.text;
        match(IDENTIFIER_TOK);
        match(EQUAL_TOK);
        switch(curr_lookahead.token_type){
            case INT_LIT_TOK:
                current_ast.where_clause[column_name] = {curr_lookahead.text, Type::INT};
                match(INT_LIT_TOK);
                break;

            case STRING_LIT_TOK:
                current_ast.where_clause[column_name] = {curr_lookahead.text, Type::TEXT};
                match(STRING_LIT_TOK);
                break;

            case BOOL_LIT_TOK:
                current_ast.where_clause[column_name] = {curr_lookahead.text, Type::BOOL};
                match(BOOL_LIT_TOK);
                break;

            default:
                exit(-1);
        }
    }

    return;
}



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
std::optional<AST> Parser::parse_query(std::string_view query)
{
    lexer.set_user_query(query);
    curr_lookahead = lexer.get_next_token();

    AST generated_ast;

    switch (curr_lookahead.token_type) {
        case TokenType::CREATE_TOK:
            generated_ast = parse_create_statement();
            break;

        case TokenType::INSERT_INTO_TOK:
            generated_ast = parse_insert_statement();
            break;

        case TokenType::UPDATE_TOK:
            generated_ast = parse_update_statement();
            break;

        case TokenType::SELECT_TOK:
            parse_select_statement();
            break;

        case TokenType::DELETE_TOK:
            parse_delete_statement();
            break;

        case TokenType::DROP_TOK:
            parse_drop_statement();
            break;

        default:
            return std::nullopt;
    }

    if (curr_lookahead.token_type == TokenType::SEMICOLON_TOK) {
        match(TokenType::SEMICOLON_TOK);
    }

    if (curr_lookahead.token_type != TokenType::END_OF_QUERY_TOK) {
        return std::nullopt;
    }

    return generated_ast;
}
