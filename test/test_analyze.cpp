/*
Project: OGLA
File: test_analyze.cpp
Author: Leonardo Banderali
Created: June 28, 2015
Last Modified: July 12, 2015

Description: A simple unit test for `ogla::firstToken()`.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include <string>
#include <vector>
#include <tuple>

#include "../include/ogla/ogla.hpp"

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

//~test subjects~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const std::string text{"The quick brown fox jumps over the lazy dog.\n"
                 "foo bar quux\n"
                 "gosofooeiowe secbarsde qux quuuuuuuuuux\n"
                 "This is \"an \\t attempt\" to parse a string\n"};
const ogla::Grammar grammar = ogla::Grammar::load();
const std::vector<std::tuple<std::string, int, std::string>> expected_tokens{
    std::make_tuple("quick", 4, "quick_rule"),
    std::make_tuple("foo", 45, "foo_rule"),
    std::make_tuple("bar", 49, "bar_rule"),
    std::make_tuple("quux", 53, "quux_rule"),
    std::make_tuple("foo", 62, "foo_rule"),
    std::make_tuple("secbarsde", 71, "c_rule"),
    std::make_tuple("qux", 81, "quux_rule"),
    std::make_tuple("quuuuuuuuuux", 85, "quux_rule"),
    std::make_tuple("\"", 106, "str_rule"),
    std::make_tuple("\\t", 110, "escape_rule"),
    std::make_tuple("\"", 120, "end_str_rule")
};



//~tests~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BOOST_AUTO_TEST_CASE( test_analyze ) {
    // pre-test code
    ogla::TokenList tokens = ogla::analyze(text, grammar);

    // run test
    BOOST_CHECK_MESSAGE(tokens.size() == expected_tokens.size(),
                        "token count: " << tokens.size() << ", expected: " << expected_tokens.size());
    for (int i = 0, s = tokens.size(); i < s; i++) {
        BOOST_CHECK_MESSAGE(tokens.at(i).lexeme() == std::get<0>(expected_tokens.at(i)),
                            "[" << i << "]" << " lexem: " << tokens.at(i).lexeme()
                                            << ", exprected: " << std::get<0>(expected_tokens.at(i)));

        BOOST_CHECK_MESSAGE(tokens.at(i).position() == std::get<1>(expected_tokens.at(i)),
                            "[" << i << "]" << " position: " << tokens.at(i).position()
                                            << ", expected: " << std::get<1>(expected_tokens.at(i)));

        BOOST_CHECK_MESSAGE(tokens.at(i).name() == std::get<2>(expected_tokens.at(i)),
                            "[" << i << "]" << " name: " << tokens.at(i).name()
                                            << ", expected: " << std::get<2>(expected_tokens.at(i)));
    }
}



BOOST_AUTO_TEST_CASE( test_analyzer ) {
    // pre-test code
    ogla::StepAnalyzer analyzer{text, grammar};
    const ogla::StepAnalyzer end;

    // run test
    for (int i = 0, s = expected_tokens.size(); i < s; i++, analyzer++) {
        BOOST_CHECK_MESSAGE(analyzer->token.lexeme() == std::get<0>(expected_tokens.at(i)),
                            "[" << i << "]" << " lexem: " << analyzer->token.lexeme()
                                            << ", exprected: " << std::get<0>(expected_tokens.at(i)));

        BOOST_CHECK_MESSAGE(analyzer->token.position() == std::get<1>(expected_tokens.at(i)),
                            "[" << i << "]" << " position: " << analyzer->token.position()
                                            << ", expected: " << std::get<1>(expected_tokens.at(i)));

        BOOST_CHECK_MESSAGE(analyzer->token.name() == std::get<2>(expected_tokens.at(i)),
                            "[" << i << "]" << " name: " << analyzer->token.name()
                                            << ", expected: " << std::get<2>(expected_tokens.at(i)));
    }
}
