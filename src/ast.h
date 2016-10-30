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
typedef struct Func_parameter_s Func_parameter;

/*typedef struct Class_s {
	char* Name;
}Class;*/

typedef enum {
	void_t,
	int_t,
	double_t,
	bool_t,
	string_t
}Data_type;

/**
 * Functions...
 */

typedef struct {
	Func_parameter* parameters;
	int count;
}Parameter_list;

typedef struct Function_s {
	enum {
		user,
		build_in
	}type;
	Parameter_list parameters;
	Data_type return_type;
}Function;

typedef struct {
	Function* items;
	int count;
}Function_list;



/**
 * Expressions...
 */
typedef  struct Expression_s{
	enum {
		function_call,
		variable
	}type;
	union {
		Function function;
		VariableId variable;
	};
}Expression;

typedef struct {
	Expression* expressions;
	int count;
}Expression_list;

typedef struct Func_parameter_s {
	char* name;
	Expression value;
}Func_parameter;



/**
 * Statements...
 */
struct Statement_collection_s {
	Statement* statements;
	int count;
};

typedef struct If_statement_s{
	Expression condition;
	Statement_block caseTrue;
	Statement_block caseFalse;
}If_statement;

typedef struct Assign_statement_s {
	VariableId target;
	Expression source;
}Assign_statement;

typedef struct While_statement_s {
	Expression condition;
	Statement_block statements;
}While_statement;

typedef struct Statement_s {
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
}Statement;
#endif
