#pragma once

#include <string>
#include <vector>

#include "tokentype.hpp"

class AST
{
public:
    std::string table_name{};

    // virtual void printAST() const;

    // virtual void lower() const;
};


// Create Table AST
struct ColumnData
{
    std::string col_name{};
    Type m_type{Type::UNKNOWN};
    Constraint m_constraint{Constraint::NONE};
};
class CreateTableAST : public AST
{
public:
    std::vector<ColumnData> columns;
};


using ColumnName = std::string;
using ColumnValue = std::string;
using DBRows = std::vector<ColumnValue>;

// Insert Statement AST
class InsertStatementAST : public AST
{
public:
    std::vector<ColumnName> columns;
    std::vector<DBRows> rows_to_insert;
};

// Update Statement AST

class UpdateStatementAST : public AST 
{
public:
    std::unordered_map<ColumnName, ColumnValue> values_to_set;
    std::unordered_map<ColumnName, ColumnValue> where_clause;
};

// Select Statement AST

// Delete Statement AST

// Drop Statement AST