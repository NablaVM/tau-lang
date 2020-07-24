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
        //! \param reporter The object that will be used to report issues
        //! \param driver The compilation driver, given to the parser
        //!        so it can request a search for more token streams
        //!        in the event there are unresolved symbols
        Parser(Reporter & reporter, Driver & driver);

        //! \brief Destruct the parser
        ~Parser();

        //! \brief Add a token stream to the parser
        //! \param stream The token stream to parse 
        void addTokenStream(TokenStream * stream);

        //! \brief Begin parsing 
        void begin();

    private:
        Reporter & reporter;
        Driver & driver;

        std::vector<TokenStream *> streams;

        void parseStream(TokenStream * stream);
    };
}

#endif