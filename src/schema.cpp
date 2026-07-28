#include <iostream>
#include <fstream> 
#include "schema.hpp"

/*
    This function serves to do a few things

    1. adds the AST to the schema

    2. writes table to the schema.sqlike file

*/
void Schema::insert_table(const CreateTableAST& create_table_ast) 
{
    std::ofstream schema("schema.sqlike");

    if (!schema.is_open()){
        std::cerr << "Error writing to schema file" << std::endl;
    }

    // write to schema here
    
    schema << "Table Name: " << create_table_ast.table_name << " {" << "\n";
    
    for (const auto [col_name, col_type, col_constraint] : create_table_ast.columns){
        schema << "\t" << to_string(col_type) << ": " << col_name << ", " << to_string(col_constraint) << "\n";
    }

    schema << "}";

    schema.close();
}