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

    ColumnData() = default;

    ColumnData(std::string name)
        : col_name(std::move(name))
    {}

    ColumnData(std::string name, Type type)
        : col_name(std::move(name)), 
          m_type(type)
    {}

    ColumnData(std::string name, Type type, Constraint constraint)
        : col_name(std::move(name)), 
          m_type(type), 
          m_constraint(constraint)
    {}
    
};

struct CreateTableAST : ASTBase
{
    std::vector<ColumnData> columns;
};

using ColumnName = std::string;
using ColumnValue = std::string;
using DBRows = std::vector<ColumnData>;

struct InsertStatementAST : ASTBase
{
    std::vector<ColumnData> columns;
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