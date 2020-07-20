#ifndef TAU_TOKEN_STREAM_HPP
#define TAU_TOKEN_STREAM_HPP

#include "Token.hpp"

#include <string>
#include <vector>

namespace TAU
{
    class TokenStream
    {
    public:
        TokenStream();

        void createStreamFromFile(std::string file);

    private:
        std::vector<Token::Type> t_stream;
        std::map<std::string, Token::Type> keywords;
    };
}


#endif