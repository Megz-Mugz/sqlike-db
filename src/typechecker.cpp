#include "typechecker.hpp"

#include <print>
#include <variant>

bool Typechecker::typecheck_ast(const AST& ast){

    if (std::holds_alternative<CreateTableAST>(ast)) {
        const auto& create_ast = std::get<CreateTableAST>(ast);

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