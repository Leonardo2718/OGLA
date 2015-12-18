/*
Project: OGLA
File: grammar.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: December 18, 2015

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
#include <tuple>

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
    template<typename TokenType, typename charT>
    using BasicGrammarRule = BasicRule<TokenType, BasicGrammarIndex, charT>;
    template<typename TokenType, typename charT>
    using BasicGrammar = std::vector<std::vector<BasicGrammarRule<TokenType, charT>>>;

    template <typename TokenType>
    using SimpleBasicGrammarRule = BasicGrammarRule<TokenType, char>;

    template <typename TokenType>
    using SimpleBasicGrammar = BasicGrammar<TokenType,  char>;

    template <typename TokenType>
    using SimpleBasicToken = BasicToken<TokenType, std::string::const_iterator>;

    template <typename TokenType>
    using SimpleBasicTokenList = BasicTokenList<TokenType, std::string::const_iterator>;

//*
    template<typename BidirectionalIterator, typename TokenType> std::tuple<std::smatch, int>
    first_match(BidirectionalIterator first, BidirectionalIterator last, std::vector<SimpleBasicGrammarRule<TokenType>> ruleList);
//*/
    /*
    Given iterators to the start and one-past-the-end of a string, returns a `std::pair` containing the index of the
    rule that gets matched first (with resptect to position in the text) as well as the `std::smatch` itself.
    */

}   // namespace `ogla`



/*
Given iterators to the start and one-past-the-end of a string, returns a `std::pair` containing the index of the
rule that gets matched first (with resptect to position in the text) as well as the `std::smatch` itself.
*/
//*
template<typename BidirectionalIterator, typename TokenType> std::tuple<std::smatch, int>
ogla::first_match(BidirectionalIterator first, BidirectionalIterator last, std::vector<SimpleBasicGrammarRule<TokenType>> ruleList) {
    int ruleIndex = -1;
    std::smatch match;
    std::smatch tmpMatch;

    // look for the rule that has the first match (in terms of position in the text)
    for (int i = 0, count = ruleList.size(); i < count; ++i) {
        if (std::regex_search(first, last, tmpMatch, ruleList[i].regex())
                            && (ruleIndex < 0 || tmpMatch.position() < match.position())) {
            match = std::move(tmpMatch);
            ruleIndex = i;
        }
    }

    return make_tuple(match, ruleIndex);
}
//*/

#endif//OGLA_GRAMMAR_HPP
