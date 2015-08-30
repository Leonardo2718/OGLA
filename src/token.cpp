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

/*
returns true if the token is the result of an empty match (search result is empty)
*/
bool ogla::Token::empty() const {
    return match.empty();
}

/*
returns the name of the token (should match name of the rule used to find it)
*/
std::string ogla::Token::name() const {
    return ruleName;
}

/*
returns the specifed position of the token within the text searched (-1 is "no/don't care position")
*/
int ogla::Token::position() const {
    return pos;
}

/*
returns the length of the lexeme (will be deprecated)
*/
int ogla::Token::length() const {
    if (match.empty())
        return -1;
    else
        return match.length();
}

/*
returns the lexeme of this token
*/
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
