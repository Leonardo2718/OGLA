/*
Project: OGLA
File: grammar.cpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: August 30, 2015

Description:
    A `Grammar` is a set of tokenization rules that collectively define a "language".  A grammar can be used to analyze
    text.  Once analyzed, the text is broken up into identified tokens.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

// library header
#include "grammar.hpp"

//standard c++ libraries
//#include <algorithm>



/*  returns a given list of rules indentified by index `n` */
/*const std::vector<ogla::Grammar::Rule>& ogla::Grammar::rule_list(size_type n) const {
    return rules.at(n);
}*/

/*
loads the default grammar used to load other grammars
*/
/*ogla::Grammar ogla::Grammar::load() {
    Grammar g;
    g.langName = "ogla_default";

    //*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //$ This grammar is only used for testing purposes and does not $$
    //$ represent anything useful.                                  $$
    g.rules.resize(2);

    g.rules[0].reserve(5);
    g.rules[0].push_back(Rule{"foo_rule", "foo", 0});
    g.rules[0].push_back(Rule{"bar_rule", "\\bbar\\b", 0});
    g.rules[0].push_back(Rule{"quux_rule", "\\bqu+x\\b", 0});
    g.rules[0].push_back(Rule{"quick_rule", "\\bquick\\b", 0});
    g.rules[0].push_back(Rule{"c_rule", "\\b[A-Za-z]+c[A-Za-z]+\\b", 0});

    // rules for a basic string tokenization
    g.rules[0].push_back(Rule{"str_rule", "\"", 1});
    g.rules[1].push_back(Rule{"escape_rule", "\\\\.", 1});
    g.rules[1].push_back(Rule{"end_str_rule", "\"", 0});
    //$                                                             $$
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*

    return g;
}*/

ogla::Grammar ogla::load() {
    Grammar g;

    //*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //$ This grammar is only used for testing purposes and does not $$
    //$ represent anything useful.                                  $$
    g.resize(2);

    g[0].reserve(5);
    g[0].push_back(Rule{"foo_rule", "foo", 0});
    g[0].push_back(Rule{"bar_rule", "\\bbar\\b", 0});
    g[0].push_back(Rule{"quux_rule", "\\bqu+x\\b", 0});
    g[0].push_back(Rule{"quick_rule", "\\bquick\\b", 0});
    g[0].push_back(Rule{"c_rule", "\\b[A-Za-z]+c[A-Za-z]+\\b", 0});

    // rules for a basic string tokenization
    g[0].push_back(Rule{"str_rule", "\"", 1});
    g[1].push_back(Rule{"escape_rule", "\\\\.", 1});
    g[1].push_back(Rule{"end_str_rule", "\"", 0});
    //$                                                             $$
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

    return g;
}
