/*
Project: OGLA
File: ogla.hpp
Author: Leonardo Banderali
Created: March 07, 2015
Last Modified: March 07, 2015

Description:
    OGLA is generic lexical analyzer intended for quick and fast integration into
    any type of project requiring a lexer.  The main goal of this library is to
    provided an easy to use generic lexer for use in proof-of-concepts and other
    small projects.  It is not intended for use in time sensitive or highly
    optimized applications.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_HPP
#define OGLA_HPP

//include standard c++ libraries
#include <string>
#include <vector>
#include <regex>
#include <iostream>

namespace ogla {

// forward declarations and function prototypes
class Rule;     // type for describing a rule used to identify a token
using RuleList = std::vector<Rule>;
class Token;    // type representing a token in analyzed text
using TokenList = std::vector<Token>;
class Grammar;  // type representing a lexcical analyses grammar

template<class BidirectionalIterator>
Token firstToken(BidirectionalIterator first, BidirectionalIterator last, const RuleList& rules);
Token firstToken(const std::string& text, const RuleList& rules);
/*  - returns the first token identified using `rules`
    - `first` is an iterator (better if const_iterator) pointing to the first character of the text to be analyzed
    - `last` is an iterator (better if const_iterator) pointing to one character past the end of the text to be analyzed
    - `text` is the text to be analyzed
*/

TokenList analyze(const std::string& text, const Grammar& grammar);
/*  returns a list of tokens representing `text` tokenized using `grammar` */

/*
A class for describing a rule used to identify a token (tokenization rule).
*/
class Rule {
    public:
        Rule() {}
        Rule(const std::string& _name, const std::string& _rgx) : name{_name}, rgx{_rgx} {}
        /*  constructs a rule with the name `_name` and uses `_rgx` as regular expression for matching */

        std::string name;
        std::regex rgx;     // holds the regular expression (regex) used to indentify the token
        RuleList* subrules; // holds rules to identify sub tokens
        std::regex endrgx;  // for tokens special tokens that use one regex to identify the start and another one
                            //   to identify the end
};

/*
A class for representing a token in analyzed text.
*/
class Token {
    // friends
    friend Token firstToken(const std::string& text, const RuleList& rules);
    template<class BidirectionalIterator>
    friend Token firstToken(BidirectionalIterator first, BidirectionalIterator last, const RuleList& rules);

    public:
        Token() {}

        std::string name() {
            return rule.name;
        }

        int position() const {
            if (match.empty())
                return -1;
            else
                return match.position() + offset;
        }

        int length() const {
            if (match.empty())
                return -1;
            else
                return match.length();
        }

        std::string lexeme() const {
            if (match.empty())
                return std::string();
            else
                return match.str();
        }

        void set_offset(int n) {
            offset = n;
        }

    private:
        Rule rule;          // holds the rule used to match the token
        std::smatch match;  // holds the lexem matched associated with the token
        int offset;         // holds the offset for the token position

        Token(Rule r, std::smatch m) : rule(r), match(m), offset(0) {}
        /*  private constructor that creates a token */
};

/*
A class representing a lexcical analyses grammar.
*/
class Grammar {
    // friends
    friend TokenList analyze(const std::string& text, const Grammar& grammar);

    public:
        // static functions

        static Grammar load();
        /*  loads the default grammar used to load other grammars */

        static Grammar load(std::istream& input);
        /*  loads the language grammar defined by the content in the `input` stream */

        static Grammar load(const std::string& filePath);
        /*  loads the language grammar defined in the file with path `filePath` */

        // getters and setters

        std::string name() {
            return langName;
        }

    protected:

    private:
        //std::string grammerFilePath;// holds the path to the grammar file
        std::string langName;       // stores the name of the current language grammer
        RuleList rules;             // holds all tokenization rules
};

/*
- returns the first token identified using `rules`
- `first` is an iterator (better if const_iterator) pointing to the first character of the text to be analyzed
- `last` is an iterator (better if const_iterator) pointing to one character past the end of the text to be analyzed
- `text` is the text to be analyzed
*/
template<class BidirectionalIterator>
Token firstToken(BidirectionalIterator first, BidirectionalIterator last, const RuleList& rules) {
    Token t;

    for (auto r: rules) {
        std::smatch m;
        if (std::regex_search(first, last, m, r.rgx) && (t.position() > m.position() || t.position() < 0))
            t = Token(r, m);
    }

    return t;
}

}

#endif  //OGLA_HPP
