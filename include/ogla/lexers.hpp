/*
Project: OGLA
File: lexers.hpp
Author: Leonardo Banderali
Created: August 30, 2015
Last Modified: December 25, 2015

Description:
    This file declares some lexers that make use of the facilities provided by this library.  As with the rest of this
    library, these are intended to be very generic.  Hence, they are not always optimzed for performance.  However,
    the libary should be flexible enough for users to create their own lexers if they whish.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_LEXERS_HPP
#define OGLA_LEXERS_HPP

// project headers
#include "grammar.hpp"

// standard libraries
#include <utility>



//~forward declare namespace members~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace ogla {

/*
Generates a list of tokens form some text and the rules stored in a grammar.  The tokens generated are non-empty
and the position of their lexeme is defined with respect to `first`.  The first rule list in the grammar
(with grammar index `0`) will be used to find the first token.

@param first: points to the the start of the text
@param last: points to one past the end of the text
@param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any other
    rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
auto basic_analyze(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenTypeT, charT>& grammar)
-> BasicTokenList<RandomAccessIterator, TokenTypeT>;

/*
Provides a convenient interface for analyzing text one token at a time.
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
class BasicLexer;

template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
auto make_lexer(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenTypeT, charT>& grammar)
-> BasicLexer<RandomAccessIterator, TokenTypeT, charT>;
/*  convenience function that constructs and returns a `BasicLexer` object */

}   // namespace `ogla`



/*
Generates a list of tokens form some text and the rules stored in a grammar.

@param first: points to the the start of the text
@param last: points to one past the end of the text
@param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any other
    rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT> auto
ogla::basic_analyze(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenTypeT, charT>& grammar)
-> typename ogla::BasicTokenList<RandomAccessIterator, TokenTypeT> {
    using RegExMatch = typename BasicToken<RandomAccessIterator, TokenTypeT>::RegExMatch;
    using GrammarRule = BasicGrammarRule<TokenTypeT, charT>;

    BasicTokenList<RandomAccessIterator, TokenTypeT> tokenList;
    RandomAccessIterator currentPosition = first;
    auto currentRuleList = 0;

    while (currentPosition < last) {
        GrammarRule rule{-1};
        RegExMatch firstMatch;
        RegExMatch m;
        for (const auto& r : grammar[currentRuleList]) {
            if (std::regex_search(currentPosition, last, m, r.regex()) && (firstMatch.empty() || m.position() < firstMatch.position() )) {
                firstMatch = std::move(m);
                rule = r;
            }
        }

        if (firstMatch.empty()) {
            break;
        } else {
            currentPosition += firstMatch.position();
            tokenList.push_back(make_token(rule.type(), firstMatch, currentPosition - first)); // append the new token to the list
            currentPosition += firstMatch.length();
            currentRuleList = rule.nextState();
        }
    }

    return tokenList;
}



/*
The `BasicLexer` class template provides a convenient interface for analyzing text one token at a time.  The interface
allows users to request each token sequentially.  This allows them to handle the tokens as the lexer generates them.

Upon instantiation, calling `current()` will return the first token in the text.  The first rule list in the grammar
(with grammar index `0`) will be used to find this token.  Calling `next()` will cause the analyzer to look for and
return the token following the current one.  This also sets the new token as the current one.  The position of tokens
is defined relative to the starting position of the text (called `first`).  An empty token is returned if no token
could be found in the text at any time.  This effectively terminates the analysis.
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
class ogla::BasicLexer {
    public:
        using Token = BasicToken<RandomAccessIterator, TokenTypeT>;
        using Grammar = BasicGrammar<TokenTypeT, charT>;
        using GrammarRule = BasicGrammarRule<TokenTypeT, charT>;

        BasicLexer(RandomAccessIterator _first, RandomAccessIterator _last, const BasicGrammar<TokenTypeT, charT>& _grammar);
        /*  @param first: points to the the start of the text
            @param last: points to one past the end of the text
            @param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any
                other rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
        */

        auto current() const -> Token;
        /*  returns the token currently being referenced */

        auto next() -> Token;
        /*  generates, returns, and moves the internal reference to the next token in the text */

        auto peek() -> Token;
        /*  generates and returns the next token but does not set the internal reference to it */

    private:
        RandomAccessIterator first;
        RandomAccessIterator last;
        RandomAccessIterator currentPosition;
        Grammar grammar;
        BasicGrammarIndex currentRuleList;
        Token currentToken;
};



/*
@param first: points to the the start of the text
@param last: points to one past the end of the text
@param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any
    other rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
ogla::BasicLexer<RandomAccessIterator, TokenTypeT, charT>::BasicLexer(RandomAccessIterator _first, RandomAccessIterator _last, const BasicGrammar<TokenTypeT, charT>& _grammar)
: first{_first}, last{_last}, currentPosition{_first}, grammar{_grammar}, currentRuleList{0} {
    currentToken = next();
}

/*
returns the token currently being referenced
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
auto ogla::BasicLexer<RandomAccessIterator, TokenTypeT, charT>::current() const -> Token {
    return currentToken;
}

/*
generates, returns, and moves the internal reference to the next token in the text
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
auto ogla::BasicLexer<RandomAccessIterator, TokenTypeT, charT>::next() -> Token {
    if (currentRuleList < 0 || currentPosition >= last) {
        currentToken = Token{}; // if the grammar index is negative, return an empty token
    } else {
        GrammarRule rule{-1};
        typename Token::RegExMatch firstMatch;
        typename Token::RegExMatch m;
        for (const auto& r : grammar[currentRuleList]) {
            if (std::regex_search(currentPosition, last, m, r.regex()) && (firstMatch.empty() || m.position() < firstMatch.position() )) {
                firstMatch = std::move(m);
                rule = r;
            }
        }

        if (firstMatch.empty()) {
            currentToken = Token{};
        } else {
            currentPosition += firstMatch.position();
            currentToken = make_token(rule.type(), firstMatch, currentPosition - first);
            currentPosition += firstMatch.length();
            currentRuleList = rule.nextState();
        }
    }

    return currentToken;
}

/*
generates and returns the next token but does not set the internal reference to it
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT>
auto ogla::BasicLexer<RandomAccessIterator, TokenTypeT, charT>::peek() -> Token {
    auto returnToken = Token{};

    if (currentRuleList >= 0 && currentPosition < last) {
        typename Token::RegExMatch firstMatch;
        typename Token::RegExMatch m;
        for (const auto& r : grammar[currentRuleList]) {
            if (std::regex_search(currentPosition, last, m, r.regex()) && (firstMatch.empty() || m.position() < firstMatch.position() )) {
                firstMatch = std::move(m);
                returnToken = make_token(r.type(), firstMatch, firstMatch.position() + currentPosition - first);
            }
        }
    }

    return returnToken;
}



/*
Convenience function that constructs and returns a `BasicLexer` object
*/
template <typename RandomAccessIterator, typename TokenTypeT, typename charT> auto
ogla::make_lexer(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenTypeT, charT>& grammar)
-> ogla::BasicLexer<RandomAccessIterator, TokenTypeT, charT> {
    return BasicLexer<RandomAccessIterator, TokenTypeT, charT>(first, last, grammar);
}

#endif//OGLA_LEXERS_HPP
