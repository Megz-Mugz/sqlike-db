#include "lexer.hpp"
#include "tokentype.hpp"
#include "parser.hpp"

/*
    Parses column constraints that will be might optionally 
    be present after the column's datatype
*/
void Parser::parse_column_constraint(ColumnData& col_data){
    switch(curr_lookahead.token_type){
        case TokenType::PRIMARY_KEY_CONSTRAINT_TOK:
            col_data.m_constraint = Constraint::PRIMARY_KEY;
            match(TokenType::PRIMARY_KEY_CONSTRAINT_TOK);
            break;

        case TokenType::NOT_NULL_CONSTRAINT_TOK:
            col_data.m_constraint = Constraint::NOT_NULL;
            match(TokenType::NOT_NULL_CONSTRAINT_TOK);
            break;

        case TokenType::DEFAULT_CONSTRAINT_TOK:
            col_data.m_constraint = Constraint::DEFAULT;
            match(TokenType::DEFAULT_CONSTRAINT_TOK);
            break;

        case TokenType::UNIQUE_CONSTRAINT_TOK:
            col_data.m_constraint = Constraint::UNIQUE;
            match(TokenType::UNIQUE_CONSTRAINT_TOK);
            break;

        case TokenType::FOREIGN_KEY_CONSTRAINT_TOK:
            col_data.m_constraint = Constraint::FOREIGN_KEY;
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
void Parser::parse_column_datatype(ColumnData& col_data){

    switch(curr_lookahead.token_type){
        case TokenType::INT_TOK:
            col_data.m_type = Type::INT;
            match(TokenType::INT_TOK);
            break;

        case TokenType::BOOLEAN_TOK:
            col_data.m_type = Type::BOOL;  
            match(TokenType::BOOLEAN_TOK);
            break;

        case TokenType::TEXT_TOK:
            col_data.m_type = Type::TEXT;
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
    
    ColumnData col_data;

    col_data.col_name = curr_lookahead.text;
    match(TokenType::IDENTIFIER_TOK);

    parse_column_datatype(col_data);
    parse_column_constraint(col_data);

    create_table_ast.columns.push_back(col_data);

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

    create_table_ast.table_name = curr_lookahead.text;
    match(TokenType::IDENTIFIER_TOK);

    if (curr_lookahead.token_type == TokenType::LEFT_PAREN_TOK){
        match(TokenType::LEFT_PAREN_TOK);
        parse_create_column_statement();
    }
    // TODO lookahead for end of line, if no end of line, throw error
    
    return;
}
