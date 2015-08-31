/*
Project: OGLA
File: grammar.hpp
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

#ifndef OGLA_GRAMMAR_HPP
#define OGLA_GRAMMAR_HPP

#include "token.hpp"

// c++ standard libraries
//#include <type_traits>
#include <vector>

namespace ogla {

    /*#################################################################################################################
    ### Here, a `Grammar` is defined as a list of tokenization rule lists (a matrix of rules).  Each list in the     ##
    ### Grammar containes the rules that can be used to find any token allowed by the language in a given state.  In ##
    ### this sense, the state of a lexer (being an FSM) can be simply represented by an index to a rule list in the  ##
    ### `Grammar` (which is, once again, a matrix of rules).                                                         ##
    ###                                                                                                              ##
    ### From this definition, we know that the "next state" property of a rule is just and index; an integer.  So,   ##
    ### we create a type alias for `int` to be our `Grammar` index and another alias for a `BasicRule` which uses    ##
    ### this index as its `LexerStateType`.                                                                          ##
    #################################################################################################################*/

    using GrammarIndex = int;
    using Rule = BasicRule<GrammarIndex>;
    using Grammar = std::vector<std::vector<Rule>>;

    Grammar load();
    /*  loads a default/test Grammar */

}   // namespace `ogla`

#endif//OGLA_GRAMMAR_HPP
