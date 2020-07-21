#include "TokenStream.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace TAU
{
    // -----------------------------------------
    //
    // -----------------------------------------
    
    TokenStream::TokenStream()
    {
        keywords["func"] = Token::Type::FUNC;
        keywords["asm"]  = Token::Type::ASM;
        keywords["return"] = Token::Type::RETURN;

        keywords["int"]    = Token::Type::INT;
        keywords["double"] = Token::Type::DOUBLE;
        keywords["char"]   = Token::Type::CHAR;
        keywords["struct"] = Token::Type::STRUCT;

        // Literal tokens would be here
        // Abstract tokens would be here

        keywords[";"]    = Token::Type::SEMI; 
        keywords[","]    = Token::Type::COMMA;
        keywords[":"]    = Token::Type::COLON;
        keywords["."]    = Token::Type::DOT;
        keywords["+"]    = Token::Type::PLUS;
        keywords["-"]    = Token::Type::MINUS;
        keywords["/"]    = Token::Type::SLASH;
        keywords["*"]    = Token::Type::STAR;
        keywords["**"]   = Token::Type::D_STAR;
        keywords["%"]    = Token::Type::PCT;
        keywords["&"]    = Token::Type::AMP;
        keywords["&&"]   = Token::Type::D_AMP;
        keywords["|"]    = Token::Type::PIPE;
        keywords["||"]   = Token::Type::D_PIPE;
        keywords["^"]    = Token::Type::HAT;
        keywords["<<"]   = Token::Type::SHL;
        keywords[">>"]   = Token::Type::SHR;
        keywords["~"]    = Token::Type::TILDE;
        keywords["<"]    = Token::Type::LT;
        keywords["<="]   = Token::Type::LTE;
        keywords[">"]    = Token::Type::GT;
        keywords[">="]   = Token::Type::GTE;
        keywords["="]    = Token::Type::EQ;
        keywords["!"]    = Token::Type::BANG;
        keywords["!="]   = Token::Type::BANG_EQ;
        keywords[":="]   = Token::Type::ASSIGN;
        keywords["("]    = Token::Type::L_PAREN;
        keywords[")"]    = Token::Type::R_PAREN;
        keywords["{"]    = Token::Type::L_BRACE;
        keywords["}"]    = Token::Type::R_BRACE;
        keywords["["]    = Token::Type::L_BRACKET;
        keywords["]"]    = Token::Type::R_BRACKET;
        keywords["->"]   = Token::Type::ARROW;

        break_points = {
            ';',',',':','.','+','-','/','*','%','&','|',
            '^','~','<','>','=','!',':','(',')','{','}','[',']',
            ' ', '\t', '\n', '\r'
        };
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void TokenStream::createStreamFromFile(std::string file)
    {
        // Create a path for the file
        //
        auto file_path = std::filesystem::path(file);

        // Ensure that the file exists as an actual file
        //
        if(!std::filesystem::is_regular_file(file_path))
        {
            std::cerr << "Error: given item \"" << file << "\" is not a file! " << std::endl;
            exit(EXIT_FAILURE);
        }

        //  Open the file
        //
        std::ifstream in(file);
        if(!in.is_open())
        {
            std::cerr << "Error: Unable to open file " << file << std::endl;
            exit(EXIT_FAILURE);
        }

        //  Set the file name that the stream was generated from
        //
        origin_file = file;

        //  Create a vector to store the lines of the file
        //
        std::vector<std::string> source_file;

        // Read in the file
        //
        std::string line;
        while(std::getline(in, line))
        {
            std::cout << "TokenStream <line> : " << line << std::endl;

            source_file.push_back(line);
        }

        //  Close file
        //
        in.close();

        //  Tokenize the file
        //
        tokenize(source_file);
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void TokenStream::tokenize(std::vector<std::string> file_contents)
    {
        std::cout << "TokenStream <file> : " << origin_file << std::endl;
        std::cout << "TokenStream <todo> : Tokenize the file here - This is the next thing to do" << std::endl;
    
        for(unsigned line_number = 0; line_number < file_contents.size(); line_number++)
        {
            // string the first char we have
            // Check if in map (would match + - / etc)
            // If not in the map
            //    if 'f' -> scan for "func"
            //    if 'a' -> scan for "asm"
            //    if '

            for(unsigned column_number = 0; column_number < file_contents[line_number].size(); column_number++)
            {

            }
        }
    
    
    
    
    }

    // -----------------------------------------
    //
    // -----------------------------------------

    std::string TokenStream::getFileName() const
    {
        return origin_file;
    }
}