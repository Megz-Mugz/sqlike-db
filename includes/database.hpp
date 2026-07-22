#include "parser.hpp"
#include "schema.hpp"
#include "code_generator.hpp"
#include "executor.hpp"

class Database
{

public:
    Parser parser; // parses & creates AST
    Schema schema; // every DB has a schema
    CodeGenerator code_gen; // lowers to LLVM IR Builder
    Executor executor; // plans & executes query


    void launch_db();
};