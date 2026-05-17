#include "../includes/lexer.hpp"

#include <string>


void Lexer::set_user_query(std::string_view query){
    user_query = query;
}

void Lexer::skip_whitespace(){
    while (cursor < user_query.size() &&
        (std::isspace(user_query[cursor]) || user_query[cursor] == '\n')){
        cursor++;
    }
}

std::optional<Token> Lexer::tokenize_single_chars(){
    char current_char = user_query[cursor];

    auto token_type = single_char_tokens.find(current_char);
    if (token_type != single_char_tokens.end()){
        cursor++;
        return Token{
                    token_type->second,
                    std::string(1, current_char)
                };
    }
    return std::nullopt;
}

std::optional<Token> Lexer::tokenize_keywords_and_identifiers(){
    
    if (!std::isalpha(user_query[cursor])) return std::nullopt;
        
    size_t start_of_word = cursor;
    
    while (cursor < user_query.size() && std::isalpha(user_query[cursor])) {
        cursor++;
    }

    
    auto word = user_query.substr(start_of_word, cursor - start_of_word);
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    
    // TODO you will have to handle situations where the token is 2 words like "PRIMARY KEY"


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

    if (user_query[cursor] != QUOTE_CHAR) return std::nullopt;

    // move past opening quote
    cursor++;
    size_t start_of_word = cursor;

    while (cursor < user_query.size() && std::isalpha(user_query[cursor])) {
        cursor++;
    }

    auto string_lit = user_query.substr(start_of_word, cursor - start_of_word);
    
    return Token {
        TokenType::STRING_LIT_TOK, 
        string_lit
    };


}


/*
    This function tokenizes the 
*/
Token Lexer::get_next_token(){

    // skip white space & new lines
    skip_whitespace();

    // find single chars like +, -, and (
    if (auto next_token = tokenize_single_chars()){
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
