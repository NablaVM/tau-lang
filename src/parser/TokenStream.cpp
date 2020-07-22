#include "TokenStream.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace TAU
{
namespace
{
std::regex reg_string_literal("\"([^\"\\\\]|\\\\.)*\"");
std::regex reg_double_literal("^[0-9]+.[0-9]+$");
std::regex reg_integer_literal("(^[0-9]+$)|(^\\-[0-9]+$)");
std::regex reg_char_literal(R"('\\*.{1}')");
std::regex reg_identifier("(^[a-zA-Z_]+[0-9]*)");
}

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

    //  Create a vector to store the lines of the file
    //
    std::vector<std::string> source_file;

    // Read in the file
    //
    std::string line;
    while(std::getline(in, line))
    {
//        //  std::cout << "TokenStream <line> : " << line << std::endl;

        source_file.push_back(line);
    }

    //  Close file
    //
    in.close();

    createStreamFromVector(file, source_file);
}

// -----------------------------------------
//
// -----------------------------------------

void TokenStream::createStreamFromVector(std::string source_name, std::vector<std::string> & source_vector)
{
    //  Set the file name that the stream was generated from
    //
    origin_source_name = source_name;

    //  Tokenize the file
    //
    tokenize(source_vector);

}

// -----------------------------------------
//
// -----------------------------------------

