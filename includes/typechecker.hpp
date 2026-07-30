#include "ast.hpp"
#include "schema.hpp"
#include "code_generator.hpp"

class Typechecker
{
public:
    CodeGenerator code_gen; // lowers to LLVM IR Builder

    bool typecheck_ast(const AST& ast, Schema& schema);
};