#ifndef TAU_TOKEN_STREAM_HPP
#define TAU_TOKEN_STREAM_HPP

#include "Token.hpp"
#include "Reporter.hpp"

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

    //! \brief Tokenize a vector of lines of source
    void createStreamFromVector(std::string source_name, std::vector<std::string> & source_vector);

    //! \brief Get file name 
    //! \returns std::string of the source name given to represent the stream
    std::string getFileName() const;

    //! \brief Get the size of the stream
    //! \returns vector of tokens that is the built stream
    std::vector<Token> getStream() const;

private:
    Reporter                           reporter;
    std::string                        origin_source_name;
    std::vector<Token>                 stream;
    std::map<std::string, Token::Type> keywords;

    // Tokenize the file
    void tokenize(std::vector<std::string> & file_contents);
};
}

#endif
