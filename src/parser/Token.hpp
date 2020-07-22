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
            IDENTIFIER,

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

        std::string toString() const 
        {
            switch(type)
            {
            case Type::FUNC       : return "FUNC";
            case Type::ASM        : return "ASM";
            case Type::RETURN     : return "RETURN";
            case Type::INT        : return "INT";
            case Type::DOUBLE     : return "DOUBLE";
            case Type::CHAR       : return "CHAR";
            case Type::STRUCT     : return "STRUCT";
            case Type::INT_L      : return "INT_L";
            case Type::DOUBLE_L   : return "DOUBLE_L";
            case Type::CHAR_L     : return "CHAR_L";
            case Type::STRING_L   : return "STRING_L";
            case Type::IDENTIFIER : return "IDENTIFIER";
            case Type::SEMI       : return "SEMI";
            case Type::COMMA      : return "COMMA";
            case Type::COLON      : return "COLON";
            case Type::DOT        : return "DOT";
            case Type::PLUS       : return "PLUS";
            case Type::MINUS      : return "MINUS";
            case Type::SLASH      : return "SLASH";
            case Type::STAR       : return "STAR";
            case Type::D_STAR     : return "D_STAR";
            case Type::PCT        : return "PCT";
            case Type::AMP        : return "AMP";
            case Type::D_AMP      : return "D_AMP";
            case Type::PIPE       : return "PIPE";
            case Type::D_PIPE     : return "D_PIPE";
            case Type::HAT        : return "HAT";
            case Type::SHL        : return "SHL";
            case Type::SHR        : return "SHR";
            case Type::TILDE      : return "TILDE";
            case Type::LT         : return "LT";
            case Type::LTE        : return "LTE";
            case Type::GT         : return "GT";
            case Type::GTE        : return "GTE";
            case Type::EQ         : return "EQ";
            case Type::BANG_EQ    : return "BANG_EQ";
            case Type::BANG       : return "BANG";
            case Type::ASSIGN     : return "ASSIGN ";
            case Type::L_PAREN    : return "L_PAREN";
            case Type::R_PAREN    : return "R_PAREN";
            case Type::L_BRACE    : return "L_BRACE";
            case Type::R_BRACE    : return "R_BRACE";
            case Type::L_BRACKET  : return "L_BRACKET";
            case Type::R_BRACKET  : return "R_BRACKET";
            case Type::ARROW      : return "ARROW";
            default:
                return "TOKEN toString >> UNKNOWN TYPE";
            }
        }
    };
}

#endif