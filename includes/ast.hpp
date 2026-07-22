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


using ColumnNames = std::string;
using ColumnValues = std::string;
using DBRows = std::vector<ColumnValues>;

// Insert Statement AST
class InsertStatementAST : public AST
{
public:
    std::vector<ColumnNames> columns;
    std::vector<DBRows> rows_to_insert;
};
