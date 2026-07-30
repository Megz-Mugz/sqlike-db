#pragma once

#include "ast.hpp"
#include <fstream>

class Schema 
{

public:
    // auto get_schema();
    void clear();

    void insert_table(const CreateTableAST& create_table_ast);
    // auto alter_table();
    // auto drop_table();

private:

    std::ofstream schema_file;

    using ColumnInfo = std::pair<Type, Constraint>;
    using ColumnStore = std::unordered_map<ColumnName, ColumnInfo>;
    
    std::unordered_map<std::string, ColumnStore> schema;

    void write_to_schema_file(const CreateTableAST& create_table_ast);

    void write_to_actual_schema(const TableName table_name,
                                const ColumnName col_name, 
                                const Type col_type, 
                                const Constraint col_constraint);

};
