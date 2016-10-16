/** Interpretr jazyka IFJ16
* @file ast.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef AST_H_
#define AST_H_

typedef int VariableId;

typedef struct Statement_s Statement;
typedef struct Statement_collection_s Statement_collection;
typedef Statement_collection Statement_block;

typedef struct Expression_s Expression;
typedef Expression Return_statement;
typedef struct If_statement_s If_statement;
typedef struct Assign_statement_s Assign_statement;
typedef struct While_statement_s While_statement;

typedef struct {
	char* Name;
}Class;


/**
 * Functions...
 */
typedef struct {
	char* name;
	Expression value;
}Func_parameter;

typedef struct {
	Func_parameter* parameters;
	int count;
}Parameter_list;

typedef struct {
	enum {
		user,
		build_in
	}type;
	Parameter_list parameters;
	//todo return type
}Function;


/**
 * Expressions...
 */
typedef struct Expression_s{
	enum {
		function_call
	}type;
	union {
		Function function;
	};
}Expression;

typedef struct {
	Expression* expressions;
	int count;
}Expression_collection;


/**
 * Statements...
 */
struct Statement_s {
	enum {
		expression,
		condition,
		assigment,
		while_loop,
		Return
	}type;

	union {
		Expression expression;
		If_statement condition;
		Assign_statement assignment;
		While_statement while_loop;
		//for? aka cycles extension
		Return_statement ret;
	};
};

struct Statement_collection_s {
	Statement* statements;
	int count;
};

struct If_statement_s{
	Expression condition;
	Statement_block caseTrue;
	Statement_block caseFalse;
};

struct Assign_statement_s {
	VariableId target;
	Expression source;
};

struct While_statement_s {
	Expression condition;
	Statement_block statements;
};

#endif
