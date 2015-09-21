/*
Project: OGLA
File: grammar.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: September 20, 2015

Description:
    A `Grammar` is a set of tokenization rules that collectively define a "language".  A grammar can be used to analyze
    text.  Once analyzed, the text is broken up into identified tokens.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_GRAMMAR_HPP
#define OGLA_GRAMMAR_HPP

#include "token.hpp"

// c++ standard libraries
//#include <type_traits>
#include <vector>

namespace ogla {

    /*#################################################################################################################
    ### Here, a `BasicGrammar` is defined as a list of tokenization rule lists (a matrix of rules).  Each list in    ##
    ### the grammar containes the rules that can be used to find any token allowed by the language in a given state. ##
    ### In this sense, the state of a lexer (being an FSM) can be simply represented by an index to a rule list in   ##
    ### the `BasicGrammar`.                                                                                          ##
    ###                                                                                                              ##
    ### From this definition, we know that the "next state" property of a rule is just an index; an integer.  So,    ##
    ### we create a type alias for `int` to be our `BasicGrammar` index and another alias for a `BasicRule` which    ##
    ### uses this index as its `LexerState` type.                                                                    ##
    #################################################################################################################*/

    using BasicGrammarIndex = int;
    template<typename TokenType>
    using BasicGrammarRule = BasicRule<TokenType, BasicGrammarIndex>;
    template<typename TokenType>
    using BasicGrammar = std::vector<std::vector<BasicGrammarRule<TokenType>>>;

}   // namespace `ogla`

#endif//OGLA_GRAMMAR_HPP
