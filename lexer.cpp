// code to input input.c and perfrom lexical analysis and output the result to output stream
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

class lexer {
    std::vector<std::string> tokens;   // holds broken down tokens
    std::fstream* fileptr;
    std::string delim, keyword, num, preprodir, op, literal; // declarations for regex strings

public:
    lexer(std::fstream* file);
    void breaker(std::fstream* file);
    void analyse();
};

lexer::lexer(std::fstream* file) {      //inintialise strings definitions for regex
    delim = "[\\,\\;\\(\\)\\[\\]\\{\\}]";   // delimiters
    keyword = "(main|while|do|for|if|else|switch|int|float|char|long|short|void|printf|scanf)";     // keywords
    op = "(=|\\+|-|\\*|/|%|==|!=|<|<=|>|>=|&&|\\|\\||!|\\^|&|\\||~|<<|>>|\\+=|-=|\\*=|/=|%=|&=|\\^=|\\|=|<<=|>>=|\\+\\+|--)";   // operators
    num = "[-+]?[0-9]*\\.?[0-9]*";  // numbers including usigned integers and floating point number
    literal = "\".*\"|'.'";     // string or character literals
    preprodir = "#.*\\ ?.*";    // preprocessor directives
    fileptr = file;     // file pointer to hold
}

void lexer::analyse() {
    for (auto i : tokens) {     // iterate over tokens and perform analysis
        if (std::regex_match(i, std::regex(keyword))) {
            std::cout << "<keyword, " << i << ">\n";
        }
        else if (std::regex_match(i, std::regex(op))) {
            std::cout << "<operator, " << i << ">\n";
        }
        else if (std::regex_match(i, std::regex(delim))) {
            std::cout << "<delim, " << i << ">\n";
        }
        else if (std::regex_match(i, std::regex(num))) {
            std::cout << "<num, " << i << ">\n";
        }
        else if (std::regex_match(i, std::regex(literal))) {
            std::cout << "<literal, " << i << ">\n";
        }
        else if (std::regex_match(i, std::regex(preprodir))) {
            std::cout << "<preprocessor directive, " << i << ">\n";
        }
        else {
            std::cout << "<id, " << i << ">\n";
        }
    }
}

void lexer::breaker(std::fstream* file) {
    std::string ib = "", s = "";     // input buffer and scanned character
    char c;     // character being scanned - same as s
    while (file->get(c)) {
        s = c;  // to handle type inconsistency in regex functions
        if (std::regex_match(s, std::regex(delim))) {   // is scanned character a deilimter?
            if (!ib.empty()) {      // if buffer is not empty then push it first
                tokens.push_back(ib);   // flush buffer contents as one token
                ib = "";
                tokens.push_back(s);    // push delimiter
            }
            else
                tokens.push_back(s);
        }
        else if (s == "\"") {   // is a string literal being scanned?
            ib += "\"";
            while (file->get(c)) {  // push all charcters till next "
                ib += c;
                if (c == '\"') {
                    tokens.push_back(ib);
                    ib = "";
                    break;
                }
            }
        }
        else if (c == ' ' || c == '\n') {     // dealing with whitespaces and newline charcters
            if (c == ' ' && std::regex_match(ib, std::regex("#.*"))) {  // if preprocessor directive contains space
                ib += c;
            }
            else if (!std::regex_match(ib, std::regex("(\\ )*"))) {     // when input buffer is not a sequence of whitespaces - end case
                tokens.push_back(ib);
                ib = "";
            }
        }
        else
            ib += s;    // advance buffer
    }
}

int main() {
    std::fstream file("../input.c");    // read file in parent directory
    lexer analyser(&file);
    analyser.breaker(&file);            // function to tokenise the source code
    analyser.analyse();                 // function to analyse the tokens
    return 0;
}