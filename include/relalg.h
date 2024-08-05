#pragma once
#include <string>
#include <memory>
#include <vector>
class PredicateExpr {
public:
	virtual ~PredicateExpr() = default;
};
class Base_Op {
public:
	virtual ~Base_Op() = default;
};

class LogicalPredicateExpr: public PredicateExpr {
	int op;
	std::unique_ptr<PredicateExpr> lhs;
	std::unique_ptr<PredicateExpr> rhs;
public:
	LogicalPredicateExpr(int op, std::unique_ptr<PredicateExpr> lhs, std::unique_ptr<PredicateExpr> rhs);
};

class ContainsPredicateExpr: public PredicateExpr {
	std::string attribute;
	std::unique_ptr<Base_Op> relation;
public:
	ContainsPredicateExpr(std::string attr, std::unique_ptr<Base_Op> relation); 
};

class RegexPredicateExpr: public PredicateExpr {
	std::string attribute;
	std::string pattern;
public:
	RegexPredicateExpr(std::string attr, std::string pattern);
};

class JoinPredicateExpr: public PredicateExpr {
	std::string attr1;
	std::string attr2;
public:
	JoinPredicateExpr(std::string attr1, std::string attr2); 
};

class Relation : public Base_Op {
	std::string name;
public:
	Relation(std::string name);
};

class Selection_Op : public Base_Op {
	std::unique_ptr<Base_Op> operand;
	std::unique_ptr<PredicateExpr> predicate;
public:
	Selection_Op(std::unique_ptr<Base_Op> operand, std::unique_ptr<PredicateExpr> predicate);
};


class Projection_Op : public Base_Op {
	std::unique_ptr<Base_Op> operand;
	std::vector<std::string> attributes;
public:
	Projection_Op(std::unique_ptr<Base_Op> operand, std::vector<std::string> attrs);
};


class Union_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Union_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
};


class Intersection_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Intersection_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
};

class Difference_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Difference_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
};


class CartesianProduct_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	CartesianProduct_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
};


class Join_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Join_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
};
