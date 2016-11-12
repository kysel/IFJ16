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
    int size;
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
    int size;
}Expression_list;

typedef struct Variable_s {
    int id;
    Expression* init_expr;
    Data_type type;
}Variable;

/*typedef struct Class_s {
	char* Name;
}Class;*/

//TODO enum BinOP
//TODO struct BinOP tree (BinOP, left_expression, right_expression)
typedef struct {
    enum {
        OP_ADD, OP_SUB, OP_MUL, OP_DIV,
        OP_LOWER, OP_GREATER,
        OP_LOWER_EQUAL, OP_GREATER_EQUAL,
        OP_BOOL_EQUAL, OP_NOT_EQUAL
    }BinOp;

    Expression *left_expr;
    Expression *right_expr;
}BinOpTree;

typedef struct {
   Data_type type;
   union {
        long int li;
        double d;
        char *c;
   };
}Constant;

typedef struct {
    char* name;
    Parameter_list parameters;
}FunctionCall;

typedef  struct Expression_s{
	enum {
		function_call,
		variable,
      constant,
      bin_op_tree
	}type;
	union {
      Function function;
      VariableId variable;
      Constant constant;
      BinOpTree tree;

      //TODO universal data type container
	
      //TODO BinOP tree
	};
}Expression;

typedef struct Func_parameter_s {
    char* name;
    Data_type type;
    Expression value;
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
