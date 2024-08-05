#pragma once
#include <string>
#include <memory>
#include <vector>
class PredicateExpr {
public:
	virtual ~PredicateExpr() = default;
	virtual std::string print() = 0;
};
class Base_Op {
public:
	virtual ~Base_Op() = default;
	virtual std::string print() = 0;
};

class LogicalPredicateExpr: public PredicateExpr {
	int op;
	std::unique_ptr<PredicateExpr> lhs;
	std::unique_ptr<PredicateExpr> rhs;
public:
	LogicalPredicateExpr(int op, std::unique_ptr<PredicateExpr> lhs, std::unique_ptr<PredicateExpr> rhs);
	std::string print();
};

class ContainsPredicateExpr: public PredicateExpr {
	std::string attribute;
	std::unique_ptr<Base_Op> relation;
public:
	ContainsPredicateExpr(std::string attr, std::unique_ptr<Base_Op> relation); 
	std::string print();
};

class RegexPredicateExpr: public PredicateExpr {
	std::string attribute;
	std::string pattern;
public:
	RegexPredicateExpr(std::string attr, std::string pattern);
	std::string print();
};

class JoinPredicateExpr: public PredicateExpr {
	std::string attr1;
	std::string attr2;
public:
	JoinPredicateExpr(std::string attr1, std::string attr2); 
	std::string print();
};

class Relation : public Base_Op {
	std::string name;
public:
	Relation(std::string name);
	std::string print();
};

class Selection_Op : public Base_Op {
	std::unique_ptr<Base_Op> operand;
	std::unique_ptr<PredicateExpr> predicate;
public:
	Selection_Op(std::unique_ptr<Base_Op> operand, std::unique_ptr<PredicateExpr> predicate);
	std::string print();
};


class Projection_Op : public Base_Op {
	std::unique_ptr<Base_Op> operand;
	std::vector<std::string> attributes;
public:
	Projection_Op(std::unique_ptr<Base_Op> operand, std::vector<std::string> attrs);
	std::string print();
};


class Union_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Union_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string print();
};


class Intersection_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Intersection_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string print();
};

class Difference_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Difference_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string print();
};


class CartesianProduct_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	CartesianProduct_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string print();
};


class Join_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Join_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string print();
};
