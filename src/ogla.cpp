/*
Project: OGLA
File: ogla.cpp
Author: Leonardo Banderali
Created: March 07, 2015
Last Modified: June 30, 2015

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

#include "../include/ogla/ogla.hpp"

//include standard c++ libraries
#include <algorithm>



//~function implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
- returns the first token identified using `rules`
- `text` is the text to be analyzed
- `offset` is the offset from the start of the string to at which to begin looking for a token
*/
ogla::Token ogla::firstToken(const std::string& text, const RuleList& rules, const int offset) {
    return firstToken(text.begin(), text.end(), rules, offset);
}

/*
returns a list of tokens representing `text` tokenized using `grammar`
*/
ogla::TokenList ogla::analyze(const std::string& text, const Grammar& grammar) {
    TokenList tl;   // token list to be returned

    int p = 0;
    while (true) {
        Token t = firstToken(text, grammar.rules, p);   // should call move constructor

        if (t.position() < 0)
            break;

        p = t.position() + t.length();
        tl.push_back(t);
    }

    return tl;
}



//~Grammar class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    g.rules.push_back(Rule("bar_rule", "\\bbar\\b"));
    g.rules.push_back(Rule("quux_rule", "\\bqu+x\\b"));
    g.rules.push_back(Rule("quick_rule", "\\bquick\\b"));
    g.rules.push_back(Rule("c_rule", "\\b[A-Za-z]+c[A-Za-z]+\\b"));
    //$                                                             $$
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

    return g;
}
