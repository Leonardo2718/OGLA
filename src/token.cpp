/*
Project: OGLA
File: token.cpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: July 22, 2015

Description:
    A `Token` is a unit of analyzed text and is identified using a `Rule`.  These form the basic building blocks of the
    OGLA lexcial analyzer.  Tokens should be immutable as they represent the result of a computation (lexical analysis).

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

// library header
#include "../include/ogla/token.hpp"



//~function implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
- returns the first token identified and its corresponding rule
- `text` is the text to be analyzed
- `rules` is the list of rules checked when looking for the first token
- `offset` is the offset from the start of the string to at which to begin looking for a token
*/
ogla::TokenRulePair ogla::firstToken(const std::string& text, const RuleList& rules, const int offset) {
    return firstToken(text.begin(), text.end(), rules, offset);
}



//~`Rule` public functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

std::string ogla::Rule::name() const {
    return ruleName;
}

std::regex ogla::Rule::regex() const {
    return rgx;
}

std::weak_ptr<const ogla::RuleList> ogla::Rule::get_nextRules() const {
    return nextRules;
}

bool ogla::Rule::operator==(const Rule& other) const {
    return ruleName == other.ruleName;// &&
           //rgx == other.rgx;// &&
           //nextRules == other.nextRules;
}

bool ogla::Rule::operator!=(const Rule& other) const {
    return !(*this == other);
}



//~`Token` public functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

std::string ogla::Token::name() const {
    return ruleName;
}

int ogla::Token::position() const {
    if (match.empty())
        return -1;
    else
        return match.position() + offset;
}

int ogla::Token::length() const {
    if (match.empty())
        return -1;
    else
        return match.length();
}

std::string ogla::Token::lexeme() const {
    if (match.empty())
        return std::string();
    else
        return match.str();
}

ogla::Rule ogla::Token::get_rule() const {
    return rule;
}

bool ogla::Token::operator==(const Token& other) const {
    return ruleName == other.ruleName &&
           match == other.match &&
           offset == other.offset;
}

bool ogla::Token::operator!=(const Token& other) const {
    return !(*this == other);
}
