/*
Project: OGLA
File: grammar.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: July 10, 2015

Description:
    A `Grammar` is a set of `ogla::Rule`s that collectively define a "language".  This grammar can be used to analyze
    text.  Once analyzed, text is broken up into identified tokens.  For the sake of making the algorithms generic, if
    a text item is not identified as a token, it is simply discarded.

    A `StepAnalyzer` object is a special kind of
    token iterator.  It points to a token in the analyzed text.  However, the next token is only generated when the
    iterator is incremented.  This is done by analyzing the text on-the-fly.  Internally, the iterator stores a vector
    of all the previously generated tokens, as to allow backward iteration.  This can be useful for programs that
    require taking specific action when a token is identified (e.g. a prittyprinter).

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_GRAMMAR_HPP
#define OGLA_GRAMMAR_HPP

#include "token.hpp"

// c++ standard libraries
#include <type_traits>

namespace ogla {

//~forward declarations and function prototypes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Grammar;
class StepAnalyzer;

TokenList analyze(const std::string& text, const Grammar& grammar);
/*  returns a list of tokens representing `text` tokenized using `grammar` */

StepAnalyzer make_StepAnalyzer(const std::string& text, const Grammar& grammar);
/*  return a `StepAnalyzer` for analyzing the `text` using the given `grammar` */



//~classes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A class representing a lexcical analyses grammar.

All tokenization rules are held in a `std::vector` of `RuleList`s.  The first `RuleList` is the main/starting tokenization
rule list used when analyzing text.  The other tokenization rules are those pointed to by the `nextRules` member of the
matched tokenization rule.
*/
class Grammar {
    public:
        // static functions

        static Grammar load();
        /*  loads the default grammar used to load other grammars */

        static Grammar load(std::istream& input);
        /*  loads the language grammar defined by the content in the `input` stream */

        static Grammar load(const std::string& filePath);
        /*  loads the language grammar defined in the file with path `filePath` */

        // getters and setters

        std::string name() {
            return langName;
        }

        bool operator==(const Grammar& other);

        bool operator!=(const Grammar& other);

    // friends:
    friend TokenList analyze(const std::string& text, const Grammar& grammar);
    friend class StepAnalyzer;

    protected:

    private:
        //std::string grammerFilePath;// holds the path to the grammar file
        std::string langName;       // stores the name of the current language grammer
        std::vector<std::shared_ptr<RuleList>> rules; // holds all tokenization rules
};

/*
A class representing a token iterator where each token is generated on-the-fly by analyzing the given text.
*/
class StepAnalyzer {
    public:
        StepAnalyzer();

        TokenRulePair& operator*() {
            return currentToken;
        }

        TokenRulePair& operator->() {
            return currentToken;
        }

        StepAnalyzer& operator++() {
            current_itr += currentToken.token.length();
            currentToken = firstToken(text_begin, text_end, *currentRules, current_itr - text_begin);
            currentRules = currentToken.rule.get_nextRules().lock();
            return *this;
        }

        StepAnalyzer& operator++(int) {
            auto old = this;
            current_itr += currentToken.token.length();
            currentToken = firstToken(text_begin, text_end, *currentRules, current_itr - text_begin);
            currentRules = currentToken.rule.get_nextRules().lock();
            return *old;
        }

        bool operator==(const StepAnalyzer& other) {
            return text_begin == other.text_begin &&
                   text_end == other.text_end &&
                   current_itr == other.current_itr &&
                   //currentToken == other.currentToken &&
                   grammar == other.grammar;
        }

        bool operator!=(const StepAnalyzer& other) {
            return !(*this == other);
        }

    //friends:
        friend StepAnalyzer make_StepAnalyzer(const std::string& text, const Grammar& grammar);

    private:
        using text_itr = decltype(std::declval<std::string>().cbegin());

        StepAnalyzer(const std::string& _text, const Grammar& _grammar)
        : grammar{_grammar}, text_begin{_text.cbegin()}, text_end{_text.cend()}, current_itr{_text.cbegin()},
        currentRules{_grammar.rules.at(0)} {}

        Grammar grammar;

        const text_itr text_begin;
        const text_itr text_end;
        text_itr current_itr;

        TokenRulePair currentToken;
        std::shared_ptr<RuleList> currentRules;
};

}   // `ogla` namespace

#endif//OGLA_GRAMMAR_HPP
