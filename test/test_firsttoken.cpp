/*
Project: OGLA
File: test_firsttoken.cpp
Author: Leonardo Banderali
Created: June 27, 2015
Last Modified: June 30, 2015

Description: A simple unit test for `ogla::firstToken()`.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include <string>

#include "../include/ogla/ogla.hpp"

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_firsttoken ) {
    // define test objects
    std::string text = "The quick brown fox jumps over the lazy dog.\n";
    ogla::Rule rule1{"rule_1", "quick"};
    ogla::Rule rule2{"rule_2", "j[A-Za-z]+ps"};
    ogla::RuleList rules{rule1, rule2};

    // pre-test code
    ogla::Token token1 = ogla::firstToken(text, rules);
    ogla::Token token2 = ogla::firstToken(text.cbegin(), text.cend(), rules, 5);

    // run tests
    BOOST_CHECK_EQUAL( token1.name(), std::string("rule_1") );
    BOOST_CHECK_EQUAL( token2.name(), std::string("rule_2") );
    BOOST_CHECK_EQUAL( token1.lexeme(), std::string("quick") );
    BOOST_CHECK_EQUAL( token2.lexeme(), std::string("jumps") );
    BOOST_CHECK_EQUAL( token1.position(), 4 );
    BOOST_CHECK_EQUAL( token2.position(), 20 );
}
