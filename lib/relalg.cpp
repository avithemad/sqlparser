#include "relalg.h"

LogicalPredicateExpr::LogicalPredicateExpr(int op, std::unique_ptr<PredicateExpr> lhs, std::unique_ptr<PredicateExpr> rhs) :
    op(op), lhs(std::move(lhs)), rhs(std::move(rhs)){}
ContainsPredicateExpr::ContainsPredicateExpr(std::string attr, std::unique_ptr<Base_Op> relation) :
    attribute(attr), relation(std::move(relation)) {}
RegexPredicateExpr::RegexPredicateExpr(std::string attr, std::string pattern) : attribute(attr), pattern(pattern) {}
JoinPredicateExpr::JoinPredicateExpr(std::string a1, std::string a2) : attr1(a1), attr2(a2) {}
Relation::Relation(std::string name): name(name){}
Selection_Op::Selection_Op(std::unique_ptr<Base_Op> op, std::unique_ptr<PredicateExpr> predicate) :
    operand(std::move(op)), predicate(std::move(predicate)) {}

Projection_Op::Projection_Op(std::unique_ptr<Base_Op> operand, std::vector<std::string> attrs) :
    operand(std::move(operand)), attributes(attrs) {}


CartesianProduct_Op::CartesianProduct_Op(std::unique_ptr<Base_Op> relation1, std::unique_ptr<Base_Op> relation2) :
    relation1(std::move(relation1)), relation2(std::move(relation2)) {}