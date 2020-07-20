#include "TokenStream.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace TAU
{
    TokenStream::TokenStream()
    {
        keywords["func"] = Token::Type::FUNC;
        keywords[";"]    = Token::Type::SEMI;
        keywords["("]    = Token::Type::LEFT_PAREN;
        keywords[")"]    = Token::Type::RIGHT_PAREN;
    }

    void TokenStream::createStreamFromFile(std::string file)
    {
        auto file_path = std::filesystem::path(file);

        if(!std::filesystem::is_regular_file(file_path))
        {
            std::cerr << "Error: given item \"" << file << "\" is not a file! " << std::endl;
            exit(EXIT_FAILURE);
        }

        std::ifstream in(file);

        if(!in.is_open())
        {
            std::cerr << "Error: Unable to open file " << file << std::endl;
            exit(EXIT_FAILURE);
        }

        std::vector<std::string> source_file;

        std::string line;
        while(std::getline(in, line))
        {
            source_file.push_back(line);
        }

        in.close();
    }
}