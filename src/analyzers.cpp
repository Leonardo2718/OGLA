/*
Project: OGLA
File: analyzers.cpp
Author: Leonardo Banderali
Created: July 12, 2015
Last Modified: August 26, 2015

Description:
    A `StepAnalyzer` object is a special kind of token iterator.  It points to a token in the analyzed text.  However,
    the next token is only generated when the iterator is incremented.  This is done by analyzing the text on-the-fly.
    Internally, the iterator stores a vector of all the previously generated tokens, as to allow backward iteration.
    This can be useful for programs that require taking specific action when a token is identified
    (e.g. a prittyprinter).

Copyright (C) 2015 Leonardo Banderali
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

// library header
#include "analyzers.hpp"



//~function implementations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
returns a list of tokens representing `text` tokenized using `grammar`
*/
ogla::TokenList ogla::analyze(const std::string& text, const Grammar& grammar) {
    TokenList tl;   // token list to be returned

    //int p = 0;
    //auto rules = grammar.rule_list(0).lock();
    auto rules = grammar.rule_list(0);
    auto first = text.cbegin();
    auto last = text.cend();
    auto position = first;
    while(position < last) {
        //TokenRulePair trp = firstToken(text, *rules, p);   // should call move constructor
        Token token;
        Grammar::Rule rule{-1};
        std::smatch match;
        std::smatch m;
        for (auto r: rules) {
            if (std::regex_search(position, last, m, r.regex()) && (match.empty() || m.position() < match.position())) {
                //token = Token(r, m, offset);
                match = m;
                rule = r;
            }
        }

        //if (trp.token.position() < 0)
        if (match.empty())
            break;

        //p = trp.token.position() + trp.token.length();
        //tl.push_back(trp.token);
        tl.push_back(Token{rule.name(), match, position - first});
        position += match.position() + match.length();

        //rules = trp.rule.get_nextRules().lock();
        if (rule.get_nextRules() < 0)
            break;
        else
            rules = grammar.rule_list(rule.get_nextRules());
    }

    return tl;
}



//~`stepAnalyzer` class definition~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ogla::StepAnalyzer::StepAnalyzer(const std::string& _text, const Grammar& _grammar)
: grammar{_grammar}, text_begin{_text.cbegin()}, text_end{_text.cend()}, current_pos{_text.cbegin()}, currentRules(0) {
    operator++();
}

/*
sends the analyzer to `pos` in the text and sets the current rules to `rules`
*/
//void ogla::StepAnalyzer::jump_to(int pos, std::weak_ptr<const RuleList> rules) {
void ogla::StepAnalyzer::jump_to(int pos, Grammar::RulesListIndex rules) {
    //current_pos = pos;
    current_pos = text_begin + pos;
    currentRules = rules;
}

/*
sends the analyzer back to the start of the text
*/
void ogla::StepAnalyzer::reset() {
    //currentPair = TokenRulePair{};
    currentToken = Token{};
    currentRule = Grammar::Rule{-1};
    //current_pos = 0;
    current_pos = text_begin;
    //currentRules = grammar.rule_list(0);
    currentRules = 0;
}

/*
moves to and returns the next token
*/
//ogla::TokenRulePair ogla::StepAnalyzer::next() {
ogla::Token ogla::StepAnalyzer::next() {
    //current_pos = currentPair.token.position() + currentPair.token.length();
    current_pos = text_begin + currentToken.position() + currentToken.length();
    //currentPair = firstToken(text_begin, text_end, *currentRules.lock(), current_pos);
    auto rules = grammar.rule_list(currentRules);

    Token token;
    Grammar::Rule rule{-1};
    std::smatch match;
    std::smatch m;
    for (auto r: rules) {
        if (std::regex_search(current_pos, text_end, m, r.regex()) && (match.empty() || m.position() < match.position())) {
            //token = Token(r, m, offset);
            match = m;
            rule = r;
        }
    }
    if (match.empty())
        return currentToken;
    currentRule = rule;
    currentToken = Token{rule.name(), match, current_pos - text_begin};

    //currentRules = currentPair.rule.get_nextRules();
    //return currentPair;
    currentRules = rule.get_nextRules();
    return currentToken;
}


//ogla::TokenRulePair& ogla::StepAnalyzer::operator*() {
ogla::Token& ogla::StepAnalyzer::operator*() {
    //return currentPair;
    return currentToken;
}

//ogla::TokenRulePair* ogla::StepAnalyzer::operator->() {
ogla::Token* ogla::StepAnalyzer::operator->() {
    //return &currentPair;
    return &currentToken;
}

ogla::StepAnalyzer& ogla::StepAnalyzer::operator++() {
    /*current_pos = currentPair.token.position() + currentPair.token.length();
    currentPair = firstToken(text_begin, text_end, *currentRules.lock(), current_pos);
    currentRules = currentPair.rule.get_nextRules();*/
    //current_pos = currentPair.token.position() + currentPair.token.length();
    current_pos = text_begin + currentToken.position() + currentToken.length();
    //currentPair = firstToken(text_begin, text_end, *currentRules.lock(), current_pos);
    auto rules = grammar.rule_list(currentRules);

    Token token;
    Grammar::Rule rule{-1};
    std::smatch match;
    std::smatch m;
    for (auto r: rules) {
        if (std::regex_search(current_pos, text_end, m, r.regex()) && (match.empty() || m.position() < match.position())) {
            //token = Token(r, m, offset);
            match = m;
            rule = r;
        }
    }
    if (match.empty())
        return *this;
    currentRule = rule;
    currentToken = Token{rule.name(), match, current_pos - text_begin};

    //currentRules = currentPair.rule.get_nextRules();
    //return currentPair;
    currentRules = rule.get_nextRules();
    return *this;
}

ogla::StepAnalyzer& ogla::StepAnalyzer::operator++(int) {
    auto old = this;
    /*current_pos = currentPair.token.position() + currentPair.token.length();
    currentPair = firstToken(text_begin, text_end, *currentRules.lock(), current_pos);
    currentRules = currentPair.rule.get_nextRules();*/
    operator++();
    return *old;
}
