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
            FUNC, LEFT_PAREN, RIGHT_PAREN, VALUE, IDENTIFIER, SEMI
        };

        Type type;          //! The type 
        int start_pos;      //! Start position of the token on its line  
        int line_number;    //! Line of source token came from
        std::string value;  //! Value of the token
    };
}

#endif