void TokenStream::tokenize(std::vector<std::string> & file_contents)
{
//    std::cout << "TokenStream <file> : " << origin_source_name << std::endl;
//    std::cout << "TokenStream <todo> : Tokenize the file here - This is the next thing to do" << std::endl;

    unsigned row = 0;

    enum class State
    {
        NONE,
        WHITESPACE,
        SYMBOL,
        LITERAL,
        LABEL
    } state = State::NONE;


    std::set<char> sym_set = { 
        ';',',',':','.','+','-','/','*','%','&','|',
        '^','~','<','>','=','!','(',')','{','}','[',']'
    };

    std::set<char> lit_set = { 
        '\'', '\"', '0', '1', '2', '3', 
        '4' , '5' , '6', '7', '8', '9'
    };
    std::regex lit_num_reg (R"(\d+(\.\d+)?)");
    std::regex lit_char_reg(R"('\\*.')");
    std::regex lit_str_reg (R"("([^"\\]|\\.)*")");

    std::set<char> lab_set = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
        'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
        'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'
    };

    for(auto line : file_contents)
    {
        std::string token_string;

        for(unsigned col = 0; col < line.size(); )
        {
            switch(state)
            {
            case State::NONE:
//                std::cout << "TokenStream: State::NONE" << std::endl;
                // Starting a new token.
                token_string.clear();
                // Any character will suffice.
                if(std::isspace(line[col]))      { state = State::WHITESPACE; }           
                else if(sym_set.count(line[col])){ state = State::SYMBOL;     }
                else if(lit_set.count(line[col])){ state = State::LITERAL;    }
                else if(lab_set.count(line[col])){ state = State::LABEL;      }
                break;
            case State::WHITESPACE:
//                std::cout << "TokenStream: State::WHITESPACE" << std::endl;
                // Process whitespace characters.
                if(std::isspace(line[col]))
                { 
                    col++; 
                }
                else
                {
                    state = State::NONE;
                }
                break;
            case State::SYMBOL:
//                std::cout << "TokenStream: State::SYMBOL" << std::endl;
                // Consume symbol.
                // Check for comment.
                if(col+1 < line.size() && line[col] == '/' && line[col+1] == '/')
                {
//                    std::cout << "TokenStream: COMMENT LINE." << std::endl;
                    state = State::NONE;
                    col = line.size();
                }
                else
                {
                    if(sym_set.count(line[col]))
                    {
                        token_string += line[col];

                        if(col + 1 < line.size() && sym_set.count(line[col + 1]))
                        {
                            token_string += line[col + 1];
                            col++;
                        }

                        col++;
                        state = State::NONE;

                        if(keywords.find(token_string) == keywords.end())
                        {
                            token_string.pop_back();
                            col--;
                        }

//                        std::cout << "TokenStream: SYMBOL: " << token_string << std::endl;

                        try
                        {
                            Token::Type type = keywords.at(token_string);
                            Token tok = { type, col - token_string.size(), row, token_string };
                            t_stream.push_back(tok);
                        }
                        catch(std::out_of_range &e)
                        {
//                            std::cout << "TokenStream: Invalid symbol." << std::endl;
                        }
                    }
                }

                break;
            case State::LITERAL:
//                std::cout << "TokenStream: State::LITERAL" << std::endl;
                {
                    std::smatch match;
                    std::string linesub(line, col);
//                    std::cout << "TokenStream: linesub: " << linesub << std::endl;
                    if(std::isdigit(line[col]) && std::regex_search(linesub, match, lit_num_reg))
                    {
//                        std::cout << "TokenStream: Found numeric literal: " << match[0] << std::endl;

                        Token::Type type = Token::Type::INT_L;

                        if(match[0].str().find(".") != std::string::npos)
                        {
                            type = Token::Type::DOUBLE_L;
                        }

                        Token tk = { type, col, row, match[0].str() };
                        t_stream.push_back(tk);

                        col += match[0].length();
                    }
                    if(line[col] == '\'' && std::regex_search(linesub, match, lit_char_reg))
                    {
//                        std::cout << "TokenStream: Found char literal: " << match[0] << std::endl;
//                        std::cout << "len = " << match[0].length() << std::endl;

                        Token tk = { Token::Type::CHAR_L, col, row, match[0].str() };
                        t_stream.push_back(tk);

                        col += match[0].length();
                    }
                    if(line[col] == '\"' && std::regex_search(linesub, match, lit_str_reg))
                    {
//                        std::cout << "TokenStream: Found string literal: " << match[0] << std::endl;
//                        std::cout << "len = " << match[0].length() << std::endl;

                        Token tk = { Token::Type::STRING_L, col, row, match[0].str() };
                        t_stream.push_back(tk);

                        col += match[0].length();
                    }

                    state = State::NONE;
                }

                // Consume literal.
                break;
            case State::LABEL:
//                std::cout << "TokenStream: State::LABEL" << std::endl;

                if(lab_set.count(line[col]))
                {
                    token_string += line[col];
                    col++;
                }

                if(col >= line.size() || !lab_set.count(line[col]))
                {
//                    std::cout << "TokenStream: LABEL: " << token_string << std::endl;

                    Token::Type type = Token::Type::IDENTIFIER;

                    try
                    {
                        type = keywords.at(token_string);
                    }
                    catch(std::out_of_range &e)
                    {
//                        std::cout << "TokenStream: Label is not keyword. Assuming identifier." << std::endl;
                    }

                    Token tok = { type, col-token_string.size(), row, token_string };
                    t_stream.push_back(tok);

                    state = State::NONE;
                }
                // Consume label.
                break;
            }
        }

        state = State::NONE;
        row++;
    }
//    std::cout << "TokenStream: TOKENS:" << std::endl;
    for(auto t : t_stream)
    {
//        std::cout << t.line_number << ":" << t.start_pos << ": " << t.value << std::endl;
    }
}

// -----------------------------------------
//
// -----------------------------------------

void TokenStream::classifyToken(std::string token_string, int line,  int col)
{
//    //std::cout << "Need to classify token string : " 
    //          << token_string << " | ";


    if(std::regex_match(token_string, reg_string_literal))
    {
//        //std::cout << "STRING LITERAL" << std::endl;
        t_stream.push_back({Token::Type::STRING_L, col, line, token_string});
    }
    else if(std::regex_match(token_string, reg_integer_literal))
    {
//        //std::cout << "INTEGER LITERAL" << std::endl;
        t_stream.push_back({Token::Type::INT_L, col, line, token_string});
    }
    else if(std::regex_match(token_string, reg_double_literal))
    {
//        //std::cout << "DOUBLE LITERAL" << std::endl;
        t_stream.push_back({Token::Type::DOUBLE_L, col, line, token_string});
    }
    else if(std::regex_match(token_string, reg_char_literal))
    {
//        //std::cout << "CHAR LITERAL" << std::endl;
        t_stream.push_back({Token::Type::CHAR_L, col, line, token_string});
    }
    else if(std::regex_match(token_string, reg_identifier))
    {
//        //std::cout << "IDENTIFIER" << std::endl;
        t_stream.push_back({Token::Type::IDENTIFIER, col, line, token_string});
    }
    else
    {
        /*
TODO: 
Need to flag the error handler when it gets developed!
         */
//        std::cout << "Error: Unknown token \"" << token_string << "\" at " << line << ":" << col << std::endl;
        exit(EXIT_FAILURE);
    }   
}

// -----------------------------------------
//
// -----------------------------------------

std::string TokenStream::getFileName() const
{
    return origin_source_name;
}

// -----------------------------------------
//
// -----------------------------------------

std::vector<Token> TokenStream::getStream() const
{
    return t_stream;
}
}
