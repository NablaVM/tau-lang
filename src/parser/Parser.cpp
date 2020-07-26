#include "Parser.hpp"

#include "Token.hpp"
#include "TokenStream.hpp"

#include <iostream>

namespace TAU
{
    // -----------------------------------------
    //
    // -----------------------------------------
    
    Parser::Parser() : reporter("TAU::Parser")
    {

    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    Parser::~Parser()
    {
        for(int i = 0; i < (int)streams.size(); i++ )
        {
            if(streams[i])
            {
                delete streams[i];
            }
        }
        streams.clear();
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Parser::addTokenStream(TokenStream * stream)
    {
       streams.push_back(stream);
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Parser::parse()
    {
        for(auto stream : streams)
        {
            //  Set the parser state to the start position
            //
            parser_state = State::START;

            //  Parse the stream
            //
            parseStream(stream);
        }
    }

    // -----------------------------------------
    //
    // -----------------------------------------

    bool Parser::tokenIsDataType(Token token)
    {
        return( token.type == Token::Type::CHAR   || 
                token.type == Token::Type::DOUBLE ||
                token.type == Token::Type::INT);
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Parser::parseStream(TokenStream * stream)
    {
        std::vector<Token> strm = stream->getStream();

        for(std::size_t sidx = 0; sidx < strm.size(); )
        {
        }
    }
}
