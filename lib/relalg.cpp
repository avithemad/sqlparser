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

//TODO: update the print and code gen with actual operation 
std::string LogicalPredicateExpr::print() {
    std::string l = lhs->print();
    std::string r = rhs->print();
    std::string res = "{ " + l + " & " + r  + " }"; // TODO: update hardcoded operation
    return res;
}
std::string LogicalPredicateExpr::codeGen() {
    std::string l = lhs->print();
    std::string r = rhs->print();
    std::string res = "{ " + l + " & " + r  + " }";
    return res;
}
std::string LogicalPredicateExpr::loopGen() {
    return "";
}
std::string ContainsPredicateExpr::print() {
    return "{ " + attribute + " in " + relation->print() + " }";
}
std::string ContainsPredicateExpr::codeGen() {
    return "{ " + attribute + " in " + relation->print() + " }";
}
std::string ContainsPredicateExpr::loopGen() {
    return "";
}
std::string RegexPredicateExpr::print() {
    return "";
}
std::string RegexPredicateExpr::codeGen() {
    return "";
}
std::string RegexPredicateExpr::loopGen() {
    return "";
}
std::string JoinPredicateExpr::print() {
    return attr1 + "=" + attr2;
}
std::string JoinPredicateExpr::codeGen() {
    return attr1 + "=" + attr2;
}
std::string JoinPredicateExpr::loopGen() {
    return "";
}
std::string Relation::print() {
    return name;
}
std::string Relation::codeGen() {
    return "scan(" + name + ")";
}
std::string Relation::loopGen() {
    std::string res = "for tuple in " + name + ": ";
    return res;
}
std::string Selection_Op::print() {
    std::string pred = predicate->print();
    std::string op = operand->print();
    return "\\sigma_" + pred + "(" + op + ")"; 
}
std::string Selection_Op::loopGen() {
    std::string res = operand->loopGen();
    res += "\n\t";
    res += "if " + predicate->loopGen(); 
    res += "\n\t";
    return res;
}
std::string Selection_Op::codeGen() {
    return operand->codeGen() + ".map(" + predicate->codeGen() + ")";
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
std::string Projection_Op::loopGen() {
    std::string res = operand->loopGen();
    res += "\n\t";
    for (auto e: attributes) {
        res += e;
        res += "\n\t";
    }
    return res;
}
std::string listToString(std::vector<std::string> v) {
    std::string res;
    int i=0;
    for (auto e: v) {
        i++;
        if (i == v.size()) res += e;
        else res += e + ", ";
    }
    return res;
}
std::string Projection_Op::codeGen() {
    return operand->codeGen() + ".project([" + listToString(attributes) + "])";
}
std::string Union_Op::print() {
    return "";
}
std::string Union_Op::codeGen() {
    return "";
}
std::string Union_Op::loopGen() {
    return "";
}
std::string Intersection_Op::print() {
    return "";
}
std::string Intersection_Op::codeGen() {
    return "";
}
std::string Intersection_Op::loopGen() {
    return "";
}
std::string Difference_Op::print() {
    return "";
}
std::string Difference_Op::codeGen() {
    return "";
}
std::string Difference_Op::loopGen() {
    return "";
}
std::string CartesianProduct_Op::print() {
    return relation1->print() + " X " + relation2->print();
}

std::string CartesianProduct_Op::codeGen() {
    return relation1->codeGen() + ".crossProduct(" + relation2->codeGen() + ")";
}
std::string CartesianProduct_Op::loopGen() {
    std::string res = relation1->loopGen();
    res += relation2->loopGen();
    return res;
}
std::string Join_Op::print() {
    return "";
}
std::string Join_Op::codeGen() {
    return "";
}
std::string Join_Op::loopGen() {
    return "";
}