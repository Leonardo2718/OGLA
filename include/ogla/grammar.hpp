/*
Project: OGLA
File: grammar.hpp
Author: Leonardo Banderali
Created: July 7, 2015
Last Modified: July 7, 2015

Description:
    A `Grammar` is a set of `ogla::Rule`s that collectively define a "language".  This grammar can be used to analyze
    text.  Once analyzed, text is broken up into identified tokens.  For the sake of making the algorithms generic, if
    a text item is not identified as a token, it is simply discarded.

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#ifndef OGLA_GRAMMAR_HPP
#define OGLA_GRAMMAR_HPP

#include "token.hpp"

namespace ogla {

//~forward declarations and function prototypes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Grammar;

TokenList analyze(const std::string& text, const Grammar& grammar);
/*  returns a list of tokens representing `text` tokenized using `grammar` */



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

    // friends:
    friend TokenList analyze(const std::string& text, const Grammar& grammar);

    protected:

    private:
        //std::string grammerFilePath;// holds the path to the grammar file
        std::string langName;       // stores the name of the current language grammer
        std::vector<std::shared_ptr<RuleList>> rules; // holds all tokenization rules
};

}   // `ogla` namespace

#endif//OGLA_GRAMMAR_HPP
