/*
Project: OGLA
File: test_lexer.cpp
Author: Leonardo Banderali
Created: June 26, 2015
Last Modified: June 27, 2015

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include <iostream>
#include "../include/ogla/ogla.hpp"

int main() {
    //*
    ogla::Grammar g = ogla::Grammar::load();
    std::string text = "Hello, my name is foo, what's yours?\nbar.\nNice to meet you quux!\n";
    std::cout << text;
    ogla::Rule rule1{"rule_1", "quick"};
    ogla::Rule rule2{"rule_2", "j[A-Za-z]+ps"};
    ogla::RuleList rules{rule1, rule2};
    ogla::Token token2 = ogla::firstToken(text.cbegin(), text.cend(), rules);

    ogla::TokenList tl = ogla::analyze(text, g);
    for (auto t : tl) {
        std::cout << t.position() << " " << t.lexeme() << " " << t.name() << std::endl;
    }

    return 0;
}
