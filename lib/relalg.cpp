#include "relalg.h"
#include<iostream>

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

std::string LogicalPredicateExpr::print() {
    std::string l = lhs->print();
    std::string r = rhs->print();
    std::string res = "{ " + l + " & " + r  + " }";
    return res;
}
std::string ContainsPredicateExpr::print() {
    return "{ " + attribute + " in " + relation->print() + " }";
}
std::string RegexPredicateExpr::print() {
    return "";
}
std::string JoinPredicateExpr::print() {
    return attr1 + "=" + attr2;
}
std::string Relation::print() {
    return name;
}
std::string Selection_Op::print() {
    std::string pred = predicate->print();
    std::string op = operand->print();
    return "\\sigma_" + pred + "(" + op + ")"; 
}
std::string Projection_Op::print() {
    std::string attrs;
    int i = 0;
    for (auto e: attributes) {
        i++;
        if (i < attributes.size()) attrs += e + ", ";
        else attrs += e;
    }
    return "\\pi_{ " + attrs + " }(" + operand->print() + ")";
}
std::string Union_Op::print() {
    return "";
}
std::string Intersection_Op::print() {
    return "";
}
std::string Difference_Op::print() {
    return "";
}
std::string CartesianProduct_Op::print() {
    return relation1->print() + " X " + relation2->print();
}

std::string Join_Op::print() {
    return "";
}