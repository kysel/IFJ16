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
#include "scanner.h"
#include <stdbool.h>

typedef struct Statement_s Statement;
typedef struct Statement_collection_s Statement_collection;
typedef Statement_collection Statement_block;

typedef int VariableId;
typedef struct Expression_s Expression;
typedef Expression Return_statement;
typedef struct Variable_s Variable;
typedef struct Func_parameter_s Func_parameter;
typedef struct Symbol_tree_s Symbol_tree;


typedef struct {
    Data_type type;
    union {
        bool b;
        int i;
        double d;
        char* s;
    };
    bool init;
} Value;

typedef struct {
    Value val;
    bool returned;
} Return_value;

typedef struct {
    Value* val;
    int size;
    int count;
} Value_list;

typedef Value (*BuildInPtr)(Value_list vals);

struct Statement_collection_s {
    Statement* statements;
    int count;
    int size;
};

typedef struct {
    Func_parameter* parameters;
    int count;
    int size;
} Parameter_list;

typedef struct Symbol_tree_leaf_s {
    const char* key;
    int id;
    Data_type type;
    Expression* init_expr;
    bool defined;
    struct Symbol_tree_leaf_s* left;
    struct Symbol_tree_leaf_s* right;
} Symbol_tree_leaf;

struct Symbol_tree_s {
    Symbol_tree_leaf* root;
    int nextId;
    bool inc;
};

typedef struct Function_s {
    char* name;
    enum {
        user,
        build_in
    } type;
    union {
        struct {
            Statement_collection statements;
            Parameter_list parameters;
            Symbol_tree local_symbols;
            int stack_size;
        };
        BuildInPtr build_in;
    };
    Data_type return_type;
} Function;

typedef struct {
    Function* items;
    int count;
    int size;
} Function_list;

typedef struct {
    Expression* expressions;
    int count;
    int size;
} Expression_list;

typedef struct Variable_s {
    int id;
    Expression* init_expr;
    Data_type type;
} Variable;

typedef struct {
    enum {
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
        OP_LOWER,
        OP_GREATER,
        OP_LOWER_EQUAL,
        OP_GREATER_EQUAL,
        OP_BOOL_EQUAL,
        OP_NOT_EQUAL
    } BinOp;
    Expression* left_expr;
    Expression* right_expr;
} BinOpTree;

typedef struct {
    Data_type type;
    union {
        long int li;
        double d;
        char* c;
    };
} Constant;

typedef struct {
    char* name;
    Parameter_list parameters;
} FunctionCall;

typedef struct Expression_s {
    enum {
        function_call,
        variable,
        constant,
        bin_op_tree
    } type;
    union {
        FunctionCall fCall;
        VariableId variable;
        Constant constant;
        BinOpTree tree;
    };
} Expression;

typedef struct Func_parameter_s {
    char* name;
    Data_type type;
    Expression value;
} Func_parameter;

typedef struct {
    Expression condition;
    Statement_block caseTrue;
    Statement_block caseFalse;
} If_statement;

typedef struct {
    VariableId target;
    Expression source;
} Assign_statement;

typedef struct {
    Expression condition;
    Statement_block statements;
} While_statement;

typedef struct {
    Variable variable;
} Declaration;

typedef struct Statement_s {
    enum {
        declaration,
        expression,
        condition,
        assigment,
        while_loop,
        Return
    } type;
    union {
        Declaration declaration;
        Expression expression;
        If_statement condition;
        Assign_statement assignment;
        While_statement while_loop;
        Return_statement ret;
    };
} Statement;
#endif
