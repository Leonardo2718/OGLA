/*
Project: OGLA
File: analyzers.hpp
Author: Leonardo Banderali
Created: July 12, 2015
Last Modified: July 12, 2015

Description:
    A `StepAnalyzer` object is a special kind of token iterator.  It points to a token in the analyzed text.  However,
    the next token is only generated when the iterator is incremented.  This is done by analyzing the text on-the-fly.
    Internally, the iterator stores a vector of all the previously generated tokens, as to allow backward iteration.
    This can be useful for programs that require taking specific action when a token is identified
    (e.g. a prittyprinter).

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_ANALYZERS_HPP
#define OGLA_ANALYZERS_HPP

// library headers
#include "grammar.hpp"

namespace ogla {

// function prototypes
TokenList analyze(const std::string& text, const Grammar& grammar);
/*  returns a list of tokens representing `text` tokenized using `grammar` */

//StepAnalyzer make_StepAnalyzer(const std::string& text, const Grammar& grammar);
/*  return a `StepAnalyzer` for analyzing the `text` using the given `grammar` */



// class declarations

/*
A class representing a token iterator where each token is generated on-the-fly by analyzing the given text.
*/
class StepAnalyzer {
    public:
        StepAnalyzer() : current_pos{0} {}
        StepAnalyzer(const std::string& _text, const Grammar& _grammar)
        : grammar{_grammar}, text_begin{_text.cbegin()}, text_end{_text.cend()}, current_pos{0},
        currentRules{_grammar.rule_list(0)} {
            operator++();
        }

        TokenRulePair& operator*() {
            return currentToken;
        }

        TokenRulePair* operator->() {
            return &currentToken;
        }

        StepAnalyzer& operator++() {
            current_pos = currentToken.token.position() + currentToken.token.length();
            currentToken = firstToken(text_begin, text_end, *currentRules.lock(), current_pos);
            currentRules = currentToken.rule.get_nextRules();
            return *this;
        }

        StepAnalyzer& operator++(int) {
            auto old = this;
            current_pos = currentToken.token.position() + currentToken.token.length();
            currentToken = firstToken(text_begin, text_end, *currentRules.lock(), current_pos);
            currentRules = currentToken.rule.get_nextRules();
            return *old;
        }

        bool operator==(const StepAnalyzer& other) const {
            return //text_begin == other.text_begin &&
                   //text_end == other.text_end &&
                   //current_itr == other.current_itr;// &&
                   currentToken.token == other.currentToken.token;// &&
                   //grammar == other.grammar;
        }

        bool operator!=(const StepAnalyzer& other) const {
            return !(*this == other);
        }

    //friends:
        //friend StepAnalyzer make_StepAnalyzer(const std::string& text, const Grammar& grammar);

    private:
        using text_itr = decltype(std::declval<std::string>().cbegin());

        Grammar grammar;

        const text_itr text_begin;
        const text_itr text_end;
        unsigned int current_pos;

        TokenRulePair currentToken;
        std::weak_ptr<const RuleList> currentRules;
};

}   // namespace `ogla`

#endif//OGLA_ANALYZERS_HPP
