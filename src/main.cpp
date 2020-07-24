#include <iostream>
#include <vector>

#include "Driver.hpp"
#include "Parser.hpp"

namespace 
{
    TAU::Driver driver;
    TAU::Parser parser;
}


void build_grammar();

int main(int argc, char ** argv) 
{
    // Convert arguments to a vector of strings, removing the program name
    std::vector<std::string> args(argv + 1, argv + argc);

    // Right now we are only accepting files as arguments; eventually we need to actually handle flags
    driver.loadFiles(parser, args);

    // Parse the token streams populated by the driver
    parser.parse();

    return 0;
}