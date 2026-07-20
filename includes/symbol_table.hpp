#include <unordered_map>

#include "tokentype.hpp"



/*
    Represents the various properties of each column. 
*/
struct ColumnData {

    Type m_type;
    Constraint m_constraint;
    bool is_primary_key;
    bool is_foreign_key;
    bool has_default;

    // int -> default 0
    // text -> default ''
    // bool -> default false
};

/*
    Each table is composed of columns which have types & constraints. 
*/
struct TableData {

    std::unordered_map<std::string column_name, ColumnData> columns_and_data;

};


/*
    Lookups are done with the table name.  
*/
class SymbolTable
{

public:
    
    // actions to perform with the symbol table
    auto get_symbol_table_size();
    auto insert_table();
    auto delete_table();
    auto lookup_table();


private:
    std::unordered_map<std::string table_name, TableData> symbol_table;

}