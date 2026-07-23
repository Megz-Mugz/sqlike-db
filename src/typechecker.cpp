#include "typechecker.hpp"

#include <print>

bool Typechecker::typecheck_ast(const AST& ast, Schema& schema){

    if (std::holds_alternative<CreateTableAST>(ast)) {
        auto create_table_ast = std::get<CreateTableAST>(ast);

        schema.insert_table(create_table_ast);

        // Set a breakpoint on the next line.
        std::println("Valid Create Statement");
    } else if (std::holds_alternative<InsertStatementAST>(ast)) {
        std::println("Valid Insert Statement");
    } else if (std::holds_alternative<UpdateStatementAST>(ast)) {
        std::println("Valid Update Statement");
    }

    std::println("Query is both syntactically & semantically correct, now onto code gen!");
    return true;
};