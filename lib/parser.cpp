#include "parser.h"
#include "lexer.h"
#include<memory>
#include<iostream>

SelectExp_Ast::SelectExp_Ast(std::vector<std::string> cols) : columns(cols) {}
TableExp_Ast::TableExp_Ast(std::vector<std::string> tables) : tables(tables) {}

SelectQuery_Ast::SelectQuery_Ast(std::unique_ptr<SelectExp_Ast> selectExp, std::unique_ptr<TableExp_Ast> tableExp) :
    selectExp(std::move(selectExp)), tableExp(std::move(tableExp)) {}

SelectQuery_Ast::SelectQuery_Ast(std::unique_ptr<SelectExp_Ast> selectExp, std::unique_ptr<TableExp_Ast> tableExp,
        std::unique_ptr<FilterExp_Ast> filterExp) :
    selectExp(std::move(selectExp)), tableExp(std::move(tableExp)), filterExp(std::move(filterExp)) {}

FilterExp_Ast::FilterExp_Ast(
    std::vector<Predicate_Ast> ps, 
    std::vector<std::string> logicalOp) : logicalOp(logicalOp), predicates(ps)
{}
Predicate_Ast::Predicate_Ast(const std::string &lhs, const std::string &rhs, predicate_op_enum op):
    lhs(lhs), rhs(rhs), op(op) {}


Parser::Parser(const std::string &src) : lexer(std::make_unique<Lexer>(src)) {}
Parser::Parser(const std::string &src, std::ostream &os) : lexer(std::make_unique<Lexer>(src))
    , logs(os) {}

std::unique_ptr<SelectExp_Ast> Parser::ParseSelectExp() {
    Token t = lexer->peekToken();
    if (t.type != star_tok && t.type != identifier_tok) {
        logs << "Failed parsing select expression. Invalid list of columns\n";
        logs << lexer->printDebug();
        return nullptr;
    } 
    lexer->eatToken();
    if (t.type == star_tok) {
        return std::make_unique<SelectExp_Ast>(std::vector<std::string>(1, "*"));
    } else { // this is identifier token
        std::vector<std::string> v(1, t.lexeme);
        while(lexer->peekToken().type == comma_tok) {
            lexer->eatToken();
            if (lexer->peekToken().type!=identifier_tok) {
                logs << "Failed parsing select expression. Expected another column name after comma\n";
                logs << lexer->printDebug();
                return nullptr;
            }
            v.push_back(lexer->peekToken().lexeme);
            lexer->eatToken();
        }
        return std::make_unique<SelectExp_Ast>(v);
    }
}

std::unique_ptr<TableExp_Ast> Parser::ParseTableExp() {
    Token t = lexer->peekToken();
    if (t.type!=identifier_tok) {
        logs << "Failed parsing table expression. Expected a table name for table expression.\n";
        logs << lexer->printDebug();
        return nullptr;
    }
    std::vector<std::string> v(1, t.lexeme);
    lexer->eatToken();
    while(lexer->peekToken().type == comma_tok) {
        lexer->eatToken();
        if (lexer->peekToken().type!=identifier_tok) {
            logs << "Failed parsing table expression. Expected a table name after comma\n";
            logs << lexer->printDebug();
            return nullptr;
        } 
        v.push_back(lexer->peekToken().lexeme);
        lexer->eatToken();
    }
    return std::make_unique<TableExp_Ast>(v);
}
std::unique_ptr<Predicate_Ast> Parser::ParsePredicate() {
    Token t = lexer->peekToken();
    if (t.type!=identifier_tok) {
        logs << "Failed parsing WHERE clause. Expected an identifier\n";
        logs << lexer->printDebug();
        return nullptr;
    }
    std::string lhs = t.lexeme;
    lexer->eatToken();

    t = lexer->peekToken();
    if (t.type!=pred_tok) {
        logs << "Failed parsing WHERE clause. Expected a comparison operator <, <=, >, >=, =\n";
        logs << lexer->printDebug();
        return nullptr;
    }
    predicate_op_enum op;
    if (t.lexeme == "<") op = lt;
    else if (t.lexeme == "<=") op = leq;
    else if (t.lexeme == ">=") op = geq;
    else if (t.lexeme == ">") op = gt;
    else if (t.lexeme == "=") op = eq;
    lexer->eatToken();

    t = lexer->peekToken();
    if (t.type!=identifier_tok) {
        logs << "Failed parsing WHERE clause. Expected rhs after comparison operator\n";
        logs << lexer->printDebug();
        return nullptr;
    } 
    std::string rhs = t.lexeme;
    lexer->eatToken();
    return std::make_unique<Predicate_Ast>(lhs, rhs, op);
}
std::unique_ptr<FilterExp_Ast> Parser::ParseFilterExp() {
    // first parse the predicate
    std::vector<Predicate_Ast> preds;
    std::vector<std::string> logicalOps;

    do {
        Token t = lexer->peekToken();
        if (t.type == and_tok || t.type == or_tok) {
            logicalOps.push_back(t.lexeme);
            lexer->eatToken();
        }
        auto pred = ParsePredicate();
        if (pred == nullptr) {
            return nullptr;
        }
        preds.push_back(*pred.get());

            // check if you see an and/or token
    } while(lexer->peekToken().type == and_tok || lexer->peekToken().type == or_tok);

    return std::make_unique<FilterExp_Ast>(preds, logicalOps);
}
std::unique_ptr<SelectQuery_Ast> Parser::ParseSelectQuery() {
    if (lexer->peekToken().type != select_tok) {
        return nullptr;
    }
    lexer->eatToken();
    auto selectExp = ParseSelectExp();
    if (selectExp == nullptr) {
        return nullptr;
    }
    if (lexer->peekToken().type != from_tok) {
        logs << "Failed parsing SELECT query. FROM clause is missing\n";
        logs << lexer->printDebug();
        return nullptr;
    }
    lexer->eatToken();
    auto tableExp = ParseTableExp();
    if (tableExp == nullptr) return nullptr;
    if (lexer->peekToken().type != where_tok) 
        return std::make_unique<SelectQuery_Ast>(std::move(selectExp), std::move(tableExp));
    // now parse the filterExp
    lexer->eatToken();
    auto filterExp = ParseFilterExp();
    if (filterExp == nullptr) return nullptr;
    return std::make_unique<SelectQuery_Ast>(std::move(selectExp), std::move(tableExp), std::move(filterExp));
}
