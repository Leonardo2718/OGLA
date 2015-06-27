/*
Project: OGLA
File: ogla.cpp
Author: Leonardo Banderali
Created: March 07, 2015
Last Modified: March 08, 2015

Description:
    OGLA is generic lexical analyzer intended for quick and fast integration into
    any type of project requiring a lexer.  The main goal of this library is to
    provided an easy to use generic lexer for use in proof-of-concepts and other
    small projects.  It is not intended for use in time sensitive or highly
    optimized applications.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include "ogla.hpp"

//include standard c++ libraries
#include <algorithm>



/*
returns the first token identified using `rules` inside the string specified by a starting andending iterators
*/
template<class BidirectionalIterator>
ogla::Token ogla::firstToken(BidirectionalIterator first, BidirectionalIterator last, const RuleList& rules) {
    Token t;

    for (auto r: rules) {
        std::smatch m;
        if (std::regex_search(first, last, m, r.rgx) && (t.position() > m.position() || t.position() < 0))
            t = Token(r, m);
    }

    return t;
}

/*
returns the first token identified using `rules`
*/
ogla::Token ogla::firstToken(const std::string& text, const RuleList& rules) {
    /*Token t;

    // identify the first matching token
    for (auto r: rules) {
        std::smatch m;
        if (std::regex_search(text, m, r.rgx) && (t.position() > m.position() || t.position() < 0))
            t = Token(r, m);
    }*/

    return firstToken(text.begin(), text.end(), rules);
}

/*
returns the tokenized text as a list of tokens
*/
ogla::TokenList ogla::analyze(const std::string& text, const Grammar& grammar) {
    TokenList tl;   // token list to be returned

    auto startPos = text.begin();
    const auto endPos = text.end();
    Token t;// = firstToken(startPos, endPos, grammar.rules);
    int p = 0;
    while (true) {
        t = firstToken(startPos, endPos, grammar.rules);
        if (t.position() < 0)
            break;
        p = int(startPos - text.begin());
        startPos += t.position() + t.length();
        t.set_offset(p);
        tl.push_back(t);
    }

    return tl;
}



//~Grammar class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
loads the default grammar used to load other grammars
*/
ogla::Grammar ogla::Grammar::load() {
    Grammar g;
    g.langName = "ogla_default";

    //*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //$ This grammar is only used for testing purposes and does not $$
    //$ represent anything useful.
    g.rules.push_back(Rule("foo_rule", "foo"));
    g.rules.push_back(Rule("bar_rule", "bar"));
    g.rules.push_back(Rule("quux_rule", "quux"));
    //$                                                             $$
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

    return g;
}
