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
          //  std::cout << "TokenStream <line> : " << line << std::endl;

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
        std::string token_string;
        for(unsigned row = 0; row < file_contents.size(); row++)
        {
            std::string &line = file_contents[row];

            bool inside_string_literal = false;
            bool inside_char_literal   = false;
            for(unsigned col = 0; col < line.size(); col++)
            {
                // Skip leading white spaces on the line, This steps over by one so we dec at the end
                //
                if(col == 0) { while(col < line.size() && std::isspace(line[++col])){} col--;}

                //  If a '"' is found without a '\' before it we need to toggle us being in a string literal
                //
                if(line[col] == '"')
                {
                    if(col == 0) { inside_string_literal = true; }
                    else if (col > 0 && line[col-1] != '\\') { inside_string_literal = false; }
                }
                
                //  If we're in a string literal add everything to it and ignore other operations
                //
                if (inside_string_literal)
                {
                    token_string += line[col];
                    continue;
                }

                //  Same thing for char literals as string literals
                //
                if(line[col] == '\'')
                {
                    if(col == 0) { inside_char_literal = true; }
                    else if (col > 0 && line[col-1] != '\\') { inside_char_literal = false; }
                }

                // Add whatever to the char literal - Validitiy of said literal will be checked after its built
                // 
                if(inside_char_literal)
                {
                    token_string += line[col];
                    continue;
                }

                //  Skip spaces that aren't in a string
                //
                if(!inside_string_literal && std::isspace(line[col]))
                { 
                    //  If there was a space and we had something going on, we need to classify it
                    //
                    if(token_string.size() > 0) { classifyToken(token_string, row, col); token_string.clear(); }
                    continue;
                }

                //  Grab the current item from the line @ col
                //
                std::string current_item; current_item += line[col];

                //  Edge case to handle the '.' Its potentially identifies an accessor, or a double 
                //
                if(line[col] == '.')
                {
                    bool take_dot = true;
                    if(col+1 < line.size())
                    {
                        if(isdigit(line[col+1]))
                        {
                            take_dot = false;
                            token_string += current_item;
                        }
                    }

                    if(take_dot)
                    {
                        t_stream.push_back({keywords["."], (int)col, (int)row, "."});
                        continue;
                    }
                }

                //  The current item was in the keywords its keys off a potential 2-size token
                //
                else if(keywords.find(current_item) != keywords.end() && !inside_string_literal)
                {
                    //  Right look ahead to see if we need to add any extras
                    //
                    if(col+1 < line.size())
                    {
                        switch(line[col])
                        {
                        case '*': if(line[col+1] == '*'){ current_item = "**"; col++; } break;
                        case '&': if(line[col+1] == '&'){ current_item = "&&"; col++; } break;
                        case '|': if(line[col+1] == '|'){ current_item = "||"; col++; } break;
                        case '<': if(line[col+1] == '<'){ current_item = "<<"; col++; } 
                                  if(line[col+1] == '='){ current_item = "<="; col++; } break;
                        case '>': if(line[col+1] == '>'){ current_item = ">>"; col++;   break;} 
                                  if(line[col+1] == '='){ current_item = ">="; col++; } break;
                        case '!': if(line[col+1] == '='){ current_item = "!="; col++; } break;
                        case '-': if(line[col+1] == '>'){ current_item = "->"; col++; } break;
                        case ':': if(line[col+1] == '='){ current_item = ":="; col++; } break;
                        case '/': if(line[col+1] == '/')
                                  { 
                                    col = line.size();
                                    token_string.clear();
                                    continue;
                                  } 
                        break;
                        default: break;
                        }
                    }

                    //  Before we add the token we just found we need to see if a token was being 
                    //  built. Here we need to figure out what kind of token it is
                    //
                    if(token_string.size() > 0)
                    {
                        //  Classify the token on line / col. We need to see if its a var or identifier
                        //
                        classifyToken(token_string, row, col);

                        // Reset token
                        token_string.clear();
                    }

                    //  Add token
                    //
                    t_stream.push_back({keywords[current_item], (int)col, (int)row, current_item});
                }
                else
                {
                    token_string += current_item;
                }


                // Check if the string contains a keyword
                //
                if(keywords.find(token_string) != keywords.end())
                {
                    if(col+1 < line.size())
                    {
                        if( std::isspace(line[col+1]) || (keywords.find(std::string(1, line[col+1])) != keywords.end()))
                        {
                            //std::cout << "Found keyword : " << token_string << std::endl;

                            t_stream.push_back({keywords[token_string], (int)col, (int)row, token_string});

                            token_string.clear();
                        }
                    }
                }
            }

            // Cross-line thingamabob 
            //
            if(token_string.size() != 0)
            {
                classifyToken(token_string, (int)row, line.size());
                token_string.clear();
            }
        }
    }

    // -----------------------------------------
    //
    // -----------------------------------------

    void TokenStream::classifyToken(std::string token_string, int line,  int col)
    {
        //std::cout << "Need to classify token string : " 
        //          << token_string << " | ";


        if(std::regex_match(token_string, reg_string_literal))
        {
            //std::cout << "STRING LITERAL" << std::endl;
            t_stream.push_back({Token::Type::STRING_L, col, line, token_string});
        }
        else if(std::regex_match(token_string, reg_integer_literal))
        {
            //std::cout << "INTEGER LITERAL" << std::endl;
            t_stream.push_back({Token::Type::INT_L, col, line, token_string});
        }
        else if(std::regex_match(token_string, reg_double_literal))
        {
            //std::cout << "DOUBLE LITERAL" << std::endl;
            t_stream.push_back({Token::Type::DOUBLE_L, col, line, token_string});
        }
        else if(std::regex_match(token_string, reg_char_literal))
        {
            //std::cout << "CHAR LITERAL" << std::endl;
            t_stream.push_back({Token::Type::CHAR_L, col, line, token_string});
        }
        else if(std::regex_match(token_string, reg_identifier))
        {
            //std::cout << "IDENTIFIER" << std::endl;
            t_stream.push_back({Token::Type::IDENTIFIER, col, line, token_string});
        }
        else
        {
            /*
                TODO: 
                Need to flag the error handler when it gets developed!
            */
            std::cout << "Error: Unknown token \"" << token_string << "\" at " << line << ":" << col << std::endl;
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
