#include <iostream>
#include "../includes/lexer.hpp"




void Lexer::set_user_query(std::string_view query){
    user_query = query;
}

void Lexer::skip_whitespace(){
    while (cursor < user_query.size() && std::isspace(user_query[cursor])){
        cursor++;
    }
}

std::optional<Token> Lexer::tokenize_operators(){
    char current_char = user_query[cursor];

    if (operators.contains(current_char)){
        return Token{
                    operators.at(user_query[cursor]), 
                    std::to_string(user_query[cursor])
                };
    }
    return std::nullopt;
}

std::optional<Token> Lexer::tokenize_keywords_and_identifiers(){
    
    if (!std::isalpha(user_query[cursor])) return std::nullopt;
        
    size_t start_of_word = cursor;
    
    while (cursor < user_query.size() && !std::isspace(user_query[cursor])) {
        cursor++;
    }

    auto word = user_query.substr(start_of_word, cursor - start_of_word);
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);

    if (keywords.contains(word)){
        return Token{
            keywords.at(word),
            word
        };
    }

    return Token{
        TokenType::IDENTIFIER_TOK,
        word
    };
    
}

std::optional<Token> Lexer::tokenize_strings(){
    return std::nullopt;
}


/*
    This function tokenizes the 
*/
Token Lexer::get_next_token(){

    // skip white space & new lines
    skip_whitespace();

    // find operators (+ - * / %)
    if (auto next_token = tokenize_operators()){
        return *next_token;
    }

    // keywords, identifiers
    if (auto next_token = tokenize_keywords_and_identifiers()){
        return *next_token;
    }

    // tokenizes strings
    if (auto next_token = tokenize_strings()){
        return *next_token;
    }

    return Token{TokenType::IDENTIFIER_TOK, ""};
};