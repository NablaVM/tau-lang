#ifndef TAU_PARSER_HPP
#define TAU_PARSER_HPP

#include "Driver.hpp"
#include "Reporter.hpp"
#include "TokenStream.hpp"
#include <string>
#include <vector>

namespace TAU
{
    //! \brief Parser
    class Parser
    {
    public:

        //! \brief Construct the parser with a reference to the driver
        Parser();

        //! \brief Destruct the parser
        ~Parser();

        //! \brief Add a token stream to the parser
        //! \param stream The token stream to parse 
        void addTokenStream(TokenStream * stream);

        //! \brief Parse token streams
        void parse();

    private:
        Reporter reporter;

        std::vector<TokenStream *> streams;

        void parseStream(TokenStream * stream);
    };
}

#endif