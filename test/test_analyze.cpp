/*
Project: OGLA
File: test_analyze.cpp
Author: Leonardo Banderali
Created: June 28, 2015
Last Modified: August 26, 2015

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



//~helper functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Tests for equallity between two values; prints a friendly error message if test fails.
*/
template<typename T>
void eq_test(const T& a, const T& b, const std::string& lable, int index) {
    BOOST_CHECK_MESSAGE(a == b, "[" << index << "]" << lable << a << ", expected: " << b);
}

void test_lexeme(const ogla::Token& token, const std::tuple<std::string, int, std::string>& exp, int index) {
    eq_test(token.lexeme(), std::get<0>(exp), " lexeme: ", index);
}

void test_position(const ogla::Token& token, const std::tuple<std::string, int, std::string>& exp, int index) {
    eq_test(token.position(), std::get<1>(exp), " position: ", index);
}

void test_name(const ogla::Token& token, const std::tuple<std::string, int, std::string>& exp, int index) {
    eq_test(token.name(), std::get<2>(exp), " name: ", index);
}



//~tests~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BOOST_AUTO_TEST_CASE( test_analyze ) {
    // pre-test code
    ogla::TokenList tokens = ogla::analyze(text, grammar);

    // run test
    BOOST_CHECK_MESSAGE(tokens.size() == expected_tokens.size(),
                        "token count: " << tokens.size() << ", expected: " << expected_tokens.size());
    for (int i = 0, s = tokens.size(); i < s; i++) {
        test_lexeme(tokens.at(i), expected_tokens.at(i), i);
        test_position(tokens.at(i), expected_tokens.at(i), i);
        test_name(tokens.at(i), expected_tokens.at(i), i);
    }
}



BOOST_AUTO_TEST_CASE( test_analyzer ) {
    // pre-test code
    ogla::StepAnalyzer analyzer{text, grammar};

    // run test
    for (int i = 0, s = expected_tokens.size(); i < s; i++, analyzer++) {
        test_lexeme(*analyzer, expected_tokens.at(i), i);
        test_position(*analyzer, expected_tokens.at(i), i);
        test_name(*analyzer, expected_tokens.at(i), i);
    }
}
