#include "lexer.h"
#include "util.h"
#include <vector>
#include<iostream>

Lexer::Lexer(const std::string &src) : src(src), src_idx(0) {
    eatToken();
}

Token Lexer::peekToken() {
    return cur_tok;
}
bool ispredToken(std::string lex) {
    std::vector<std::string> v = {"<", ">", "<=", ">=", "="};
    for (auto e: v) {
        if (stricmp(e, lex)) return true;
    }
    return false;
}

void Lexer::eatToken() {
    if(src_idx >= src.size()) {
        cur_tok = {eof_tok, ""};
        return;
    }
    // skip blank spaces
    while(isspace(src[src_idx])) {
        if(src_idx == src.size()) {
            cur_tok = {eof_tok, ""};
            return;
        }
        src_idx++;
    } 

    // now parse the tokens
    if (isalnum(src[src_idx])) {
        std::string lex;
        while(isalnum(src[src_idx])) {
            lex.push_back(src[src_idx++]);
        }
        if (stricmp("select", lex)) {
            cur_tok = {select_tok, lex};
        } else if (stricmp("from", lex)) {
            cur_tok = {from_tok, lex};
        } else if (stricmp("where", lex)) {
            cur_tok = {where_tok, lex};
        } else if (stricmp("and", lex)) {
            cur_tok = {and_tok, lex};
        } else if (stricmp("or", lex)) {
            cur_tok = {or_tok, lex};
        } else if (stricmp("in", lex)) {
            cur_tok = {in_tok, lex};
        } else if (stricmp("like", lex)) {
            cur_tok = {like_tok, lex};
        } else {
            cur_tok = {identifier_tok, lex};
        }
    } else if (src[src_idx] == '*') {
        cur_tok = {star_tok, "*"};
        src_idx++;
    } else if (src[src_idx] == ',') {
        cur_tok = {comma_tok, ","};
        src_idx++;
    } else if (src[src_idx] == '=') {
        cur_tok = {eq_tok, "="};
        src_idx++;
    } else {
        std::string lex;
        lex.push_back(src[src_idx++]);
        if (src_idx < src.size() && src[src_idx] == '=') lex.push_back(src[src_idx++]);
        if (ispredToken(lex)) {
            cur_tok = {pred_tok, lex};
        } else {
            cur_tok = {invalid_tok, lex};
        }
    }
}
int Lexer::getSrcIdx() {
    return src_idx;
}

std::string Lexer::printDebug() {
    std::string res;
    res += src;
    res += '\n';
    for (int i=0; i<src_idx-1; i++) res += ' ';
    res += "^\n";
    return res;
}