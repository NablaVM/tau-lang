#include "Driver.hpp"

#include "Parser.hpp"
#include "TokenStream.hpp"

#include <algorithm>
#include <iostream>

namespace TAU
{
    // -----------------------------------------
    //
    // -----------------------------------------

    Driver::Driver() 
    {
        parser = new Parser(*this);
    }

    // -----------------------------------------
    //
    // -----------------------------------------

    Driver::~Driver() 
    {
        delete parser;
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Driver::start(std::vector<std::string> compilation_files)
    {
        //  For every file given
        //
        for(auto item : compilation_files)
        {
            //  Create a path for it
            //
            std::filesystem::path current_path(item);

            //  Ensure that the given path is a regular file
            //
            if(!std::filesystem::is_regular_file(current_path))
            {
                std::cerr << "Error : Item given to driver \"" << item << "\" is not a file" << std::endl;
                exit(EXIT_FAILURE);
            }

            //  Remove the file name from the path to get the directory
            //
            std::filesystem::path current_dir = current_path.remove_filename();

            //  Check if that path has been stored in the project directories vector - 
            //      What we will use to scan for files if we need to; If it hasn't 
            //      ben added then push it into the vector
            //
            if(std::find(project_directories.begin(), project_directories.end(), current_dir) == project_directories.end())
            {
                project_directories.push_back(current_dir);
            }

            //  Create a token stream for the file
            //
            TokenStream * token_stream = new TokenStream();
            token_stream->createStreamFromFile(item);

            // Hand the token stream to the parser - it will handle clean up
            //
            parser->addTokenStream(token_stream);
        }

        //  The parser now has all of the initial streams, start parsing
        //
        parser->begin();
    }

    // -----------------------------------------
    //
    // -----------------------------------------
    
    void Driver::import(std::string filename)
    {
        std::cout << "DEV INFO : Driver asked to import a file - This functionality is not yet implemented" << std::endl;

        exit(EXIT_FAILURE);

        /*
            If the parser requests a file (or something maybe not a file) it will call this method
            and ask the driver to search the project_directories for the thing that it needs.

            If / When found a token stream will be made with the file name, added to the parser
            and then upon the return of this function the parser will attempt to continue
        */
    }
}