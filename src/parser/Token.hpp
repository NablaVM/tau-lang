#ifndef TAU_TOKEN_HPP
#define TAU_TOKEN_HPP

#include <map>
#include <string>

// IDENTIFIER COLON INT ASSIGN      VALUE ADD VALUE MULT VALUE SEMI 
//     x       :    int   =           3    +   2     *     p     ;
//     68      68   68    68         68   68   69    70    71    71

namespace TAU
{
    //! \brief A single token item
    class Token
    {
    public:

        //! \brief The token type - Something that identifies a particular element
        enum class Type
        {
            // Meta keywords
            FUNC, ASM, RETURN,

            // Primitive Tokens
            INT, DOUBLE, CHAR, STRUCT, 

            // Literal Tokens
            INT_L, DOUBLE_L, CHAR_L, STRING_L,
            
            // Abstract Tokens
            VALUE, IDENTIFIER,

            // Symbol tokens
            SEMI, COMMA, COLON, DOT,
            PLUS, MINUS, SLASH, STAR, D_STAR, PCT, 
            AMP, D_AMP, PIPE, D_PIPE, 
            HAT, SHL, SHR, TILDE,
            LT, LTE, GT, GTE, EQ, BANG_EQ, BANG,
            ASSIGN,

            L_PAREN,   R_PAREN, 
            L_BRACE,   R_BRACE, 
            L_BRACKET, R_BRACKET, 

            ARROW, 
        };

        Type type;          //! The type 
        int start_pos;      //! Start position of the token on its line  
        int line_number;    //! Line of source token came from
        std::string value;  //! Value of the token
    };
}

#endif