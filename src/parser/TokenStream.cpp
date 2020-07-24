#include "TokenStream.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace TAU
{

// -----------------------------------------
//
// -----------------------------------------

TokenStream::TokenStream(Reporter &reporter) : reporter(reporter)
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
    unsigned row = 0;

    // States for what we might encounter in the source.
    enum class State
    {
        CLASSIFY,
        WHITESPACE,
        SYMBOL,
        LITERAL,
        LABEL
    } state = State::CLASSIFY;


    // All valid characters which may begin a symbol.
    std::set<char> sym_set = { 
        ';',',',':','.','+','-','/','*','%','&','|',
        '^','~','<','>','=','!','(',')','{','}','[',']'
    };

    // All valid characters which may begin a literal.
    std::set<char> lit_set = { 
        '\'', '\"', '0', '1', '2', '3', 
        '4' , '5' , '6', '7', '8', '9'
    };

    // Regexes to match literals.
    std::regex lit_num_reg (R"(\d+(\.\d+)?)");
    std::regex lit_char_reg(R"('\\*.')");
    std::regex lit_str_reg (R"("([^"\\]|\\.)*")");

    // All valid characters which may begin a label -- a keyword or identifier.
    std::set<char> lab_set = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
        'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 
        'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'
    };
    
    // Regex to match a valid label.
    std::regex label_reg(R"([a-zA-Z_]\w*)");

    for(auto line : file_contents)
    {
        std::string token_string;

        for(std::size_t col = 0; col < line.size(); )
        {
            switch(state)
            {
            // Classify which type of token we're looking at.
            case State::CLASSIFY:

                // Starting a new token.
                token_string.clear();
                
                // Classify which type of token it is...
                if(std::isspace(line[col]))      { state = State::WHITESPACE; }           
                else if(sym_set.count(line[col])){ state = State::SYMBOL;     }
                else if(lit_set.count(line[col])){ state = State::LITERAL;    }
                else if(lab_set.count(line[col])){ state = State::LABEL;      }
                // ...or fail because the current character is not a valid one.
                else                             
                { 
                    reporter.issueReport({
                        Reporter::Level::ERROR,
                        Reporter::Indicator::ARROW,
                        row,
                        col,
                        origin_source_name,
                        line,
                        "Invalid character"
                    });     
                }
                break;

            // Consume whitespace characters.
            case State::WHITESPACE:

                if(std::isspace(line[col]))
                { 
                    col++; 
                }
                else
                {
                    // Done with whitespace. Back to classify.
                    state = State::CLASSIFY;
                }
                break;

            // Process a symbol token.
            case State::SYMBOL:

                // Check if the current symbol is a comment delimiter and bail if it is.
                if(col+1 < line.size() && line[col] == '/' && line[col+1] == '/')
                {
                    state = State::CLASSIFY;
                    col = line.size();
                }
                else
                {
                    // Check if current character is a valid symbol character.
                    if(sym_set.count(line[col]))
                    {
                        token_string += line[col];

                        // Need to grab the next character if it's also a symbol character
                        // in case it's a two-character symbol token.
                        if(col + 1 < line.size() && sym_set.count(line[col + 1]))
                        {
                            token_string += line[col + 1];
                            col++;
                        }

                        // Increment from the original character grab.
                        col++;

                        // If our two-character symbol isn't in the keywords map, it's
                        // not a valid symbol, so back up one to see if we have a 
                        // one-character symbol on our hands.
                        if(keywords.find(token_string) == keywords.end())
                        {
                            token_string.pop_back();
                            col--;
                        }

                        // Now try to get the token type from our map.
                        try
                        {
                            Token::Type type = keywords.at(token_string);
                            Token tok = { type, col - token_string.size(), row, token_string };
                            stream.push_back(tok);
                        }
                        catch(std::out_of_range &e)
                        {
                            reporter.issueReport({
                                Reporter::Level::ERROR,
                                Reporter::Indicator::LINE,
                                row,
                                col,
                                origin_source_name,
                                line,
                                "Unrecognized symbol : " + token_string
                            });
                        }

                        state = State::CLASSIFY;
                    }
                }

                break;

            // Process a numeric, character, or string literal token.
            case State::LITERAL:
                {
                    // Set up to do regexes. Substring the line so we start our search
                    // at the current column instead of the beginning of the line.
                    std::smatch match;
                    std::string linesub(line, col);

                    // Match if a numeric string literal is detected.
                    if(std::isdigit(line[col]))
                    {
                        if(std::regex_search(linesub, match, lit_num_reg))
                        {
                            // Default to int literal because...
                            Token::Type type = Token::Type::INT_L;

                            // ...if we find a dot we'll change it to a double.
                            if(match[0].str().find(".") != std::string::npos)
                            {
                                type = Token::Type::DOUBLE_L;
                            }

                            Token tk = { type, col, row, match[0].str() };
                            stream.push_back(tk);

                            col += match[0].length();
                        }
                    }

                    // If the first character is a ' then we're looking at a potential character literal.
                    else if(line[col] == '\'')
                    {
                        // If regex matches, then we have a valid character literal.
                        if(std::regex_search(linesub, match, lit_char_reg))
                        {
                            Token tk = { Token::Type::CHAR_L, col, row, match[0].str() };
                            stream.push_back(tk);
                            col += match[0].length();
                        }
                        // Otherwise there is something wrong with the literal.
                        else
                        {
                            reporter.issueReport({
                                Reporter::Level::ERROR,
                                Reporter::Indicator::LINE,
                                row,
                                col,
                                origin_source_name,
                                line,
                                "Malformed character literal"
                            });
                        }
                    }

                    // If the first character is a " then we're looking at a potential string literal.
                    else if(line[col] == '\"')
                    {
                        // If regex matches, then we have a valid string literal.
                        if(std::regex_search(linesub, match, lit_str_reg))
                        {
                            Token tk = { Token::Type::STRING_L, col, row, match[0].str() };
                            stream.push_back(tk);
                            col += match[0].length();
                        }

                        // Otherwise there is something wrong with the literal.
                        else
                        {
                            reporter.issueReport({
                                Reporter::Level::ERROR,
                                Reporter::Indicator::LINE,
                                row,
                                col,
                                origin_source_name,
                                line,
                                "Unterminated string literal"
                            });
                        }
                    }

                    state = State::CLASSIFY;
                }

                break;

            // Process a label -- either a keyword or an identifier.
            case State::LABEL:
                {
                    std::smatch match;
                    std::string linesub(line, col);

                    // The fact that we got to the LABEL case at all means we have
                    // a valid label to regex against, so this can never fail. 
                    // If it does, something is horribly horribly wrong and we damn
                    // well better bail.
                    if(std::regex_search(linesub, match, label_reg))
                    {
                        col += match[0].length();
                        // Default to IDENTIFIER
                        Token::Type type = Token::Type::IDENTIFIER;

                        // If our match is in the keywords map, set it to that instead
                        // of IDENTIFIER.
                        try
                        {
                            type = keywords.at(match[0].str());
                        }
                        catch(std::out_of_range &e)
                        {
                            // This is not an error. It just means that our token is
                            // not one of our keywords. Nothing needs to be acted
                            // upon here.
                        }

                        Token tk = { type, col-token_string.size(), row, match[0].str() };
                        stream.push_back(tk);
                    }
                    else
                    {
                        reporter.issueReport({
                            Reporter::Level::ERROR,
                            Reporter::Indicator::LINE,
                            row,
                            col,
                            origin_source_name,
                            line,
                            "Unrecognized character sequence when matching label"
                        });
                    }

                    state = State::CLASSIFY;
                }
                break;
            }
        }

        state = State::CLASSIFY;
        row++;
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
    return stream;
}
}
