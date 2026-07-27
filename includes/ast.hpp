#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "tokentype.hpp"

using TableName = std::string;

struct ASTBase
{
    TableName table_name{};
};

struct ColumnData
{
    std::string col_name{NAME_NOT_SPECIFIED};
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
using ColumnName = std::string;

using ColumnValue = std::string;
using DBRows = std::vector<ColumnData>;

struct CreateTableAST : ASTBase
{
    std::vector<ColumnData> columns;
};



struct InsertStatementAST : ASTBase
{
    std::vector<ColumnData> columns;
    std::vector<DBRows> rows_to_insert;
};


/* 

    TODO refactor the code so it doesn't use pairs with "first" & "second"

    we can instead make a 2 member struct for proper names
*/
struct UpdateStatementAST : ASTBase
{
    std::unordered_map<ColumnName, std::pair<ColumnValue, Type>> values_to_set;
    std::unordered_map<ColumnName, std::pair<ColumnValue, Type>> where_clause;
};

struct SelectStatementAST : ASTBase
{
    std::vector<std::pair<ColumnName, TableName>> selected_cols;
    TableName join_table;
    std::pair<std::pair<ColumnName, TableName>, std::pair<ColumnName, TableName>> join_cols; // keep outer pair
    std::unordered_map<ColumnName, std::pair<ColumnValue, Type>> where_clause;
};

struct DropStatementAST : ASTBase {};

struct DeleteStatementAST : ASTBase 
{
    std::unordered_map<ColumnName, std::pair<ColumnValue, Type>> where_clause;
};

using AST = std::variant<
    CreateTableAST,
    InsertStatementAST,
    UpdateStatementAST, 
    SelectStatementAST, 
    DropStatementAST, 
    DeleteStatementAST
>;