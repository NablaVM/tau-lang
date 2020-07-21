#ifndef TAU_DRIVER_HPP
#define TAU_DRIVER_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace TAU
{
    //  Forward declare the parser
    //
    class Parser;

    //! \brief The Tau driver - Used to run the compilation of source files
    //!
    class Driver
    {
    public:

        //! \brief Driver constructor
        Driver();

        //! \brief Deconstruct driver
        ~Driver();

        //! \brief Start driving the compilation
        //! \param compilation_files The compilation files for given project
        void start(std::vector<std::string> compilation_files);

        //! \brief Request that the dirver search for and import a file
        //! \param filename The filename to search for
        void import(std::string filename);

    private:

        Parser * parser;

        std::vector< std::filesystem::path > project_directories;
    };
}

#endif