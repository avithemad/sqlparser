#include "parser.h"
#include "lexer.h"
#include<memory>
#include<iostream>


SelList_Ast::SelList_Ast(std::vector<std::string> attributes): attributes(attributes){}
FromList_Ast::FromList_Ast(std::vector<std::string> relations): relations(relations){}
ConditionalPredicate_Ast::ConditionalPredicate_Ast(std::unique_ptr<Condition_Ast> lhs, std::unique_ptr<Condition_Ast> rhs, logical_op op)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op) {}
ConditionalIn_Ast::ConditionalIn_Ast(std::string attr, std::unique_ptr<Query_Ast> query) : attribute(attr), query(std::move(query)) {}

ConditionalJoin_Ast::ConditionalJoin_Ast(std::string attr1, std::string attr2, join_op op) :
    attr1(attr1), attr2(attr2), op(op) {}


ConditionalLike_Ast::ConditionalLike_Ast(std::string attribute, std::string pattern) : attribute(attribute), pattern(pattern) {}


Condition_Ast::Condition_Ast(std::unique_ptr<Base_Ast> condition) : condition(std::move(condition)) {}


Query_Ast::Query_Ast(std::unique_ptr<SelList_Ast> sl, std::unique_ptr<FromList_Ast> fl, std::unique_ptr<Condition_Ast> c)
    : selList(std::move(sl)), fromList(std::move(fl)), condition(std::move(c)) {}

Query_Ast::Query_Ast(std::unique_ptr<SelList_Ast> sl, std::unique_ptr<FromList_Ast> fl)
    : selList(std::move(sl)), fromList(std::move(fl)) {}

std::unique_ptr<SelList_Ast> Parser::ParseSelList() {
    std::cout << "Parsing select list\n";
    Token t = lexer->peekToken();
    if (t.type!=identifier_tok) return nullptr;
    lexer->eatToken();
    std::vector<std::string> attributes;
    attributes.push_back(t.lexeme);
    t = lexer->peekToken();
    while(t.type == comma_tok) {
        lexer->eatToken();
        if (lexer->peekToken().type != identifier_tok) return nullptr;
        attributes.push_back(lexer->peekToken().lexeme);
        lexer->eatToken();
        t = lexer->peekToken();
    }
    return std::make_unique<SelList_Ast>(attributes);
}
std::unique_ptr<FromList_Ast> Parser::ParseFromList() {
    std::cout << "Parsing from list\n";
    Token t = lexer->peekToken();
    if (t.type!=identifier_tok) return nullptr;
    std::vector<std::string> attributes;
    attributes.push_back(t.lexeme);
    lexer->eatToken();
    t = lexer->peekToken();
    while(t.type == comma_tok) {
        lexer->eatToken();
        if (lexer->peekToken().type != identifier_tok) return nullptr;
        attributes.push_back(lexer->peekToken().lexeme);
        lexer->eatToken();
        t = lexer->peekToken();
    }
    return std::make_unique<FromList_Ast>(attributes);
}
std::unique_ptr<Condition_Ast> Parser::ParseCondition() {
    std::cout << "Parsing condition\n";
    Token t = lexer->peekToken();
    if (t.type != identifier_tok) return nullptr;
    lexer->eatToken();
    std::string attr1 = t.lexeme;
    std::unique_ptr<Base_Ast> result;
    t = lexer->peekToken();
    if (t.type == in_tok) {
        lexer->eatToken();
        auto query = ParseQuery();
        if (query == nullptr) return nullptr;
        result = std::make_unique<ConditionalIn_Ast>(attr1, std::move(query));
    } else if (t.type == eq_tok) {
        lexer->eatToken();
        if (lexer->peekToken().type != identifier_tok) return nullptr;
        std::string attr2 = lexer->peekToken().lexeme;
        lexer->eatToken();
        result = std::make_unique<ConditionalJoin_Ast>(attr1, attr2, eq);
    } else if (t.type == like_tok) {
        lexer->eatToken();
        if (lexer->peekToken().type != identifier_tok) return nullptr;
        std::string pattern = lexer->peekToken().lexeme;
        lexer->eatToken();
        result = std::make_unique<ConditionalLike_Ast>(attr1, pattern);
    }
    std::unique_ptr<Condition_Ast> lhs = std::make_unique<Condition_Ast>(std::move(result));
    if (lexer->peekToken().type == and_tok) {
        lexer->eatToken();
        auto rhs = ParseCondition();
        if (rhs == nullptr) return nullptr;
        auto res = std::make_unique<ConditionalPredicate_Ast>(std::move(lhs), std::move(rhs), and_op);
        return std::make_unique<Condition_Ast>(std::move(res));
    }
    return lhs;
}
std::unique_ptr<Query_Ast> Parser::ParseQuery() {
    std::cout << "Parsing query\n";
    Token t = lexer->peekToken();
    if (t.type!=select_tok) return nullptr;
    lexer->eatToken();
    auto selList = ParseSelList();
    if (selList == nullptr) return nullptr;
    t = lexer->peekToken();
    if (t.type != from_tok) return nullptr;
    lexer->eatToken();
    auto fromList = ParseFromList();
    if (fromList == nullptr) return nullptr;
    t = lexer->peekToken();
    if (t.type != where_tok) return std::make_unique<Query_Ast>(std::move(selList), std::move(fromList));
    lexer->eatToken();
    auto cond = ParseCondition();
    if (cond == nullptr) return nullptr;
    return std::make_unique<Query_Ast>(std::move(selList), std::move(fromList), std::move(cond));
}

Parser::Parser(const std::string &src) : lexer(std::make_unique<Lexer>(src)) {}
Parser::Parser(const std::string &src, std::ostream &os) : lexer(std::make_unique<Lexer>(src))
    , logs(os) {}