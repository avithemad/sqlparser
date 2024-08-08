#pragma once
#include <string>
#include <memory>
#include <vector>
class PredicateExpr {
public:
	virtual ~PredicateExpr() = default;
	virtual std::string print() = 0;
	virtual std::string codeGen() = 0;
	virtual std::string loopGen() = 0;
};
class Base_Op {
public:
	virtual ~Base_Op() = default;
	virtual std::string print() = 0;
	virtual std::string codeGen() = 0;
	virtual std::string loopGen() = 0;
};

class LogicalPredicateExpr: public PredicateExpr {
	int op;
	std::unique_ptr<PredicateExpr> lhs;
	std::unique_ptr<PredicateExpr> rhs;
public:
	LogicalPredicateExpr(int op, std::unique_ptr<PredicateExpr> lhs, std::unique_ptr<PredicateExpr> rhs);
	std::string print();
	std::string codeGen();
	std::string loopGen();
};

class ContainsPredicateExpr: public PredicateExpr {
	std::string attribute;
	std::unique_ptr<Base_Op> relation;
public:
	ContainsPredicateExpr(std::string attr, std::unique_ptr<Base_Op> relation); 
	std::string print();
	std::string codeGen();
	std::string loopGen();
};

class RegexPredicateExpr: public PredicateExpr {
	std::string attribute;
	std::string pattern;
public:
	RegexPredicateExpr(std::string attr, std::string pattern);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};

class JoinPredicateExpr: public PredicateExpr {
	std::string attr1;
	std::string attr2;
public:
	JoinPredicateExpr(std::string attr1, std::string attr2); 
	std::string codeGen();
	std::string print();
	std::string loopGen();
};

// this is actually equivalent to table scan operation
class Relation : public Base_Op {
	std::string name;
public:
	Relation(std::string name);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};

class Selection_Op : public Base_Op {
	std::unique_ptr<Base_Op> operand;
	std::unique_ptr<PredicateExpr> predicate;
public:
	Selection_Op(std::unique_ptr<Base_Op> operand, std::unique_ptr<PredicateExpr> predicate);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};


class Projection_Op : public Base_Op {
	std::unique_ptr<Base_Op> operand;
	std::vector<std::string> attributes;
public:
	Projection_Op(std::unique_ptr<Base_Op> operand, std::vector<std::string> attrs);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};


class Union_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Union_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};


class Intersection_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Intersection_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};

class Difference_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Difference_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};


class CartesianProduct_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	CartesianProduct_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};


class Join_Op : public Base_Op {
	std::unique_ptr<Base_Op> relation1;
	std::unique_ptr<Base_Op> relation2;
public:
	Join_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2);
	std::string codeGen();
	std::string print();
	std::string loopGen();
};
