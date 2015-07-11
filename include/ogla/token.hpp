/*
Project: OGLA
File: token.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: July 10, 2015

Description:
    A `Token` is a unit of analyzed text and is identified using a `Rule`.  These form the basic building blocks of the
    OGLA lexcial analyzer.  Tokens should be immutable as they represent the result of a computation (lexical analysis).

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_TOKEN_HPP
#define OGLA_TOKEN_HPP

//include standard c++ libraries
#include <string>
#include <vector>
#include <regex>
#include <memory>

namespace ogla {

//~forward declarations and function prototypes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Rule;     // type for describing a rule used to identify a token
using RuleList = std::vector<Rule>;
class Token;    // type representing a token in analyzed text
using TokenList = std::vector<Token>;
struct TokenRulePair;

template<class RandomAccessIterator>
TokenRulePair firstToken(RandomAccessIterator first, RandomAccessIterator last, const RuleList& rules, const int offset = 0);
TokenRulePair firstToken(const std::string& text, const RuleList& rules, const int offset = 0);
/*  - returns the first token identified and its corresponding rule
    - `first` is an iterator (prefer const_iterator) pointing to the first character of the text to be analyzed
    - `last` is an iterator (prefer const_iterator) pointing to one character past the end of the text to be analyzed
    - `text` is the text to be analyzed
    - `rules` is the list of rules checked when looking for the first token
    - `offset` is the offset from the start of the string at which to begin looking for a token
*/



//~classes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A class for describing a rule used to identify a token (tokenization rule).
*/
class Rule {
    public:
        Rule() {}
        Rule(const std::string& _name, const std::string& _regex)
            : ruleName{_name}, rgx{_regex} {}
        Rule(const std::string& _name, const std::string& _regex, std::weak_ptr<RuleList> _nextRules)
            : ruleName{_name}, rgx{_regex}, nextRules{_nextRules} {}
        /*  constructs a rule with the name `_name` and uses `_regex` as regular expression for matching */

        std::string name() const;

        std::regex regex() const;

        std::weak_ptr<RuleList> get_nextRules() const;

        bool operator==(const Rule& other);

        bool operator!=(const Rule& other);

    private:
        std::string ruleName;
        std::regex rgx;                     // holds the regular expression (regex) used to indentify the token
        std::weak_ptr<RuleList> nextRules;  // points to (but does not own) the next rules to be used for tokenization
};

class Token {
    public:
        Token() : offset{0} {}

        std::string name() const;

        int position() const;

        int length() const;

        std::string lexeme() const;

        Rule get_rule() const;

        bool operator==(const Token& other);

        bool operator!=(const Token& other);

    // friends:
    template<class RandomAccessIterator>
    friend TokenRulePair firstToken(RandomAccessIterator first, RandomAccessIterator last, const RuleList& rules, const int offset);
    //friend TokenRulePair firstToken(const std::string& text, const RuleList& rules, const int offset);

    private:
        Rule rule;          // holds the rule used to match the token
        std::string ruleName;
        std::smatch match;  // holds the lexem matched associated with the token
        int offset;         // holds the offset for the token position

        Token(Rule r, std::smatch m, int _offset = 0) : rule{r}, ruleName{r.name()}, match{m}, offset{_offset} {}
        /*  private constructor that creates a token */
};

struct TokenRulePair {
    Token token;
    Rule rule;
};



//~function implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
- returns the first token identified and its corresponding rule
- `first` is an iterator (prefer const_iterator) pointing to the first character of the text to be analyzed
- `last` is an iterator (prefer const_iterator) pointing to one character past the end of the text to be analyzed
- `rules` is the list of rules checked when looking for the first token
- `offset` is the offset from the start of the string at which to begin looking for a token
*/
template<class RandomAccessIterator>
TokenRulePair firstToken(RandomAccessIterator first, RandomAccessIterator last, const RuleList& rules, const int offset) {
    Token token;
    Rule rule;
    if (first + offset < last) {
        std::smatch m;
        for (auto r: rules) {
            if (std::regex_search(first + offset, last, m, r.regex()) && (m.position() + offset < token.position() || token.position() < 0)) {
                token = Token(r, m, offset);
                rule = r;
            }
        }
    }

    return TokenRulePair{token, rule};
}

/*TokenRulePair firstToken(const std::string& text, const RuleList& rules, const int offset) {
    return firstToken(text.begin(), text.end(), rules, offset);
}*/

}   // `ogla` namepsace

#endif//OGLA_TOKEN_HPP
