#include "typechecker.hpp"

#include <print>

bool Typechecker::typecheck_ast(const AST& ast, Schema& schema){

    if (std::holds_alternative<CreateTableAST>(ast)) {
        auto create_table_ast = std::get<CreateTableAST>(ast);

        schema.insert_table(create_table_ast);

        code_gen.lower_create_ast(create_table_ast);

        // Set a breakpoint on the next line.
        std::println("Valid Create Statement");
    } else if (std::holds_alternative<InsertStatementAST>(ast)) {
        auto insert_statement_ast = std::get<InsertStatementAST>(ast);

        std::println("Valid Insert Statement");

    } else if (std::holds_alternative<UpdateStatementAST>(ast)) {
        auto update_statement_ast = std::get<UpdateStatementAST>(ast);

        std::println("Valid Update Statement");
    
    
    } else if (std::holds_alternative<SelectStatementAST>(ast)) {
        auto select_statement_ast = std::get<SelectStatementAST>(ast);

        std::println("Valid Select Statement");
    } 
    
    else if (std::holds_alternative<DropStatementAST>(ast)) {
        auto drop_statement_ast = std::get<DropStatementAST>(ast);

        std::println("Valid drop Statement");
    }

    else if (std::holds_alternative<DeleteStatementAST>(ast)) {
        auto delete_statement_ast = std::get<DeleteStatementAST>(ast);

        std::println("Valid Delete Statement");
    
    } else {
        std::println("not valid, try again");

        return false;
    }

    std::println("Query is both syntactically & semantically correct, now onto code gen!");
    return true;
};