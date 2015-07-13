/*
Project: OGLA
File: analyzers.cpp
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

// library header
#include "../include/ogla/analyzers.hpp"



//~function implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
returns a list of tokens representing `text` tokenized using `grammar`
*/
ogla::TokenList ogla::analyze(const std::string& text, const Grammar& grammar) {
    TokenList tl;   // token list to be returned

    int p = 0;
    auto rules = grammar.rule_list(0);
    while(rules) {
        TokenRulePair trp = firstToken(text, *rules, p);    // should call move constructor

        if (trp.token.position() < 0)
            break;

        p = trp.token.position() + trp.token.length();
        tl.push_back(trp.token);

        rules = trp.rule.get_nextRules().lock();
    }

    return tl;
}
