#ifndef TAU_TOKEN_HPP
#define TAU_TOKEN_HPP

#include <map>
#include <string>


namespace TAU
{

    class Token
    {
    public:
        enum class Type
        {
            FUNC, LEFT_PAREN, RIGHT_PAREN, VALUE, IDENTIFIER, SEMI
        };
        Type type;     // The type 
        int start_pos;      // Start position of the token on its line  
        int line_number;    // Line of source token came from
        std::string value;  // Value of the token
    };
}

#endif