#pragma once
#include "lexer.h"
#include<string>
#include<memory>
#include<vector>
#include<iostream>

enum logical_op {
    and_op, or_op
};
enum join_op {
    eq, le, leq, ge, geq
};
class Base_Ast {
public:
	virtual ~Base_Ast() = default;
};

class SelList_Ast : Base_Ast {
    std::vector<std::string> attributes;
public:
    SelList_Ast(std::vector<std::string> attributes);
};

class FromList_Ast : Base_Ast {
    std::vector<std::string> relations;
public:
    FromList_Ast(std::vector<std::string> relations);
};

class Condition_Ast : public Base_Ast {
    std::unique_ptr<Base_Ast> condition;
public:
    Condition_Ast(std::unique_ptr<Base_Ast> condition);
};
class ConditionalPredicate_Ast : public Base_Ast {
   logical_op op;
   std::unique_ptr<Condition_Ast> lhs;
   std::unique_ptr<Condition_Ast> rhs;
public:
    ConditionalPredicate_Ast(std::unique_ptr<Condition_Ast> lhs, std::unique_ptr<Condition_Ast> rhs, logical_op op);
};

class Query_Ast : Base_Ast{
    std::unique_ptr<SelList_Ast> selList;
    std::unique_ptr<FromList_Ast> fromList;
    std::unique_ptr<Condition_Ast> condition;
public:
    Query_Ast(std::unique_ptr<SelList_Ast> sl, std::unique_ptr<FromList_Ast> fl);
    Query_Ast(std::unique_ptr<SelList_Ast> sl, std::unique_ptr<FromList_Ast> fl, std::unique_ptr<Condition_Ast> c);
};
class ConditionalIn_Ast : public Base_Ast {
    std::string attribute;
    std::unique_ptr<Query_Ast> query;
public:
    ConditionalIn_Ast(std::string attr, std::unique_ptr<Query_Ast> query);
};

class ConditionalJoin_Ast : public Base_Ast {
    std::string attr1;
    std::string attr2;
    join_op op;
public:
    ConditionalJoin_Ast(std::string attr1, std::string attr2, join_op op);
};

class ConditionalLike_Ast : public Base_Ast {
    std::string attribute;
    std::string pattern;
public:
    ConditionalLike_Ast(std::string attribute, std::string pattern);
};



class Parser {
	std::unique_ptr<Lexer> lexer;
	std::ostream &logs = std::cout;
public:
	Parser(const std::string &src);
	Parser(const std::string &src, std::ostream &os);
	std::unique_ptr<Query_Ast> ParseQuery();
	std::unique_ptr<SelList_Ast> ParseSelList();
	std::unique_ptr<FromList_Ast> ParseFromList();
	std::unique_ptr<Condition_Ast> ParseCondition();
};

