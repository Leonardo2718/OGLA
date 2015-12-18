/*
Project: OGLA
File: rule.hpp
Author: Leonardo Banderali
Created: December 17, 2015
Last Modified: December 18, 2015

Description:
    Lexers use a set of `Rule`s to find tokens. This file provides a class template for the rules used by OGLA lexers.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_RULE_HPP
#define OGLA_RULE_HPP

// c++ standard libraries
#include <regex>

//~forward declare namespace members~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace ogla {

template <typename TokenType, typename LexerState, typename charT> class BasicRule; // type for describing a rule used to identify a token
//template <typename TokenType, typename LexerState> class BasicRule;

template <typename TokenType, typename LexerState, typename charT> BasicRule<TokenType, LexerState, charT>
make_basic_rule(const TokenType& type, const std::basic_regex<charT>& regex, const LexerState& nextState);
/*  convenience function that constructs and returns a `BasicRule` object */

template <typename TokenType, typename LexerState> BasicRule<TokenType, LexerState, char>
make_rule(const TokenType& type, const std::string& regex, const LexerState& nextState);
/*  convenience function that constructs and returns a `BasicRule` object */

}   // `ogla` namepsace



/*
A class template for describing a rule used to find a token (a tokenization rule).  A rule essentially containes the
information needed by a lexer to find a token.  It also containes some information (a hint) as to what the lexer
*should* do if it finds a token using this rule.  Semantically, this information is represented as a state.  This makes
the most sence when thinking about the lexer as a finite-state-machine (FSM).

Rules have three basic properties:
1. a token type (which is the type or category of tokens the rule finds)
2. the regular expression used to search text
3. a definition of what the state of a lexer should be after generating/finding a token using the rule

Each rule should only be used to search for a single category of token.  For example, "keyword" can be a category.

The three template paramaters are:
* TokenType: the data type for the identifying the type/category of tokens the rule matches
* LexerState: the type used to represent lexer states
* charT: the type used for regular expressions

*/
template <typename TokenType, typename LexerState, typename charT>
//template <typename TokenType, typename LexerState>
class ogla::BasicRule {
    public:
        using RegEx = std::basic_regex<charT>;

        BasicRule(LexerState _nState) : nState{_nState} {}
        BasicRule(const TokenType& _type, const std::basic_regex<charT>& _regex, LexerState _nState)
            : tokenType{_type}, rgx{_regex}, nState{_nState} {}

        TokenType type() const;
        /*  returns the type of token the rule finds */

        //std::basic_regex<charT> regex() const;
        RegEx regex() const;
        /*  returns the regular expression used to find the token associated with this rule */

        LexerState nextState() const;
        /*  returns the state the lexer should have after finding a token from this rule */

    private:
        TokenType tokenType;
        RegEx rgx;              // holds the regular expression (regex) used to indentify the token
        LexerState nState;      // points to (but does not own) the next rules to be used for tokenization
};

/*
returns the type of token the rule finds
*/
template <typename TokenType, typename LexerState, typename charT>
TokenType ogla::BasicRule<TokenType, LexerState, charT>::type() const {
    return tokenType;
}

/*
returns the regular expression used to find the token associated with this rule
*/
template <typename TokenType, typename LexerState, typename charT> typename
ogla::BasicRule<TokenType, LexerState, charT>::RegEx ogla::BasicRule<TokenType, LexerState, charT>::regex() const {
    return rgx;
}

/*
returns the state the lexer should have after finding a token from this rule
*/
template <typename TokenType, typename LexerState, typename charT>
LexerState ogla::BasicRule<TokenType, LexerState, charT>::nextState() const {
    return nState;
}



/*
convenience function that constructs and returns a `BasicRule` object
*/
template <typename TokenType, typename LexerState, typename charT> ogla::BasicRule<TokenType, LexerState, charT>
ogla::make_basic_rule(const TokenType& type, const std::basic_regex<charT>& regex, const LexerState& nextState) {
    return BasicRule<TokenType, LexerState, charT>{type, regex, nextState};
}

/*
convenience function that constructs and returns a `BasicRule` object
*/
template <typename TokenType, typename LexerState> ogla::BasicRule<TokenType, LexerState, char>
ogla::make_rule(const TokenType& type, const std::string& regex, const LexerState& nextState) {
    return BasicRule<TokenType, LexerState, char>{type, std::regex(regex), nextState};
}

#endif//OGLA_RULE_HPP
