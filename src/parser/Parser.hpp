#ifndef TAU_PARSER_HPP
#define TAU_PARSER_HPP

#include "Ast.hpp"
#include "Lexeme.hpp"
#include "Driver.hpp"
#include "Reporter.hpp"
#include "TokenStream.hpp"
#include <string>
#include <vector>
#include <map>

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

        enum class State
        {
            START,
            FUNCTION_DECLS,
            FUNCTION_PARAMS,
            RETURN_VALUE,
            FUNCTION_BLOCK
        };

        State parser_state;

        Reporter reporter;

        std::vector<TokenStream *> streams;

        void parseStream(TokenStream * stream);

        std::vector<Token> getNextStatement(std::vector<Token> & strm, std::size_t & idx, bool & complete);

        void acceptStatements(std::vector<Token> & strm, std::size_t & idx, bool & complete);
    };
}

#endif