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