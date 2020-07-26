#ifndef TAU_TOKEN_HPP
#define TAU_TOKEN_HPP

#include <map>
#include <string>

// IDENTIFIER COLON INT ASSIGN      VALUE ADD VALUE MULT VALUE SEMI 
//     x       :    int   =           3    +   2     *     p     ;
//     68      68   68    68         68   68   69    70    71    71

namespace TAU
{
class TokenStream;

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

    //! \brief Construct a token
    //! \param type  The type of the token
    //! \param col   The column that the token was found on
    //! \param line  The line number the token was found on
    //! \param value The value of the token
    Token(Type type, TokenStream const &source, std::size_t line, std::size_t col, std::string value) 
        : type(type), source(source), line(line), col(col), value(value){}

    //! \brief Construct a token with only a type for building lexemes to match
    //!        against token streams
    //! \param type The type of the token
    //Token(Type type, TokenStream const &source) : type(type), source(source), line(0), col(0), value(""){}

    Type               type;   //! The type 
    TokenStream const &source; //! TokenStream where this token lives.
    std::size_t        line;   //! Line of source token came from
    std::size_t        col;    //! Start position of the token on its line  
    std::string        value;  //! Value of the token

    inline std::string toString(bool verbose = false) const 
    {
        std::string type_string;
        switch(type)
        {
        case Type::FUNC       : type_string = "FUNC";       break;
        case Type::ASM        : type_string = "ASM";        break;
        case Type::RETURN     : type_string = "RETURN";     break;
        case Type::INT        : type_string = "INT";        break;
        case Type::DOUBLE     : type_string = "DOUBLE";     break;
        case Type::CHAR       : type_string = "CHAR";       break;
        case Type::STRUCT     : type_string = "STRUCT";     break;
        case Type::INT_L      : type_string = "INT_L";      break;
        case Type::DOUBLE_L   : type_string = "DOUBLE_L";   break;
        case Type::CHAR_L     : type_string = "CHAR_L";     break;
        case Type::STRING_L   : type_string = "STRING_L";   break;
        case Type::IDENTIFIER : type_string = "IDENTIFIER"; break;
        case Type::SEMI       : type_string = "SEMI";       break;
        case Type::COMMA      : type_string = "COMMA";      break;
        case Type::COLON      : type_string = "COLON";      break;
        case Type::DOT        : type_string = "DOT";        break;
        case Type::PLUS       : type_string = "PLUS";       break;
        case Type::MINUS      : type_string = "MINUS";      break;
        case Type::SLASH      : type_string = "SLASH";      break;
        case Type::STAR       : type_string = "STAR";       break;
        case Type::D_STAR     : type_string = "D_STAR";     break;
        case Type::PCT        : type_string = "PCT";        break;
        case Type::AMP        : type_string = "AMP";        break;
        case Type::D_AMP      : type_string = "D_AMP";      break;
        case Type::PIPE       : type_string = "PIPE";       break;
        case Type::D_PIPE     : type_string = "D_PIPE";     break;
        case Type::HAT        : type_string = "HAT";        break;
        case Type::SHL        : type_string = "SHL";        break;
        case Type::SHR        : type_string = "SHR";        break;
        case Type::TILDE      : type_string = "TILDE";      break;
        case Type::LT         : type_string = "LT";         break;
        case Type::LTE        : type_string = "LTE";        break;
        case Type::GT         : type_string = "GT";         break;
        case Type::GTE        : type_string = "GTE";        break;
        case Type::EQ         : type_string = "EQ";         break;
        case Type::BANG_EQ    : type_string = "BANG_EQ";    break;
        case Type::BANG       : type_string = "BANG";       break;
        case Type::ASSIGN     : type_string = "ASSIGN ";    break;
        case Type::L_PAREN    : type_string = "L_PAREN";    break;
        case Type::R_PAREN    : type_string = "R_PAREN";    break;
        case Type::L_BRACE    : type_string = "L_BRACE";    break;
        case Type::R_BRACE    : type_string = "R_BRACE";    break;
        case Type::L_BRACKET  : type_string = "L_BRACKET";  break;
        case Type::R_BRACKET  : type_string = "R_BRACKET";  break;
        case Type::ARROW      : type_string = "ARROW";      break;
        default               : type_string = "UNKNOWN";    break;
        }

        if(!verbose){ return type_string; }

        return std::to_string(line) + ":" + std::to_string(col) + "(" + type_string + "): " + value;
    }
};
}

#endif
