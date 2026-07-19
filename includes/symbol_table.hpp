#include <unordered_map>

typedef struct ColumnData {

    Type m_type;
    Constraint m_constraint;
    bool is_primary_key;
    bool is_foreign_key;

} ColumnData;

/*
    Each table is composed of 
*/
typedef struct TableData {

    std::unordered_map<std::string column_name, ColumnData> columns_and_data;

} TableData;

class SymbolTable
{

public:

    auto get_symbol_table_size();
    auto insert_element();
    auto delete_element();
    auto lookup_element();


private:
    std::unordered_map<std::string table_name, TableData> symbol_table;

}