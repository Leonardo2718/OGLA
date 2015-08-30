/*
Project: OGLA
File: token.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: August 30, 2015

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

template <typename NextStateType> class BasicRule;  // type for describing a rule used to identify a token
class Token;                                        // type representing a token in analyzed text
using TokenList = std::vector<Token>;
struct TokenRulePair;



//~class templates~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A class template for describing a rule used to find a token (tokenization rule).
*/
template <typename NextStateType>
class BasicRule {
    public:
        BasicRule(NextStateType _nextState) : nextState{_nextState} {}
        BasicRule(const std::string& _name, const std::string& _regex, NextStateType _nextState)
            : ruleName{_name}, rgx{_regex}, nextState{_nextState} {}

        std::string name() const;

        std::regex regex() const;

        NextStateType next() const;

    private:
        std::string ruleName;
        std::regex rgx;             // holds the regular expression (regex) used to indentify the token
        NextStateType nextState;    // points to (but does not own) the next rules to be used for tokenization
};

template <typename NextStateType>
std::string BasicRule<NextStateType>::name() const {
    return ruleName;
}

template <typename NextStateType>
std::regex BasicRule<NextStateType>::regex() const {
    return rgx;
}

template <typename NextStateType>
NextStateType BasicRule<NextStateType>::next() const {
    return nextState;
}



//~classes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Token {
    public:
        Token() = default;
        Token(const std::string& _ruleName, const std::smatch& _match, int _pos = -1)
            :ruleName{_ruleName}, match{_match}, pos{_pos} {}

        bool empty() const;

        std::string name() const;

        int position() const;

        int length() const;

        std::string lexeme() const;

        bool operator==(const Token& other) const;

        bool operator!=(const Token& other) const;

    private:
        std::string ruleName;
        std::smatch match;  // the matched lexeme associated with the token
        int pos = -1;       // the assigned position of the token in the text (-1 is "no/don't care position")
};

}   // `ogla` namepsace

#endif//OGLA_TOKEN_HPP
