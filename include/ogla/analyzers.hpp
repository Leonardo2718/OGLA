/*
Project: OGLA
File: analyzers.hpp
Author: Leonardo Banderali
Created: July 12, 2015
Last Modified: July 30, 2015

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



// class declarations

/*
A class representing a token iterator-like object where each token is generated on-the-fly by analyzing the given text.
*/
class StepAnalyzer {
    public:
        // constructors
        StepAnalyzer() : current_pos{0} {}
        StepAnalyzer(const std::string& _text, const Grammar& _grammar);

        void jump_to(int pos, std::weak_ptr<const RuleList> rules);
        /*  sends the analyzer to `pos` in the text and sets the current rules to `rules` */

        void reset();
        /*  sends the analyzer back to the start of the text */

        TokenRulePair next();
        /*  moves to and returns the next token */

        // overloaded operators

        TokenRulePair& operator*();

        TokenRulePair* operator->();

        StepAnalyzer& operator++();

        StepAnalyzer& operator++(int);

    private:
        using text_itr = decltype(std::declval<std::string>().cbegin());

        Grammar grammar;

        const text_itr text_begin;
        const text_itr text_end;
        unsigned int current_pos;

        TokenRulePair currentPair;
        std::weak_ptr<const RuleList> currentRules;
};

}   // namespace `ogla`

#endif//OGLA_ANALYZERS_HPP
