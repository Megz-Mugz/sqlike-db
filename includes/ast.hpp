#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "tokentype.hpp"

struct ASTBase
{
    std::string table_name{};
};

struct ColumnData
{
    std::string col_name{};
    Type m_type{Type::UNKNOWN};
    Constraint m_constraint{Constraint::NONE};
};

struct CreateTableAST : ASTBase
{
    std::vector<ColumnData> columns;
};

using ColumnName = std::string;
using ColumnValue = std::string;
using DBRows = std::vector<ColumnValue>;

struct InsertStatementAST : ASTBase
{
    std::vector<ColumnName> columns;
    std::vector<DBRows> rows_to_insert;
};

struct UpdateStatementAST : ASTBase
{
    std::unordered_map<ColumnName, ColumnValue> values_to_set;
    std::unordered_map<ColumnName, ColumnValue> where_clause;
};

using AST = std::variant<
    CreateTableAST,
    InsertStatementAST,
    UpdateStatementAST
>;