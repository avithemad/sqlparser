/* Grammar for sql
 * selectQuery -> [SELECT] selectExp [FROM] tableExp filterExp 
 * selectExp -> [*] | columns // extend this to define other functions as well
 * columns -> column columns'
 * columns' -> [,]column columns' | empty
 * column -> [alphanum_value] | empty
 * tableExp -> table tables'
 * tables' -> [,]table tables' | empty
 * filterExp -> [WHERE] predicates | empty
 * predicates -> predicate pred'
 * pred' -> and predicates | or predicates | empty
 * predicate -> operand op operand
 * op -> >|>=|<|<=|=
 * operand -> identifier | constant
 * constant -> [0-9]+ | "string"
 *
 * Currently only supporting simple queries like
 * SELECT col1, col2 FROM table1, table2 WHERE table1.id = table2.id
 */
// if columns are prefixed with table name, use that to reference the table column, otherwise use the names of the column directly, same in case of where query
// Right now only support the CSV files table name is appended with csv file for parsing
// Backend should be pandas code generated.

#pragma once
#include "lexer.h"
#include<string>
#include<memory>
#include<vector>
#include<iostream>

enum predicate_op_enum {
	lt,leq, gt, geq, eq
};

class Base_Ast {
public:
	virtual ~Base_Ast() = default;
};

// TODO: need to handle UDFs and aggregations
class SelectExp_Ast : Base_Ast {
	std::vector<std::string> columns;
public:
	SelectExp_Ast(std::vector<std::string> cols);
};
// TODO: need to handle joins, and SelectQuery
class TableExp_Ast : Base_Ast {
	std::vector<std::string> tables;
public:
	TableExp_Ast(std::vector<std::string> tables);
};

// right now represent the operands as just strings, later create ast for these nodes.
class Predicate_Ast : Base_Ast {
	std::string lhs, rhs;
	predicate_op_enum op;
public:
	Predicate_Ast(const std::string &lhs, const std::string &rhs, predicate_op_enum op);	
};
class FilterExp_Ast : Base_Ast {
	std::vector<Predicate_Ast> predicates;
	std::vector<std::string> logicalOp;
public:
	FilterExp_Ast(std::vector<Predicate_Ast> predicates, std::vector<std::string> logicalOp);
};

class SelectQuery_Ast : Base_Ast {
	std::unique_ptr<SelectExp_Ast> selectExp;
	std::unique_ptr<TableExp_Ast> tableExp;
	std::unique_ptr<FilterExp_Ast> filterExp;
public:
	SelectQuery_Ast(std::unique_ptr<SelectExp_Ast> selectExp, std::unique_ptr<TableExp_Ast> tableExp);
	SelectQuery_Ast(std::unique_ptr<SelectExp_Ast> selectExp, std::unique_ptr<TableExp_Ast> tableExp, std::unique_ptr<FilterExp_Ast> FilterExp_Ast);
};

class Parser {
	std::unique_ptr<Lexer> lexer;
	std::ostream &logs = std::cout;
public:
	Parser(const std::string &src);
	Parser(const std::string &src, std::ostream &os);
	std::unique_ptr<SelectQuery_Ast> ParseSelectQuery();
	std::unique_ptr<SelectExp_Ast> ParseSelectExp();
	std::unique_ptr<TableExp_Ast> ParseTableExp();
	std::unique_ptr<FilterExp_Ast> ParseFilterExp();
	std::unique_ptr<Predicate_Ast> ParsePredicate();
};
