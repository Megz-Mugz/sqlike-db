#pragma once

#include <iostream>
#include <vector>

#include "tokentype.hpp"

struct AST 
{
    virtual void printAST() const;

    virtual void lower();
};


// Create Table AST
struct ColumnData 
{
    std::string col_name{};
    Type m_type{Type::UNKNOWN};
    Constraint m_constraint{Constraint::NONE};
};
struct CreateTableAST {
    std::string table_name{};
    std::vector<ColumnData> columns;
};


using ColumnNames = std::string;
using ColumnValues = std::string;

// Insert Statement AST
struct InsertStatementAST 
{
    std::string table_name{};
    std::vector<ColumnNames> columns;
    std::vector<std::vector<ColumnValues>> rows_to_insert;
};
