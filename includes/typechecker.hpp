#include "ast.hpp"
#include "schema.hpp"

class Typechecker
{
public:
    bool typecheck_ast(const AST& ast, Schema& schema);
};