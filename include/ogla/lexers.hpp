/*
Project: OGLA
File: lexers.hpp
Author: Leonardo Banderali
Created: August 30, 2015
Last Modified: August 31, 2015

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
#include <regex>
#include <utility>



//~declaration of namespace members~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
template <typename RandomAccessIterator>
TokenList analyze(RandomAccessIterator first, RandomAccessIterator last, const Grammar& grammar);

/*
The `BasicLexer` class template provides a convenient interface for analyzing text one token at a time.  The interface
allows users to request each token sequentially.  This allows them to handle the tokens as the lexer generates them.

Upon instantiation, calling `current()` will return the first token in the text.  The first rule list in the grammar
(with grammar index `0`) will be used to find this token.  Calling `next()` will cause the analyzer to look for and
return the token following the current one.  This also sets the new token as the current one.  The position of tokens
is defined relative to the starting position of the text (called `first`).  An empty token is returned if no token
could be found in the text at any time.  This effectively terminates the analysis.
*/
template <typename RandomAccessIterator>
class BasicLexer {
    public:
        BasicLexer(RandomAccessIterator _first, RandomAccessIterator _last, const Grammar& _grammar);
        /*  @param first: points to the the start of the text
            @param last: points to one past the end of the text
            @param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any
                other rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
        */

        Token current() const;
        /*  returns the token currently being referenced */

        Token next();
        /*  generates, returns, and moves the internal reference to the next token in the text */

    private:
        RandomAccessIterator first;
        RandomAccessIterator last;
        RandomAccessIterator currentPosition;
        Grammar grammar;
        GrammarIndex currentRuleList;
        Token currentToken;
};

/*
Convenience function that constructs and returns a `BasicLexer` object
*/
template <typename RandomAccessIterator>
BasicLexer<RandomAccessIterator> make_lexer(RandomAccessIterator first, RandomAccessIterator last, const Grammar& grammar);

}



//~template implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Generates a list of tokens form some text and the rules stored in a grammar.

@param first: points to the the start of the text
@param last: points to one past the end of the text
@param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any other
    rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
*/
template <typename RandomAccessIterator>
ogla::TokenList ogla::analyze(RandomAccessIterator first, RandomAccessIterator last, const Grammar& grammar) {
    TokenList tokenList;
    RandomAccessIterator current = first;
    auto ruleList = grammar[0];

    while (current < last) {
        int ruleIndex = -1;
        std::smatch match;
        std::smatch tmpMatch;

        // look for the rule that has the first match (in terms of position in the text)
        for (int i = 0, count = ruleList.size(); i < count; ++i) {
            if (std::regex_search(current, last, tmpMatch, ruleList[i].regex())
                                && (ruleIndex < 0 || tmpMatch.position() < match.position())) {
                match = std::move(tmpMatch);
                ruleIndex = i;
            }
        }

        if (ruleIndex < 0) {
            break;  // if no match was found, terminate the analysis
        } else {
            auto rule = ruleList[ruleIndex];
            tokenList.push_back(Token{rule.name(), match, current - first + match.position()}); // append the new token to the list

            auto nextRuleList = rule.nextState();
            if (nextRuleList < 0) {
                break;  // if the next grammar index is negative, terminate the analysis
            } else {
                current += match.position() + match.length();   // move forward in the text
                ruleList = grammar[nextRuleList];               // load the next rule list
            }
        }
    }

    return tokenList;
}



/*
@param first: points to the the start of the text
@param last: points to one past the end of the text
@param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any
    other rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
*/
template <typename RandomAccessIterator>
ogla::BasicLexer<RandomAccessIterator>::BasicLexer(RandomAccessIterator _first, RandomAccessIterator _last, const Grammar& _grammar)
: first{_first}, last{_last}, currentPosition{_first}, grammar{_grammar}, currentRuleList{0} {
    currentToken = next();
}

/*
returns the token currently being referenced
*/
template <typename RandomAccessIterator>
ogla::Token ogla::BasicLexer<RandomAccessIterator>::current() const {
    return currentToken;
}

/*
generates, returns, and moves the internal reference to the next token in the text
*/
template <typename RandomAccessIterator>
ogla::Token ogla::BasicLexer<RandomAccessIterator>::next() {
    if (currentRuleList < 0) {
        currentToken = Token{};     // if the grammar index is negative, return an empty token
    } else {
        auto ruleList = grammar[currentRuleList];
        int ruleIndex = -1;
        std::smatch match;
        std::smatch tmpMatch;

        // look for the rule that has the first match (in terms of position in the text)
        for (int i = 0, count = ruleList.size(); i < count; ++i) {
            if (std::regex_search(currentPosition, last, tmpMatch, ruleList[i].regex())
                                && (ruleIndex < 0 || tmpMatch.position() < match.position())) {
                match = std::move(tmpMatch);
                ruleIndex = i;
            }
        }

        if (ruleIndex < 0) {
            currentToken = Token{}; // if no match was found, return an empty token
        } else {
            auto rule = ruleList[ruleIndex];
            currentToken = Token{rule.name(), match, currentPosition - first + match.position()};   // make the new token
            currentPosition += match.position() + match.length();                                   // move forward in the text
            currentRuleList = rule.nextState();
        }
    }

    return currentToken;
}

/*
Convenience function that constructs and returns a `BasicLexer` object
*/
template <typename RandomAccessIterator>
ogla::BasicLexer<RandomAccessIterator> ogla::make_lexer(RandomAccessIterator first, RandomAccessIterator last, const Grammar& grammar) {
    return BasicLexer<RandomAccessIterator>{first, last, grammar};
}

#endif//OGLA_LEXERS_HPP
