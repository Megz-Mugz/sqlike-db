#pragma once

#include "tokentype.hpp"

struct Token {
    TokenType token_type;
    std::string text;
}
