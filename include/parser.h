/* Grammar for sql
 * selectQuery -> [SELECT] selectExp [FROM] tableExp filterExp
 * selectExp -> [*] | columns // extend this to define other functions as well
 * columns -> column columns'
 * columns' -> [,]column columns' | empty
 * column -> [alphanum_value] | empty
 * tableExp -> table tables'
 * tables' -> [,]table tables' | empty
 * filterExp -> [WHERE] expression
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

class SelectQuery_Ast : Base_Ast {
	std::unique_ptr<SelectExp_Ast> selectExp;
	std::unique_ptr<TableExp_Ast> tableExp;
public:
	SelectQuery_Ast(std::unique_ptr<SelectExp_Ast> selectExp, std::unique_ptr<TableExp_Ast> tableExp);
};

class Parser {
	std::unique_ptr<Lexer> lexer;
public:
	Parser(const std::string &src);
	std::unique_ptr<SelectQuery_Ast> ParseSelectQuery();
	std::unique_ptr<SelectExp_Ast> ParseSelectExp();
	std::unique_ptr<TableExp_Ast> ParseTableExp();
};
