#pragma once

#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <optional>

#include "tokentype.hpp"


struct Column {
    std::string column_name;
    TokenType datatype;
    std::optional<TokenType> constraint;
};

struct CREATE_STATEMENT_AST {
    std::string table_name;
    std::set<Column> columns;
};