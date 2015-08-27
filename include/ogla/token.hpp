/*
Project: OGLA
File: token.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: August 26, 2015

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

namespace ogla {

//~forward declarations and function prototypes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename NextRulesPointer> class BasicRule;   // type for describing a rule used to identify a token
class Token;                                            // type representing a token in analyzed text
using TokenList = std::vector<Token>;
struct TokenRulePair;



//~class templates~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A class template for describing a rule used to identify a token (tokenization rule).
*/
template <typename NextRulesPointer>
class BasicRule {
    public:
        BasicRule(NextRulesPointer _nextRules) : nextRules{_nextRules} {}
        BasicRule(const std::string& _name, const std::string& _regex, NextRulesPointer _nextRules)
            : ruleName{_name}, rgx{_regex}, nextRules{_nextRules} {}

        std::string name() const;

        std::regex regex() const;

        NextRulesPointer get_nextRules() const;

    private:
        std::string ruleName;
        std::regex rgx;             // holds the regular expression (regex) used to indentify the token
        NextRulesPointer nextRules; // points to (but does not own) the next rules to be used for tokenization
};

template <typename NextRulesPointer>
std::string BasicRule<NextRulesPointer>::name() const {
    return ruleName;
}

template <typename NextRulesPointer>
std::regex BasicRule<NextRulesPointer>::regex() const {
    return rgx;
}

template <typename NextRulesPointer>
NextRulesPointer BasicRule<NextRulesPointer>::get_nextRules() const {
    return nextRules;
}



//~classes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Token {
    public:
        Token() : offset{0} {}
        Token(const std::string& _ruleName, const std::smatch& _match, int _offset = 0)
            :ruleName{_ruleName}, match{_match}, offset{_offset} {}

        std::string name() const;

        int position() const;

        int length() const;

        std::string lexeme() const;

        bool operator==(const Token& other) const;

        bool operator!=(const Token& other) const;

    private:
        std::string ruleName;
        std::smatch match;  // holds the lexem matched associated with the token
        int offset;         // holds the offset for the token position
};

}   // `ogla` namepsace

#endif//OGLA_TOKEN_HPP
