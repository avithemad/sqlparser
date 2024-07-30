#include "parser.h"
#include "lexer.h"
#include<memory>

SelectExp_Ast::SelectExp_Ast(std::vector<std::string> cols) : columns(cols) {}
TableExp_Ast::TableExp_Ast(std::vector<std::string> tables) : tables(tables) {}

SelectQuery_Ast::SelectQuery_Ast(std::unique_ptr<SelectExp_Ast> selectExp, std::unique_ptr<TableExp_Ast> tableExp) :
    selectExp(std::move(selectExp)), tableExp(std::move(tableExp)) {}

Parser::Parser(const std::string &src) : lexer(std::make_unique<Lexer>(src)) {}

std::unique_ptr<SelectExp_Ast> Parser::ParseSelectExp() {
    Token t = lexer->peekToken();
    if (t.type != star_tok && t.type != identifier_tok) return nullptr;
    lexer->eatToken();
    if (t.type == star_tok) {
        return std::make_unique<SelectExp_Ast>(std::vector<std::string>(1, "*"));
    } else { // this is identifier token
        std::vector<std::string> v(1, t.lexeme);
        while(lexer->peekToken().type == comma_tok) {
            lexer->eatToken();
            if (lexer->peekToken().type!=identifier_tok) return nullptr;
            v.push_back(lexer->peekToken().lexeme);
            lexer->eatToken();
        }
        return std::make_unique<SelectExp_Ast>(v);
    }
}

std::unique_ptr<TableExp_Ast> Parser::ParseTableExp() {
    Token t = lexer->peekToken();
    if (t.type!=identifier_tok) return nullptr;
    std::vector<std::string> v(1, t.lexeme);
    lexer->eatToken();
    while(lexer->peekToken().type == comma_tok) {
        lexer->eatToken();
        if (lexer->peekToken().type!=identifier_tok) return nullptr;
        v.push_back(lexer->peekToken().lexeme);
        lexer->eatToken();
    }
    return std::make_unique<TableExp_Ast>(v);
}

std::unique_ptr<SelectQuery_Ast> Parser::ParseSelectQuery() {
    if (lexer->peekToken().type != select_tok) {
        return nullptr;
    }
    lexer->eatToken();
    auto selectExp = ParseSelectExp();
    if (selectExp == nullptr) return nullptr;
    if (lexer->peekToken().type != from_tok) {
        return nullptr;
    }
    lexer->eatToken();
    auto tableExp = ParseTableExp();
    if (tableExp == nullptr) return nullptr;
    return std::make_unique<SelectQuery_Ast>(std::move(selectExp), std::move(tableExp));
}
