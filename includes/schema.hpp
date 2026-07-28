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

    /* 
    
    table_name -> 
        { col_name_1 -> {type, constraint} }
        { col_name_2 -> {type, constraint} }
        { col_name_3 -> {type, constraint} }
    */

    std::ofstream schema_file;

    using ColumnInfo = std::pair<Type, Constraint>;
    using ColumnStore = std::unordered_map<ColumnName, ColumnInfo>;
    
    std::unordered_map<std::string, ColumnStore> schema;

};
