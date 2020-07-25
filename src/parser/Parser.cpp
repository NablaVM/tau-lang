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

            //  Accept tokens specfic to the parser state
            //
            switch(parser_state)
            {
                case State::START:
                {
                    if(strm[sidx].type == Token::Type::FUNC){ parser_state = State::FUNCTION_DECLS; }
                    else 
                    {
                        std::cout << "---> Token type: " << strm[sidx].toString(true) << std::endl;
                        reporter.issueInternalError("Unexpected token in start state. Expected func");
                    }
                    ++sidx;
                    break;
                }

                case State::FUNCTION_DECLS:
                {
                    if(strm.size() < 2){ reporter.issueInternalError("Not enough toks"); }

                    if(! (strm[sidx].type == Token::Type::IDENTIFIER)) { reporter.issueInternalError("Expected identifier (function decl) -> Issue report"); }
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
                    if(strm[sidx].type == Token::Type::R_PAREN)
                    {
                        if(strm[sidx-1].type == Token::Type::COMMA)
                        {
                            std::cout << "Expected parameter preceeding comma, got ')' instead" << std::endl;
                            reporter.issueInternalError("Invalid syntax -> report it");
                        }

                        parser_state = State::RETURN_VALUE;
                        ++sidx;
                        continue;
                    }

                    if(sidx + 4 > strm.size()) { reporter.issueInternalError("Not enough toks"); }

                    if(strm[sidx].type != Token::Type::IDENTIFIER)
                    {
                        std::cout << "Expected identifier while parsing parameter types" << std::endl;
                        std::cout << "Got : " << strm[sidx].toString(true) << " instead" << std::endl;
                        reporter.issueInternalError("Expected id in params");
                    }

                    if(strm[sidx+1].type != Token::Type::COLON)
                    {
                        reporter.issueInternalError("Expected colon while parsing func params");
                    }

                    if(! tokenIsDataType(strm[sidx+2]) )
                    {
                        reporter.issueInternalError("Expected data type while parsing param");
                    }

                    //  If the next thing is a comma we need to get the next chunk of params
                    //
                    if(strm[sidx+3].type == Token::Type::COMMA)
                    {
                        sidx+=4;
                        continue;
                    }
                    else if(strm[sidx+3].type == Token::Type::R_PAREN)
                    {
                        sidx+=4;
                        parser_state = State::RETURN_VALUE;
                        continue;
                    }

                    break;
                }

                case State::RETURN_VALUE:
                {
/*

                        This needs to be updated to use tokenIsDataType() and it needs to get multiple
                        return types. Right now it only handles one

*/


                    if(strm[sidx].type == Token::Type::L_BRACE)
                    {
                        // No return type
                        parser_state = State::FUNCTION_BLOCK;
                        ++sidx;
                    }
                    else
                    {
                        if(sidx + 2 > strm.size()) { reporter.issueInternalError("Not enough toks"); }

                        if(! (strm[sidx].type   == Token::Type::ARROW)) { reporter.issueInternalError("Expected arrow.. didn't get an arrow. We need an arrow"); }
                        if(! (strm[sidx+1].type != Token::Type::CHAR
                           || strm[sidx+1].type != Token::Type::DOUBLE
                           || strm[sidx+1].type != Token::Type::INT))   
                        {
                               reporter.issueInternalError("Unknown data type given for return type"); 
                        }

                        if(! (strm[sidx+2].type != Token::Type::L_BRACKET)) { reporter.issueInternalError("Expected { but we didn't get one"); }

                        //  Go to the next state!
                        //
                        parser_state = State::FUNCTION_BLOCK;
                        sidx+=3;
                    }
                    break;
                }

                case State::FUNCTION_BLOCK:
                {
                    if(strm[sidx].type == Token::Type::R_BRACE)
                    {
                        // No function data
                        ++sidx;
                        parser_state = State::START;
                        break;
                    }

                    // Accept statements until '}' is reached 
                    //
                    bool completed = false;
                    acceptStatements(strm, sidx, completed);


                    parser_state = State::START;
                    break;
                }

                default:
                    reporter.issueInternalError("Parser reached default state");
                    break;
            }

        }
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    std::vector<Token> Parser::getNextStatement(std::vector<Token> & strm, std::size_t & idx, bool & complete)
    {
        /*
            This needs to handle special cases!!!

            This works \  x : int := 43; 

            This wont | 

                if(condition){

                    blah;
                    blah;
                    blah;

                } 

            If we hit the } on the if statement we will trigger a function end which is wrong

            If we hit 'blah;' the statement [   if(condition){blah; ] woule be returned. 

            We need to handle things like lookinf for (if) and call a different function to pull 
            the if from the stream. 

            Same thing would happen with any other block thing. But this works great for assignments and reassignments
            and we could totally call an if grabber then in that func call this func to build its statements. 

            Wild
        
        */


        //  Check for completion condition
        //
        if(strm[idx].type == Token::Type::R_BRACE)
        { 
            ++idx;              // Step idx so we don't do anything with the }
            complete = true;    // Indicate that everything is done
            return {};          // Return out of the function
        }

        //  Statement to hand back
        //
        std::vector<Token> statement;

        //  Iterate stream from current idx until ';' or end of stream
        //
        for(; idx < strm.size(); idx++)
        {
            //  If we reach the end of the stream without hitting a ';' we fail
            //
            if(idx == strm.size()-1){ reporter.issueInternalError("Unterminated statement"); }

            //  If we find a ';' we want to return the statement, otherewise we need to 
            //  add that token to the return statement
            //
            if(strm[idx].type == Token::Type::SEMI)
            {
                idx++; // Skip the semi
                return statement;
            }
            else
            {
                statement.push_back(strm[idx]);
            }
        }

        // Keep the compiler happy
        //
        return {};
    }
    
    // -----------------------------------------
    //
    // -----------------------------------------

    std::vector<Token> Parser::getIfStatement(std::vector<Token> & strm, std::size_t & idx, bool & complete)
    {
        reporter.issueInternalError("If Parsing not yet complete");
        return {};
    }
    
    // -----------------------------------------
    //
    // -----------------------------------------

    std::vector<Token> Parser::getWhileStatement(std::vector<Token> & strm, std::size_t & idx, bool & complete)
    {
        reporter.issueInternalError("While Parsing not yet complete");
        return {};
    }
    
    // -----------------------------------------
    //
    // -----------------------------------------

    std::vector<Token> Parser::getForLoopStatement(std::vector<Token> & strm, std::size_t & idx, bool & complete)
    {
        reporter.issueInternalError("ForLoop Parsing not yet complete");
        return {};
    }
    
    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Parser::acceptStatements(std::vector<Token> & strm, std::size_t & idx, bool & complete)
    {
        std::cout << "acceptStatements: idx : " << idx << std::endl;
        std::vector<Token> stmt = getNextStatement(strm, idx, complete);

        if(complete){ return; }

        //  Need to determine statement type. We COULD hash the tokens ? nah..
        //  Idk, need to determine type of statement to build
        //


        std::cout << "Got a statement of size : " << stmt.size() << std::endl;

        for(auto t : stmt)
        {
            std::cout << t.toString() << " ";
        }
        std::cout << std::endl;

        //reporter.issueInternalError("Just here for a test");

        //  Here we recurse on the acceptStatements method until the function is completed
        //
        acceptStatements(strm, idx, complete);
    }
}