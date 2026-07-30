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
    write_to_schema_file(create_table_ast);
}

/*
    Writes to the schema file for users to see what their database looks like
*/
void Schema::write_to_schema_file(const CreateTableAST& create_table_ast){
    if (!schema_file.is_open()){
        std::cerr << "Error writing to schema file" << std::endl;
    }

    // write to schema here
    
    auto table_name = create_table_ast.table_name ;

    schema_file << "Table Name: " << table_name << " {" << "\n";
    
    for (const auto& [col_name, col_type, col_constraint] : create_table_ast.columns){
        write_to_actual_schema(table_name, col_name, col_type, col_constraint);
        schema_file << "\t" << to_string(col_type) << ": " << col_name << ", " << to_string(col_constraint) << "\n";
    }

    schema_file << "}\n\n";
}

void Schema::write_to_actual_schema(const TableName table_name,
                                const ColumnName col_name, 
                                const Type col_type, 
                                const Constraint col_constraint){
    schema[table_name][col_name] = {col_type, col_constraint};
}