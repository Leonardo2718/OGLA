/*
Project: OGLA
File: token.cpp
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

// library header
#include "token.hpp"



//~`Token` public functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool ogla::Token::empty() const {
    return match.empty();
}

std::string ogla::Token::name() const {
    return ruleName;
}

int ogla::Token::position() const {
    return pos;
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

bool ogla::Token::operator==(const Token& other) const {
    return ruleName == other.ruleName &&
           match == other.match &&
           pos == other.pos;
}

bool ogla::Token::operator!=(const Token& other) const {
    return !(*this == other);
}
