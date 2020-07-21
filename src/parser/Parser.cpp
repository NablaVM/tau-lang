#include "Parser.hpp"

#include "Token.hpp"
#include "TokenStream.hpp"

#include <iostream>

namespace TAU
{
    // -----------------------------------------
    //
    // -----------------------------------------
    
    Parser::Parser(Driver & driver) : driver(driver)
    {
        /*
            The driver is given to the parser so if we run out of token streams and have
            unresolved items we can request that the driver search for it and generate a
            token stream for us
        */
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
    
    void Parser::begin()
    {
        std::cout << "Parser <todo> : Begin parsing token streams" << std::endl;

        for(auto stream : streams)
        {
            parseStream(stream);
        }
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Parser::parseStream(TokenStream * stream)
    {
        std::cout << "Parser <todo>  : Parse stream for file \"" << stream->getFileName() << "\"" << std::endl;
    }
}