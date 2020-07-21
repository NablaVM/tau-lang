#include <iostream>
#include <vector>

#include "Driver.hpp"

int main(int argc, char ** argv) 
{
    // Convert arguments to a vector of strings, removing the program name
    std::vector<std::string> args(argv + 1, argv + argc);

    TAU::Driver compilation_driver;

    // Right now we are only accepting files as arguments; eventually we need to actually handle flags
    compilation_driver.start(args);

    return 0;
}