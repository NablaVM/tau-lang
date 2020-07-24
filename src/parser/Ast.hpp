#ifndef AST_HPP
#define AST_HPP

#include <any>

namespace TAU
{

class Ast
{
public:
    enum class Variant
    {
        TOKEN,      // Ast emerged from a single token
        ARITH_EXPR  // Ast represents an expression
    };

    Ast(Variant variant, std::any value) 
        : ast_variant(variant), value(value), left(nullptr), right(nullptr){}

    Ast(Variant variant, std::any value, Ast * left, Ast * right) 
        : ast_variant(variant), value(value), left(left), right(right){}

    Variant ast_variant;
    std::any value;
    Ast * left; 
    Ast * right;
};

}

#endif