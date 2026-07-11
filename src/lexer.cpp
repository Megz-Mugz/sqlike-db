#include "lexer.hpp"

#include <string>


void Lexer::set_user_query(std::string_view query){
    user_query = query;
    cursor = 0;
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
    
    if (!std::isalpha(static_cast<unsigned char>(user_query[cursor])) &&
        user_query[cursor] != '_') {
        return std::nullopt;
    }
        
    size_t start_of_word = cursor;
    
    while (cursor < user_query.size() &&
        (std::isalnum(static_cast<unsigned char>(user_query[cursor])) ||
        user_query[cursor] == '_')) {
        cursor++;
    }

    auto word = user_query.substr(start_of_word, cursor - start_of_word);
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    
    // TODO you will have to handle situations where the token is 2 words like "PRIMARY KEY"
    if (first_word_of_compound_keyword.contains(word)){
        skip_whitespace();
        
        size_t start_of_second_word = cursor;

        while (cursor < user_query.size() && std::isalpha(user_query[cursor])) {
            cursor++;
        }
        
        word += " " + user_query.substr(start_of_second_word, cursor - start_of_second_word);
    }

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


std::optional<Token> Lexer::tokenize_integer_literal(){
    if (!std::isdigit(static_cast<unsigned char>(user_query[cursor])))
        return std::nullopt;

    std::string number_as_str;

    while (std::isdigit(static_cast<unsigned char>(user_query[cursor]))){
        number_as_str += user_query[cursor];
        cursor++;
    }

    return Token(
        INT_LIT_TOK, 
        number_as_str
    );
}

std::optional<Token> Lexer::tokenize_boolean_or_string_literal(){
    if (user_query[cursor] != QUOTE_CHAR) return std::nullopt;

    // move past opening quote
    cursor++;
    size_t start_of_word = cursor;

    while (cursor < user_query.size() && user_query[cursor] != QUOTE_CHAR) {
        cursor++;
    }

    if (cursor == user_query.size()){
        return std::nullopt;
    }

    auto string_lit = user_query.substr(start_of_word, cursor - start_of_word);

    // move past closing quote
    cursor++;

    auto upper_string_lit = string_lit;

    std::transform(
        upper_string_lit.begin(), 
        upper_string_lit.end(), 
        upper_string_lit.begin(), 
        [](unsigned char c) {
            return std::toupper(c);
        }
    );

    if (boolean_key_words.contains(upper_string_lit)){
        return Token {
            TokenType::BOOL_LIT_TOK, 
            string_lit
        };
    }
    
    return Token {
        TokenType::STRING_LIT_TOK, 
        string_lit
    };
}



/*
    This function generates a token for the following types of literals
        1. integer literals
        2. boolean literals
        3. string literals
*/
std::optional<Token> Lexer::tokenize_literals(){

    if (auto literal_token = tokenize_integer_literal()){
        return *literal_token;
    }

    if (auto literal_token = tokenize_boolean_or_string_literal()){
        return *literal_token;
    }

    return std::nullopt;
}

/*
    This function tokenizes the 
*/
Token Lexer::get_next_token(){

    // skip white space & new lines
    skip_whitespace();

    if (cursor >= user_query.size()){
        return Token{TokenType::END_OF_QUERY_TOK, ""};
    }

    // find single chars like +, -, and (
    if (auto next_token = tokenize_single_chars()){
        return *next_token;
    }

    // keywords, identifiers
    if (auto next_token = tokenize_keywords_and_identifiers()){
        return *next_token;
    }

    // tokenizes strings
    if (auto next_token = tokenize_literals()){
        return *next_token;
    }

    
    return Token{TokenType::IDENTIFIER_TOK, ""};
};
