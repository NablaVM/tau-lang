#include <iostream>
#include <vector>
#include <limits>
#include "Reporter.hpp"
#include "TestHelpers.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"

#include "CppUTest/TestHarness.h"

namespace
{
    std::string getRandomIntLiteral()
    {
        return std::to_string(
            TEST::getRandom<uint8_t>(0, std::numeric_limits<uint8_t>::max())
            );
    }

    std::string getRandomCharLiteral()
    {
        bool which = TEST::getRandom<bool>(0, 1);

        if(which)
        {
            // We check a subset of the ascii to ensure we don't accidentally
            // generate one that should be preceeded by a '\'
            char c = TEST::getRandom<unsigned>(64, 126);
            std::string r_val = "'"; r_val += c; r_val += "'";
            return r_val;
        }
        else
        {
            std::vector<std::string> options = {
                "'\\t'",
                "'\\n'",    // We add multiple slashes to ensure that it is as
                "'\\r'",    // what would appear in the file '\n'
                "'\\0'",
                "'\\''",
                "'0'", "'1'", "'2'", "'3'", "'4'", "'5'","'6'","'7'","'8'","'9'",
                "'^'", "'.'"
            };

            int selection = TEST::getRandom<int>(0, options.size()-1);

            return options[selection];
        }
    }

    std::string getRandomDoubleLiteral()
    {
        std::vector<std::string> options = {
            "3.14159",
            "0.000",
            "9128.2342344",
            "00033.3333"
        };

        int selection = TEST::getRandom<int>(0, options.size()-1);

        return options[selection];
    }

    std::string getRandomStringLiteral()
    {
        // These have to look this way to ensure that they represent what we would see in a file
        //
        //  i.e    " This is \" a dumb \" string "
        //
        std::vector<std::string> options = {
            "\"This is a string\"",
            "\"This is \\\"also\\\" a string\"",
            "\"This is a dumb \\\" kjnkjn\\\" string\"",
            "\"\"",
            "\"\\\"\"",
            "\"H\""
        };

        int selection = TEST::getRandom<int>(0, options.size()-1);

        return options[selection];
    }

    std::string getRandomIdentifier()
    {
        std::vector<std::string> options = {
            "function_name",
            "fasm_bad_func_name234234",
            "x_q_c_func_name",
            "areallyreallylongandbadidentifierbutnonethelesstotallyvalid"
        };

        int selection = TEST::getRandom<int>(0, options.size()-1);

        return options[selection];
    }

    std::vector<TAU::Token> getTauTokens()
    {
        return {
                { TAU::Token::Type::FUNC,       1, 1, "func "},     // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::ASM,        1, 1, "asm "},      // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::RETURN,     1, 1, "return "},   // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::INT,        1, 1, "int "},      // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::DOUBLE,     1, 1, "double "},   // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::CHAR,       1, 1, "char "},     // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::INT_L,      1, 1, getRandomIntLiteral()},    
                { TAU::Token::Type::DOUBLE_L,   1, 1, getRandomDoubleLiteral()},       
                { TAU::Token::Type::CHAR_L,     1, 1, getRandomCharLiteral()},  
                { TAU::Token::Type::IDENTIFIER, 1, 1, getRandomIdentifier()},     
                { TAU::Token::Type::STRING_L,   1, 1, getRandomStringLiteral()},
                { TAU::Token::Type::SEMI,       1, 1, ";"},    
                { TAU::Token::Type::COMMA,      1, 1, ","},       
                { TAU::Token::Type::COLON,      1, 1, ":"},
                { TAU::Token::Type::PLUS,       1, 1, "+"},    
                { TAU::Token::Type::MINUS,      1, 1, "-"},       
                { TAU::Token::Type::SLASH,      1, 1, "/"},
                { TAU::Token::Type::AMP,        1, 1, "&"},    
                { TAU::Token::Type::D_AMP,      1, 1, "&&"},       
                { TAU::Token::Type::PIPE,       1, 1, "|"},
                { TAU::Token::Type::HAT,        1, 1, "^"},    
                { TAU::Token::Type::SHL,        1, 1, "<<"},       
                { TAU::Token::Type::SHR,        1, 1, ">>"},
                { TAU::Token::Type::LT,         1, 1, "<"},    
                { TAU::Token::Type::LTE,        1, 1, "<="},       
                { TAU::Token::Type::GT,         1, 1, ">"},
                { TAU::Token::Type::EQ,         1, 1, "="},    
                { TAU::Token::Type::BANG_EQ,    1, 1, "!="},       
                { TAU::Token::Type::BANG,       1, 1, "!"},
                { TAU::Token::Type::D_STAR,     1, 1, "**"},    
                { TAU::Token::Type::PCT,        1, 1, "%"},       
                { TAU::Token::Type::DOT,        1, 1, "."},
                { TAU::Token::Type::L_PAREN,    1, 1, "("},    
                { TAU::Token::Type::R_PAREN,    1, 1, ")"},       
                { TAU::Token::Type::STAR,       1, 1, "*"},
                { TAU::Token::Type::L_BRACE,    1, 1, "{"},    
                { TAU::Token::Type::R_BRACE,    1, 1, "}"},       
                { TAU::Token::Type::D_PIPE,     1, 1, "||"},
                { TAU::Token::Type::L_BRACKET,  1, 1, "["},    
                { TAU::Token::Type::R_BRACKET,  1, 1, "]"},       
                { TAU::Token::Type::TILDE,      1, 1, "~"},
                { TAU::Token::Type::ARROW,      1, 1, "->"},    
                { TAU::Token::Type::STRUCT,     1, 1, "struct "},       // Add space at end so it isn't mistaken as identifier, and doesn't polute with more tokens
                { TAU::Token::Type::GTE,        1, 1, ">="},
                { TAU::Token::Type::ASSIGN,     1, 1, ":="}
        };
    }
}

// ---------------------------------------------------------------
// 
// ---------------------------------------------------------------

TEST_GROUP(TokenStreamTests)
{
};

// ---------------------------------------------------------------
//  Check that the parser is matching every known 
//  individual token - This only tests valids. We need error handler
//  to ensure that it throws errors on invalid tokens
// ---------------------------------------------------------------

TEST(TokenStreamTests, IndividualTokens)
{
    for(int i = 0; i < 100; i++)
    {
        std::vector<TAU::Token> pairs = getTauTokens(); 

        for(auto item : pairs)
        {
            std::vector<std::string> source; 
            source.push_back(item.value);

            TAU::Reporter * reporter = new TAU::Reporter("TEST::TAU::TokenStream");

            TAU::TokenStream ts(*reporter); 
            ts.createStreamFromVector("IndividualTokenTest", source);

            std::vector<TAU::Token> t_stream = ts.getStream();

            if(t_stream.size() == 0)
            {
                std::cout << "Expected: " << item.toString() << " but got nothing" << std::endl;
                FAIL("TokenStream did not return a token");
            }

            //std::cout << item.toString() << " | " << t_stream[0].toString() << std::endl;

            CHECK_TRUE_TEXT(item.type == t_stream[0].type, "Types did not match");

            delete reporter;
        }
    }
}
