/*
Project: OGLA
File: token.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: December 18, 2015

Description:
    A `Token` is a unit of analyzed text and is identified using a `Rule`.  These form the basic building blocks of the
    OGLA lexcial analyzer.  Tokens should be immutable as they represent the result of a computation (lexical analysis).

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_TOKEN_HPP
#define OGLA_TOKEN_HPP

// project headers
#include "rule.hpp"

// c++ standard libraries
#include <string>
#include <vector>

//~forward declare namespace members~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace ogla {

using smatch = boost::match_results<std::string::const_iterator>;

template <typename BidirectionalIterator, typename TokenTypeT> class BasicToken; // type representing a token in analyzed text

/*
Convenience function that constructs and returns a `BasicToken` object.
*/
template <typename BidirectionalIterator, typename TokenTypeT>
auto make_token(const TokenTypeT& tokenType, const boost::match_results<BidirectionalIterator>& match, int pos)
-> BasicToken<BidirectionalIterator, TokenTypeT>;

template <typename BidirectionalIterator, typename TokenTypeT>
using BasicTokenList = std::vector<BasicToken<BidirectionalIterator, TokenTypeT>>;

}   // `ogla` namepsace



//~Implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
As the name suggests, `BasicToken` is a class that represents a token.  Tokens are generated by a lexer using rules.
For the sake of generality, an instance of this class only containes basic information about a token, including:
its type (or category), its corresponding lexeme, and its position in the text (which may be optionally specified).
Any other information needed must be extracted by the user from the lexeme and other information already provided.
This essentailly offloads the work of learning the value of a token to an other tool such as a parser or semantic
analyzer.

The template paramaters are:
* TokenTypeT: the data type for the identifying the type/category of tokens the rule matches
* BidirectionalIterator: the iterator used to store regex matches

*/
template <typename BidirectionalIterator, typename TokenTypeT>
class ogla::BasicToken {
    public:
        using TokenType = TokenTypeT;
        using RegExMatch = boost::match_results<BidirectionalIterator>;

        BasicToken() = default;
        BasicToken(TokenTypeT _tokenType, const boost::match_results<BidirectionalIterator>& _match, int _pos = -1)
            :tokenType{_tokenType}, match{_match}, pos{_pos} {}

        bool empty() const;
        /*  returns true if the token is the result of an empty match (search result is empty) */

        auto type() const -> TokenType;
        /*  returns the type of the token */

        int position() const;
        /*  returns the specifed position of the token within the text searched (-1 is "no/don't care position") */

        auto lexeme() const -> typename RegExMatch::string_type;
        /*  returns the lexeme of this token */

        bool operator==(const BasicToken& other) const;

        bool operator!=(const BasicToken& other) const;

    private:
        TokenTypeT tokenType;
        RegExMatch match;   // the matched lexeme associated with the token
        int pos = -1;       // the assigned position of the token in the text (-1 is "no/don't care position")
};

/*
returns true if the token is the result of an empty match (search result is empty)
*/
template <typename BidirectionalIterator, typename TokenTypeT>
bool ogla::BasicToken<BidirectionalIterator, TokenTypeT>::empty() const {
    return match.empty();
}

/*
returns the type of the token
*/
template <typename BidirectionalIterator, typename TokenTypeT>
auto ogla::BasicToken<BidirectionalIterator, TokenTypeT>::type() const -> TokenType {
    return tokenType;
}

/*
returns the specifed position of the token within the text searched (-1 is "no/don't care position")
*/
template <typename BidirectionalIterator, typename TokenTypeT>
int ogla::BasicToken<BidirectionalIterator, TokenTypeT>::position() const {
    return pos;
}

/*
returns the lexeme of this token
*/
template <typename BidirectionalIterator, typename TokenTypeT>
auto ogla::BasicToken<BidirectionalIterator, TokenTypeT>::lexeme() const -> typename RegExMatch::string_type {
    if (match.empty())
        return std::string();
    else
        return match.str();
}

template <typename BidirectionalIterator, typename TokenTypeT>
bool ogla::BasicToken<BidirectionalIterator, TokenTypeT>::operator==(const BasicToken& other) const {
    return tokenType == other.tokenType && match == other.match && pos == other.pos;
}

template <typename BidirectionalIterator, typename TokenTypeT>
bool ogla::BasicToken<BidirectionalIterator, TokenTypeT>::operator!=(const BasicToken& other) const {
    return !(*this == other);
}



/*
Convenience function that constructs and returns a `BasicToken` object.
*/
template <typename BidirectionalIterator, typename TokenTypeT>
auto ogla::make_token(const TokenTypeT& tokenType, const boost::match_results<BidirectionalIterator>& match, int pos)
-> ogla::BasicToken<BidirectionalIterator, TokenTypeT> {
    return BasicToken<BidirectionalIterator, TokenTypeT>{tokenType, match, pos};
}

#endif//OGLA_TOKEN_HPP
