#ifndef LEXEME_HPP
#define LEXEME_HPP


namespace TAU
{

//! \brief A single unit of language that is expected to yield an AST
class Lexeme
{
public:

    enum class Variant
    {
        TOKEN,
        TOKEN_GROUP
    };

    Lexeme(Variant variant) : variant(variant){}

    //! \brief Retrieve the Ast from a lexeme
    //virtual Ast * getAst() = 0;

    Variant variant;
};


}

#endif