#ifndef TAU_TOKEN_STREAM_HPP
#define TAU_TOKEN_STREAM_HPP

#include "Token.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace TAU
{
    //! \brief A stream of tokens to be parsed by the parser
    class TokenStream
    {
    public:
        //! \brief Create a token stream
        TokenStream();

        //! \brief Tokenize a source file
        void createStreamFromFile(std::string file);

        //! \brief Get file name 
        std::string getFileName() const;

    private:
        std::string origin_file;
        std::vector<Token> t_stream;
        std::map<std::string, Token::Type> keywords;
        std::set<char> break_points;

        // Tokenize the file
        void tokenize(std::vector<std::string> file_contents);
    };
}

#endif
