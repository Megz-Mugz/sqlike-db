#include <iostream>
#include "schema.hpp"

void Schema::clear()
{
    schema.clear();

    schema_file.close();
    schema_file.open("schema.sqlike", std::ios::out | std::ios::trunc);

    if (!schema_file.is_open()) {
        std::cerr << "Error clearing schema file" << std::endl;
    }
}

/*
    This function serves to do a few things

    1. adds the AST to the schema

    2. writes table to the schema.sqlike file

*/
void Schema::insert_table(const CreateTableAST& create_table_ast) 
{

    if (!schema_file.is_open()){
        std::cerr << "Error writing to schema file" << std::endl;
    }

    // write to schema here
    
    schema_file << "Table Name: " << create_table_ast.table_name << " {" << "\n";
    
    for (const auto [col_name, col_type, col_constraint] : create_table_ast.columns){
        schema_file << "\t" << to_string(col_type) << ": " << col_name << ", " << to_string(col_constraint) << "\n";
    }

    schema_file << "}\n\n";
}
