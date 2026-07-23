#include "parser.hpp"
#include "schema.hpp"
#include "typechecker.hpp"
#include "code_generator.hpp"
#include "executor.hpp"

class Database
{

public:
    Parser parser; // parses & creates AST
    Schema schema; // every DB has a schema
    Typechecker typechecker;
    CodeGenerator code_gen; // lowers to LLVM IR Builder
    Executor executor; // plans & executes query


    void launch_db();
};