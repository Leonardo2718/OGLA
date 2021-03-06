/*
Project: OGLA
File: test_analyze.cpp
Author: Leonardo Banderali
Created: August 30, 2015
Last Modified: December 18, 2015

Description: A simple unit test for `ogla::firstToken()`.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include <string>
#include <vector>
#include <tuple>

#include "ogla/ogla.hpp"

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

//~test subjects~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//the string to be analyzed
const std::string text{"The quick brown fox jumps over the lazy dog.\n"
                 "foo bar quux\n"
                 "gosofooeiowe secbarsde qux quuuuuuuuuux\n"
                 "This is \"an \\t attempt\" to parse a string\n"};

// the test rules to be used by the lexer
const auto grammar = ogla::make_basic_grammar({
    {   // objects must convert explicitly constructed because parameters are templated
        ogla::make_basic_rule(std::string("foo_rule"), std::regex("foo"), 0),
        ogla::make_basic_rule(std::string("bar_rule"), std::regex("\\bbar\\b"), 0),
        ogla::make_basic_rule(std::string("quux_rule"), std::regex("\\bqu+x\\b"), 0),
        ogla::make_basic_rule(std::string("quick_rule"), std::regex("\\bquick\\b"), 0),
        ogla::make_basic_rule(std::string("c_rule"), std::regex("\\b[A-Za-z]+c[A-Za-z]+\\b"), 0),
        ogla::make_basic_rule(std::string("str_rule"), std::regex("\""), 1)
    }
    ,
    {
        ogla::make_basic_rule(std::string("escape_rule"), std::regex("\\\\."), 1),
        ogla::make_basic_rule(std::string("end_str_rule"), std::regex("\""), 0)
    }
});

const std::vector<std::vector<std::tuple<std::string, int>>> expected_rules = {
    {
        std::make_tuple(std::string("foo_rule"), 0),
        std::make_tuple(std::string("bar_rule"), 0),
        std::make_tuple(std::string("quux_rule"), 0),
        std::make_tuple(std::string("quick_rule"), 0),
        std::make_tuple(std::string("c_rule"), 0),
        std::make_tuple(std::string("str_rule"), 1)
    }
    ,
    {
        std::make_tuple(std::string("escape_rule"), 1),
        std::make_tuple(std::string("end_str_rule"), 0)
    }
};

// a representation of the tokens expected from the lexer
const std::vector<std::tuple<std::string, std::string, int>> expected_tokens = {
    std::make_tuple("quick_rule", "quick", 4),
    std::make_tuple("foo_rule", "foo", 45),
    std::make_tuple("bar_rule", "bar", 49),
    std::make_tuple("quux_rule", "quux", 53),
    std::make_tuple("foo_rule", "foo", 62),
    std::make_tuple("c_rule", "secbarsde", 71),
    std::make_tuple("quux_rule", "qux", 81),
    std::make_tuple("quux_rule", "quuuuuuuuuux", 85),
    std::make_tuple("str_rule", "\"", 106),
    std::make_tuple("escape_rule", "\\t", 110),
    std::make_tuple("end_str_rule", "\"", 120)
};



//~helpers~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// a macro that generates the message to be printed by a BOOST_CHECK_MESSAGE
#define MAKE_MESSAGE(token, tuple) "expected:{" << std::get<0>(tuple) << "," << std::get<1>(tuple) << "," << std::get<2>(tuple) \
                                                << "} got:{" << (token.type()) << "," << (token.lexeme()) << "," << (token.position()) << "}"



//~tests~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BOOST_AUTO_TEST_CASE( test_make_grammar ) {
    BOOST_TEST(grammar.size() == expected_rules.size());
    for (int i = 0, l = grammar.size(); i < l; i++) {

        BOOST_TEST(grammar[i].size() == expected_rules[i].size());
        for (int j = 0, m = grammar[i].size(); j < m; j++) {
            BOOST_TEST(grammar[i][j].type() == std::get<0>(expected_rules[i][j]));
            BOOST_TEST(grammar[i][j].nextState() == std::get<1>(expected_rules[i][j]));
        }

    }
}

BOOST_AUTO_TEST_CASE( test_analyze ) {
    // pre-test code
    auto tokens = ogla::basic_analyze(text.cbegin(), text.cend(), grammar);

    // run test
    BOOST_CHECK_MESSAGE(tokens.size() == expected_tokens.size(),
                        "token count: " << tokens.size() << ", expected: " << expected_tokens.size());
    for (int i = 0, s = tokens.size(); i < s; i++) {
        auto token = tokens.at(i);
        BOOST_CHECK_MESSAGE(token.type() == std::get<0>(expected_tokens[i]), MAKE_MESSAGE(token,(expected_tokens[i])));
        BOOST_CHECK_MESSAGE(token.lexeme() == std::get<1>(expected_tokens[i]), MAKE_MESSAGE(token,(expected_tokens[i])));
        BOOST_CHECK_MESSAGE(token.position() == std::get<2>(expected_tokens[i]), MAKE_MESSAGE(token,(expected_tokens[i])));
    }
}

BOOST_AUTO_TEST_CASE( test_BasicLexer ) {
    // pre-test code
    auto lexer = ogla::make_lexer(text.cbegin(), text.cend(), grammar);

    // run test
    for (int i = 0, s = expected_tokens.size(); i < s; i++) {
        auto token = lexer.current();
        BOOST_CHECK_MESSAGE(token.type() == std::get<0>(expected_tokens[i]), MAKE_MESSAGE(token,(expected_tokens[i])));
        BOOST_CHECK_MESSAGE(token.lexeme() == std::get<1>(expected_tokens[i]), MAKE_MESSAGE(token,(expected_tokens[i])));
        BOOST_CHECK_MESSAGE(token.position() == std::get<2>(expected_tokens[i]), MAKE_MESSAGE(token,(expected_tokens[i])));
        if (i < s - 1) {
            auto ptoken = lexer.peek();
            BOOST_CHECK_MESSAGE(ptoken.type() == std::get<0>(expected_tokens[i + 1]), MAKE_MESSAGE(ptoken,(expected_tokens[i + 1])));
            BOOST_CHECK_MESSAGE(ptoken.lexeme() == std::get<1>(expected_tokens[i + 1]), MAKE_MESSAGE(ptoken,(expected_tokens[i + 1])));
            BOOST_CHECK_MESSAGE(ptoken.position() == std::get<2>(expected_tokens[i + 1]), MAKE_MESSAGE(ptoken,(expected_tokens[i + 1])));
        }
        lexer.next();
    }
}
