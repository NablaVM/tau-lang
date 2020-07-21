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
        keywords[";"]    = Token::Type::SEMI;
        keywords["("]    = Token::Type::LEFT_PAREN;
        keywords[")"]    = Token::Type::RIGHT_PAREN;
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
    }

    // -----------------------------------------
    //
    // -----------------------------------------

    std::string TokenStream::getFileName() const
    {
        return origin_file;
    }
}