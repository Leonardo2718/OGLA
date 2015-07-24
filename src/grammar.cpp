/*
Project: OGLA
File: grammar.cpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: July 24, 2015

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



/*  returns a given list of rules indentified by index `n` */
std::weak_ptr<const ogla::RuleList> ogla::Grammar::rule_list(size_type n) const {
    return rules.at(n);
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
