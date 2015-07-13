/*
Project: OGLA
File: grammar.cpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: July 12, 2015

Description:
    A `Grammar` is a set of `ogla::Rule`s that collectively define a "language".  This grammar can be used to analyze
    text.  Once analyzed, text is broken up into identified tokens.  For the sake of making the algorithms generic, if
    a text item is not identified as a token, it is simply discarded.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

// library header
#include "../include/ogla/grammar.hpp"

//standard c++ libraries
#include <algorithm>



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



//~Grammar class~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*  returns a given list of rules indentified by index `n` */
std::shared_ptr<const ogla::RuleList> ogla::Grammar::rule_list(size_type n) const {
    return rules.at(n);
}

bool ogla::Grammar::operator==(const Grammar& other) {
    return langName == other.langName;// &&
           //rules == other.rules;
}

bool ogla::Grammar::operator!=(const Grammar& other) {
    return !(*this == other);
}

/*
loads the default grammar used to load other grammars
*/
ogla::Grammar ogla::Grammar::load() {
    Grammar g;
    g.langName = "ogla_default";

    //*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //$ This grammar is only used for testing purposes and does not $$
    //$ represent anything useful.
    //g.rules.push_back(RuleList{});
    //auto rlist = g.rules.at(0);
    auto rlist = std::make_shared<RuleList>(RuleList{});
    g.rules.push_back(rlist);
    rlist->push_back(Rule{"foo_rule", "foo", rlist });
    rlist->push_back(Rule{"bar_rule", "\\bbar\\b", rlist});
    rlist->push_back(Rule{"quux_rule", "\\bqu+x\\b", rlist});
    rlist->push_back(Rule{"quick_rule", "\\bquick\\b", rlist});
    rlist->push_back(Rule{"c_rule", "\\b[A-Za-z]+c[A-Za-z]+\\b", rlist});

    // define rules for a basic string token
    auto rs = std::make_shared<RuleList>(RuleList{});
    g.rules.push_back(rs);
    rs->push_back(Rule{"escape_rule", "\\\\.", rs});
    rs->push_back(Rule{"end_str_rule", "\"", rlist});
    rlist->push_back(Rule{"str_rule", "\"", rs});
    //$                                                             $$
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

    return g;
}
