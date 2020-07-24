#include "Reporter.hpp"
#include <iostream>
#include <libnabla/termcolor.hpp>

namespace TAU
{

// -----------------------------------------
//
// -----------------------------------------

Reporter::Reporter(std::string reporting_object) : reporting_object(reporting_object)
{

}

// -----------------------------------------
//
// -----------------------------------------

void Reporter::issueReport(Report report)
{
    //  Push the report to the stack so we can have it for debugging / testing 
    //
    reports_generated.push(report);

    //  Create the label that will tell the user if its an error or warning
    //
    std::string level_display = (report.level == Level::WARNING) ? "[WARNING]" : "[ERROR]";

    //  Make the color for the output
    //
    auto color_start = (report.level == Level::WARNING) ? termcolor::yellow : termcolor::red;

    //  Display the first part of the report
    //
    std::cout << color_start 
              << level_display
              << termcolor::reset
              << " "
              << report.source_file
              << ":"
              << report.line + 1
              << ":"
              << report.col + 1
              << " "
              << report.info
              << std::endl
              << report.source_line
              << std::endl;

    //  Draw the indicator object if there is one selected
    //
    switch(report.indicator)
    {
        case Indicator::ARROW_LINE: displayIndicatorArrowLine(report.level, report.col); break;
        case Indicator::ARROW     : displayIndicatorArrow (report.level, report.col); break;
        case Indicator::LINE      : displayIndicatorLine (report.level, report.col, report.source_line.size());
        case Indicator::NONE :
        default:
            break;
    }

    //  If the report level is of type ERROR we need to exit - Tell the user
    //  explicitly that we are exiting
    //
    if(report.level == Level::ERROR)
    {
        std::cout << termcolor::cyan << "Exiting" << termcolor::reset << std::endl;
        exit(EXIT_FAILURE);
    }
}

// -----------------------------------------
//
// -----------------------------------------

void Reporter::issueInternalError(std::string what)
{
    std::cerr << termcolor::red   << "[INTERNAL ERROR]"
              << termcolor::reset << ":"
              << termcolor::magenta  << reporting_object
              << termcolor::reset << " | "
              << what << std::endl;

    std::cerr << termcolor::cyan << "Exiting" << termcolor::reset << std::endl;
    exit(EXIT_FAILURE);
}

// -----------------------------------------
//
// -----------------------------------------

void Reporter::displayIndicatorArrowLine(Level level, std::size_t arrow_pos, unsigned trail_length)
{
    std::string line;
    std::size_t s = 0;
    while(s != arrow_pos)
    {
        if(arrow_pos > trail_length && (arrow_pos - s) <= trail_length)
        {
            line += "~";
        }
        else
        {
            line += " ";
        }
        s++;
    }

    line += "^";

    s = 0;
    while(s < trail_length){ line += "~"; s++; }

    auto color_start = (level == Level::WARNING) ? termcolor::yellow : termcolor::red;

    std::cout << color_start << line << termcolor::reset << std::endl;
}

// -----------------------------------------
//
// -----------------------------------------

void Reporter::displayIndicatorArrow(Level level, std::size_t arrow_pos)
{
    std::string line;
    std::size_t s = 0;
    while(s != arrow_pos)
    {
        line += " ";
        s++;
    }

    line += "^";

    auto color_start = (level == Level::WARNING) ? termcolor::yellow : termcolor::red;

    std::cout << color_start << line << termcolor::reset << std::endl;
}

// -----------------------------------------
//
// -----------------------------------------

void Reporter::displayIndicatorLine(Level level, std::size_t start_pos, std::size_t end_pos)
{
    if(end_pos < start_pos){ return; }

    std::size_t s = 0;
    std::string line;
    while(s < start_pos){ line += " "; s++; }
    while(s != end_pos){ line += "~";  s++; }

    auto color_start = (level == Level::WARNING) ? termcolor::yellow : termcolor::red;

    std::cout << color_start << line << termcolor::reset << std::endl;
}


}