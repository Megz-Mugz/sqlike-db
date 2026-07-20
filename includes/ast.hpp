#pragma once

#include <iostream>
#include <vector>

#include "tokentype.hpp"

struct ColumnData {

    std::string col_name{};
    Type m_type{Type::UNKNOWN};
    Constraint m_constraint{Constraint::NONE};
};


struct CreateTableAST {
    std::string table_name{};
    std::vector<ColumnData> columns;
};
