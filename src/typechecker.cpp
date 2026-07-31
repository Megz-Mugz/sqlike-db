#include "typechecker.hpp"

#include <print>

bool Typechecker::typecheck_ast(const AST& ast, Schema& schema){

    if (auto* create_table_ast = std::get_if<CreateTableAST>(&ast)) {

        schema.insert_table(*create_table_ast);

        code_gen.lower_create_ast(*create_table_ast);

        // Set a breakpoint on the next line.
        // std::println("Valid Create Statement");
    } else if (auto* insert_statement_ast = std::get_if<InsertStatementAST>(&ast)) {

        std::println("Valid Insert Statement");

    } else if (auto* update_statement_ast = std::get_if<UpdateStatementAST>(&ast)) {

        std::println("Valid Update Statement");
    
    
    } else if (auto* select_statement_ast = std::get_if<SelectStatementAST>(&ast)) {

        std::println("Valid Select Statement");
    } 
    
    else if (auto* drop_statement_ast = std::get_if<DropStatementAST>(&ast)) {

        std::println("Valid drop Statement");
    }

    else if (auto* delete_statement_ast = std::get_if<DeleteStatementAST>(&ast)) {

        std::println("Valid Delete Statement");
    
    } else {
        std::println("not valid, try again");

        return false;
    }

    std::println("Query is both syntactically & semantically correct, now onto code gen!");
    return true;
};
