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
    
    void Parser::parseStream(TokenStream * stream)
    {
        std::vector<Token> strm = stream->getStream();

        for(std::size_t sidx = 0; sidx < strm.size(); )
        {

            //  Accept tokens specfic to the parser state
            //
            switch(parser_state)
            {
                case State::START:
                {
                    if(strm[sidx].type == Token::Type::FUNC){ parser_state = State::FUNCTION_DECLS; }
                    else 
                    {
                        reporter.issueInternalError("Unexpected token in start state. Expected func");
                    }
                    ++sidx;
                    break;
                }

                case State::FUNCTION_DECLS:
                {
                    if(strm.size() < 2){ reporter.issueInternalError("Not enough toks"); }

                    if(! (strm[sidx].type == Token::Type::IDENTIFIER)) { reporter.issueInternalError("Expected identifier -> Issue report"); }
                    if(! (strm[sidx+1].type == Token::Type::L_PAREN))    { reporter.issueInternalError("Expected ) -> Issue report");          }

                    // Here we could instantiate a function representation that the rest of the state would build.
                    // Once we hit the end of the function block we can push it to a list of things that need to be
                    // optimized / generated

                    parser_state = State::FUNCTION_PARAMS;

                    // Increase index by 2 because we just consumed 2
                    //
                    sidx += 2;
                    break;
                }

                case State::FUNCTION_PARAMS:
                {

                    if(sidx + 3 > strm.size()) { reporter.issueInternalError("Not enough toks"); }

                    if(! (strm[sidx].type   != Token::Type::IDENTIFIER) ){ reporter.issueInternalError("Expected identifier. -> give real report here"); }
                    if(! (strm[sidx+1].type != Token::Type::COLON))      { reporter.issueInternalError("Expected colon. "); }
                    if(! (strm[sidx+2].type != Token::Type::CHAR
                       && strm[sidx+2].type != Token::Type::DOUBLE
                       && strm[sidx+2].type != Token::Type::INT))        { reporter.issueInternalError("Unknown data type given for param"); }

                    if(strm[sidx+3].type == Token::Type::COMMA)
                    {
                        // We expect more from this
                    }
                    else if(strm[sidx+3].type == Token::Type::L_PAREN)
                    {
                        parser_state = State::RETURN_VALUE;
                    }
                    else
                    {
                        reporter.issueInternalError("Unexpected token");
                    }

                    sidx += 3;
                    break;
                }

                case State::RETURN_VALUE:
                {
                    if(strm[sidx].type == Token::Type::L_BRACE)
                    {
                        // No return type
                        parser_state = State::FUNCTION_BLOCK;
                        ++sidx;
                    }
                    else
                    {
                        if(sidx + 2 < strm.size()) { reporter.issueInternalError("Not enough toks"); }

                        if(! (strm[sidx].type   == Token::Type::ARROW)) { reporter.issueInternalError("Expected arrow.. didn't get an arrow. We need an arrow"); }
                        if(! (strm[sidx+1].type != Token::Type::CHAR
                           && strm[sidx+1].type != Token::Type::DOUBLE
                           && strm[sidx+1].type != Token::Type::INT))   { reporter.issueInternalError("Unknown data type given for return type"); }

                        if(! (strm[sidx+2].type != Token::Type::L_BRACKET)) { reporter.issueInternalError("Expected { but we didn't get one"); }

                        //  Go to the next state!
                        //
                        parser_state = State::FUNCTION_BLOCK;
                        sidx+=2;
                    }
                    break;
                }

                case State::FUNCTION_BLOCK:
                {
                    break;
                }

                default:
                    reporter.issueInternalError("Parser reached default state");
                    break;
            }

        }
    }
}