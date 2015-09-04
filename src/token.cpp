/*
Project: OGLA
File: token.cpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: August 31, 2015

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
returns the type of the token
*/
std::string ogla::Token::type() const {
    return tokenType;
}

/*
returns the specifed position of the token within the text searched (-1 is "no/don't care position")
*/
int ogla::Token::position() const {
    return pos;
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
    return tokenType == other.tokenType && match == other.match && pos == other.pos;
}

bool ogla::Token::operator!=(const Token& other) const {
    return !(*this == other);
}
