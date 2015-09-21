/*
Project: OGLA
File: lexers.hpp
Author: Leonardo Banderali
Created: August 30, 2015
Last Modified: September 20, 2015

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
template <typename RandomAccessIterator, typename TokenType>
BasicTokenList<TokenType> analyze(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenType>& grammar);

/*
The `BasicLexer` class template provides a convenient interface for analyzing text one token at a time.  The interface
allows users to request each token sequentially.  This allows them to handle the tokens as the lexer generates them.

Upon instantiation, calling `current()` will return the first token in the text.  The first rule list in the grammar
(with grammar index `0`) will be used to find this token.  Calling `next()` will cause the analyzer to look for and
return the token following the current one.  This also sets the new token as the current one.  The position of tokens
is defined relative to the starting position of the text (called `first`).  An empty token is returned if no token
could be found in the text at any time.  This effectively terminates the analysis.
*/
template <typename RandomAccessIterator, typename TokenType>
class BasicLexer {
    public:
        BasicLexer(RandomAccessIterator _first, RandomAccessIterator _last, const BasicGrammar<TokenType>& _grammar);
        /*  @param first: points to the the start of the text
            @param last: points to one past the end of the text
            @param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any
                other rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
        */

        BasicToken<TokenType> current() const;
        /*  returns the token currently being referenced */

        BasicToken<TokenType> next();
        /*  generates, returns, and moves the internal reference to the next token in the text */

        BasicToken<TokenType> peek();
        /*  generates and returns the next token but does not set the internal reference to it */

    private:
        RandomAccessIterator first;
        RandomAccessIterator last;
        RandomAccessIterator currentPosition;
        BasicGrammar<TokenType> grammar;
        BasicGrammarIndex currentRuleList;
        BasicToken<TokenType> currentToken;
};

/*
Convenience function that constructs and returns a `BasicLexer` object
*/
template <typename RandomAccessIterator, typename TokenType>
BasicLexer<RandomAccessIterator, TokenType> make_lexer(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenType>& grammar);

}



//~template implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Generates a list of tokens form some text and the rules stored in a grammar.

@param first: points to the the start of the text
@param last: points to one past the end of the text
@param grammar: holds the tokenization rules. It must contain a minimum of one rule list as well as any other
    rule lists that is internally pointed to.  Otherwise, behaviour is undefined.
*/
template <typename RandomAccessIterator, typename TokenType> typename
ogla::BasicTokenList<TokenType> ogla::analyze(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenType>& grammar) {
    BasicTokenList<TokenType> tokenList;
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
            tokenList.push_back(BasicToken<TokenType>{rule.type(), match, current - first + match.position()}); // append the new token to the list

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
template <typename RandomAccessIterator, typename TokenType>
ogla::BasicLexer<RandomAccessIterator, TokenType>::BasicLexer(RandomAccessIterator _first, RandomAccessIterator _last, const BasicGrammar<TokenType>& _grammar)
: first{_first}, last{_last}, currentPosition{_first}, grammar{_grammar}, currentRuleList{0} {
    currentToken = next();
}

/*
returns the token currently being referenced
*/
template <typename RandomAccessIterator, typename TokenType>
ogla::BasicToken<TokenType> ogla::BasicLexer<RandomAccessIterator, TokenType>::current() const {
    return currentToken;
}

/*
generates, returns, and moves the internal reference to the next token in the text
*/
template <typename RandomAccessIterator, typename TokenType>
ogla::BasicToken<TokenType> ogla::BasicLexer<RandomAccessIterator, TokenType>::next() {
    if (currentRuleList < 0) {
        currentToken = BasicToken<TokenType>{};     // if the grammar index is negative, return an empty token
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
            currentToken = BasicToken<TokenType>{}; // if no match was found, return an empty token
        } else {
            auto rule = ruleList[ruleIndex];
            currentToken = BasicToken<TokenType>{rule.type(), match, currentPosition - first + match.position()};   // make the new token
            currentPosition += match.position() + match.length();                                   // move forward in the text
            currentRuleList = rule.nextState();
        }
    }

    return currentToken;
}

/*
generates and returns the next token but does not set the internal reference to it
*/
template <typename RandomAccessIterator, typename TokenType>
ogla::BasicToken<TokenType> ogla::BasicLexer<RandomAccessIterator, TokenType>::peek() {
    auto returnToken = BasicToken<TokenType>{};

    if (currentRuleList >= 0) {
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

        if (ruleIndex >= 0) {
            auto rule = ruleList[ruleIndex];
            returnToken = BasicToken<TokenType>{rule.type(), match, currentPosition - first + match.position()};
        }
    }

    return returnToken;
}

/*
Convenience function that constructs and returns a `BasicLexer` object
*/
template <typename RandomAccessIterator, typename TokenType>
ogla::BasicLexer<RandomAccessIterator, TokenType> ogla::make_lexer(RandomAccessIterator first, RandomAccessIterator last, const BasicGrammar<TokenType>& grammar) {
    return BasicLexer<RandomAccessIterator, TokenType>{first, last, grammar};
}

#endif//OGLA_LEXERS_HPP
