/*
Project: OGLA
File: lexer_test.cpp
Author: Leonardo Banderali
Created: June 26, 2015
Last Modified: June 26, 2015

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

    ogla::TokenList tl = ogla::analyze(text, g);
    for (auto t : tl) {
        std::cout << t.position() << " " << t.lexeme() << " " << t.name() << std::endl;
    }

    return 0;
}
