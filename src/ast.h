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

typedef struct Statement_s Statement;
typedef struct Statement_collection_s Statement_collection;
typedef Statement_collection Statement_block;

typedef int VariableId;
typedef struct Expression_s Expression;
typedef Expression Return_statement;
typedef struct Variable_s Variable;
typedef struct Func_parameter_s Func_parameter;

typedef enum {
	void_t,
	int_t,
	double_t,
	bool_t,
	string_t
}Data_type;

struct Statement_collection_s {
	Statement* statements;
	int count;
	int size;
};

typedef struct {
	Func_parameter* parameters;
	int count;
}Parameter_list;

typedef struct Function_s {
    char* name;
	enum {
		user,
		build_in
	}type;
	union {
		struct {
			Statement_collection statements;
			Parameter_list parameters;
		};
		//buildIn funkce
	};
	Data_type return_type;
}Function;

typedef struct {
	Function* items;
	int count;
}Function_list;

typedef struct {
	Expression* expressions;
	int count;
}Expression_list;

typedef struct Variable_s {
	int id;
	Expression* init_expr;
	Data_type type;
}Variable;

/*typedef struct Class_s {
	char* Name;
}Class;*/

typedef struct Expression_s{
	enum {
		function_call,
		variable
	}type;
	union {
		Function function;
		VariableId variable;
	};
}Expression;

typedef struct Func_parameter_s {
	char* name;
	Data_type type;
	//Expression value;
}Func_parameter;

typedef struct {
	Expression condition;
	Statement_block caseTrue;
	Statement_block caseFalse;
}If_statement;

typedef struct {
	VariableId target;
	Expression source;
}Assign_statement;

typedef struct {
	Expression condition;
	Statement_block statements;
}While_statement;

typedef struct {
	Variable variable;
}Declaration;

typedef struct Statement_s {
	enum {
		declaration,
		expression,
		condition,
		assigment,
		while_loop,
		Return
	}type;

	union {
		Declaration declaration;
		Expression expression;
		If_statement condition;
		Assign_statement assignment;
		While_statement while_loop;
		//for? aka cycles extension
		Return_statement ret;
	};
}Statement;
#endif
