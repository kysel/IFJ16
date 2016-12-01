/** Interpretr jazyka IFJ16
* @file expr_parser.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <stdlib.h>
#include <string.h>
#include "expr_parser.h"
#include "gc.h"
#include "ast.h"
#include "ial.h"
#include "return_codes.h"
#include "syntax_analysis.h"

#define stackEmpty(s) ((s)->top_element == -1)
#define stackFull(s) ((s)->top_element == (s)->stack_size - 1)
#define stackSetStopBit(s) ((s)->arr[(s)->top_token].stop_bit = 1)

//Inicializácia (prázdneho) zásobníka
void stackInit(t_Stack* s) {
    s->arr = NULL;
    s->stack_size = 0;
    s->top_element = -1;
    s->top_token = -1;
}

//Rozšírenie zásobníka o 5 prvkov
void stackExpand(t_Stack* s) {
    t_Element* p = gc_realloc(s->arr, (s->stack_size + 5) * sizeof(t_Element));

    s->arr = p;
    s->stack_size += 5;
}

//Vráti index terminálu, ktorý je najbližšie vrcholu zásobníku
int findTopTerminal(t_Stack* s) {
    for (int i = s->top_element; i >= 0; i--)
        if (s->arr[i].type == EOS || s->arr[i].type == TOKEN)
            return i;
    return -1;
}

void stackPop(t_Stack* s) {
    if (!stackEmpty(s)) {
        s->top_element -= 1;
        s->top_token = findTopTerminal(s);
    }
}

//Pridanie prvku na vrchol zásobníka
void stackPush(t_Stack* s, t_Element_Type type, void* address) {
    //Ak je zasobnik prázdny, je rozšírený
    if (stackFull(s)) {
        stackExpand(s);
    }

    s->top_element += 1;
    s->arr[s->top_element].type = type;
    s->arr[s->top_element].address = address;
    s->arr[s->top_element].stop_bit = 0;
    if (type == EOS || type == TOKEN)
        s->top_token = s->top_element;
}

void stackApplyRule(t_Stack* s, t_Expr_Parser_Init* symbol_tabs, long long line) {
    int rule_lenght;
    for (rule_lenght = 0; (rule_lenght < s->top_element) && !(s->arr[s->top_element - rule_lenght].stop_bit); rule_lenght++);

    Expression* expression;

    switch (rule_lenght) {
            //Ziadne pravidlo
        default:
            fprintf(stderr, "Syntax error on line %lld.\n", line);
            exit(syntactic_analysis_error);

            //Krátke pravidlo
        case 1:
            //Pravidlo E -> i
            if (s->arr[s->top_element].type == TOKEN) {
                expression = gc_alloc(sizeof(Expression));
                Ttoken* token = s->arr[s->top_element].address;
                Symbol_tree_leaf* leaf;
                char* full_name;

                switch (token->type) {
                    case T_ID:
                        //Vytvorenie plne kvalifikovaného identifikátora
                        full_name = gc_alloc(sizeof(char) * (strlen(token->c) + strlen(symbol_tabs->class_name) + 2));
                        full_name[0] = '\0';
                        strcat(full_name, symbol_tabs->class_name);
                        strcat(full_name, ".");
                        strcat(full_name, token->c);

                        expression->type = variable;
                        if ((leaf = get_symbol_by_key(symbol_tabs->local_tab, token->c)))
                            expression->variable = leaf->id;
                        else if ((leaf = get_symbol_by_key(symbol_tabs->global_tab, full_name)))
                            expression->variable = leaf->id;
                        else {
                            leaf = add_symbol(symbol_tabs->global_tab, full_name);
                            expression->variable = leaf->id;
                        }
                        break;

                    case T_FULL_ID:
                        expression->type = variable;
                        if ((leaf = get_symbol_by_key(symbol_tabs->global_tab, token->c)))
                            expression->variable = leaf->id;
                        else {
                            leaf = add_symbol(symbol_tabs->global_tab, token->c);
                            leaf->defined = false;
                            leaf->init_expr = NULL;
                            expression->variable = leaf->id;
                        }
                        break;

                    case T_INT:
                        expression->type = constant;
                        expression->constant.type = int_t;
                        expression->constant.li = token->li;
                        break;

                    case T_DOUBLE:
                        expression->type = constant;
                        expression->constant.type = double_t;
                        expression->constant.d = token->d;
                        break;

                    case T_STRING:
                        expression->type = constant;
                        expression->constant.type = string_t;
                        expression->constant.c = token->c;
                        break;

                    case T_BOOL:
                        expression->type = constant;
                        expression->constant.type = bool_t;
                        expression->constant.b = token->b;
                        break;

                    default:
                        fprintf(stderr, "Syntax error on line %lld.\n", line);
                        exit(syntactic_analysis_error);
                }
            } else {
                fprintf(stderr, "Syntax error on line %lld.\n", line);
                exit(syntactic_analysis_error);
            }
            stackPop(s);
            break;

            //Unáne pravidlá
        case 2:
            if (s->arr[s->top_element - 1].type == TOKEN && s->arr[s->top_element].type == EXPRESSION) {      
                Ttoken* token = s->arr[s->top_element - 1].address;
                expression = gc_alloc(sizeof(Expression));
                expression->type = op_tree;

                switch (token->type) {
                    case T_NOT:
                        expression->tree.Op = OP_NOT;
                        expression->tree.left_expr = s->arr[s->top_element].address;
                        expression->tree.right_expr = NULL;
                        break;
                    
                    default:
                        fprintf(stderr, "Syntax error on line %lld.\n", line);
                        exit(syntactic_analysis_error);
                }
            } else {
                fprintf(stderr, "Syntax error on line %lld.\n", line);
                exit(syntactic_analysis_error);
            }
            stackPop(s);
            stackPop(s);
            break;

            //Dlhé pravidlá
        case 3:
            if (s->arr[s->top_element - 2].type == TOKEN && s->arr[s->top_element - 1].type == EXPRESSION && s->arr[s->top_element].type == TOKEN) {
                Ttoken* left_token = s->arr[s->top_element - 2].address;
                Ttoken* right_token = s->arr[s->top_element].address;

                if (left_token->type == T_BRACKET_LROUND && right_token->type == T_BRACKET_RROUND) {
                    expression = s->arr[s->top_element - 1].address;
                } else {
                    fprintf(stderr, "Syntax error on line %lld.\n", line);
                    exit(syntactic_analysis_error);
                }

            }

            //Pravidlá E -> E _ E 
            else if (s->arr[s->top_element - 2].type == EXPRESSION && s->arr[s->top_element - 1].type == TOKEN && s->arr[s->top_element].type == EXPRESSION) {
                expression = gc_alloc(sizeof(Expression));
                Ttoken* token = s->arr[s->top_element - 1].address;

                switch (token->type) {
                    case T_ADD:
                        expression->tree.Op = OP_ADD;
                        break;

                    case T_SUB:
                        expression->tree.Op = OP_SUB;
                        break;

                    case T_MUL:
                        expression->tree.Op = OP_MUL;
                        break;

                    case T_DIV:
                        expression->tree.Op = OP_DIV;
                        break;

                    case T_LOWER:
                        expression->tree.Op = OP_LOWER;
                        break;

                    case T_GREATER:
                        expression->tree.Op = OP_GREATER;
                        break;

                    case T_LOWER_EQUAL:
                        expression->tree.Op = OP_LOWER_EQUAL;
                        break;

                    case T_GREATER_EQUAL:
                        expression->tree.Op = OP_GREATER_EQUAL;
                        break;

                    case T_BOOL_EQUAL:
                        expression->tree.Op = OP_BOOL_EQUAL;
                        break;

                    case T_NOT_EQUAL:
                        expression->tree.Op = OP_NOT_EQUAL;
                        break;

                    case T_LOGIC_AND:
                        expression->tree.Op = OP_AND;
                        break;

                    case T_LOGIC_OR:
                        expression->tree.Op = OP_OR;
                        break;

                    default:
                        fprintf(stderr, "Syntax error on line %lld.\n", line);
                        exit(syntactic_analysis_error);
                }
                expression->type = op_tree;
                expression->tree.left_expr = s->arr[s->top_element - 2].address;
                expression->tree.right_expr = s->arr[s->top_element].address;
            } else {
                fprintf(stderr, "Syntax error on line %lld.\n", line);
                exit(syntactic_analysis_error);
            }

            stackPop(s);
            stackPop(s);
            stackPop(s);
            break;
    }

    s->arr[s->top_element].stop_bit = 0;
    stackPush(s, EXPRESSION, expression);
}

void processFunCall(t_Stack* s, Tinit* scanner, t_Expr_Parser_Init* symbol_tabs, long long line) {
    int rule_lenght;
    for (rule_lenght = 0; (rule_lenght < s->top_element) && !(s->arr[s->top_element - rule_lenght].stop_bit); rule_lenght++);
    char* full_name;

    if (rule_lenght == 1 && s->arr[s->top_element].type == TOKEN) {
        Ttoken* token = s->arr[s->top_element].address;

        if (token->type == T_ID) {
            full_name = gc_alloc(sizeof(char) * (strlen(token->c) + strlen(symbol_tabs->class_name) + 2));
            full_name[0] = '\0';
            strcat(full_name, symbol_tabs->class_name);
            strcat(full_name, ".");
            strcat(full_name, token->c);

            stackPop(s);
        } else if (token->type == T_FULL_ID) {
            full_name = token->c;

            stackPop(s);
        } else {
            fprintf(stderr, "Syntax error on line %lld.\n", line);
            exit(syntactic_analysis_error);
        }

    } else {
        fprintf(stderr, "Syntax error on line %lld.\n", line);
        exit(syntactic_analysis_error);
    }

    s->arr[s->top_element].stop_bit = 0;
    stackPush(s, EXPRESSION, parse_f_call(symbol_tabs, scanner, full_name));
}

//
int terminal2TabIndex(void* terminal, long long line) {
    //Jediný terminál, ktorý má adresu nula je EOS - End of Stack - Dno zásobníka
    if (terminal == NULL)
        return 14;

    //Ak má terminál adresu rôznu od NULL, ide o token
    //Konverzia pointeru na terminál na pointer na token 
    Ttoken* token = terminal;
    switch (token->type) {
        case T_ADD: return 0;
        case T_SUB: return 1;
        case T_MUL: return 2;
        case T_DIV: return 3;

        case T_BOOL_EQUAL: return 4;
        case T_NOT_EQUAL: return 5;
        case T_LOWER: return 6;
        case T_GREATER: return 7;
        case T_LOWER_EQUAL: return 8;
        case T_GREATER_EQUAL: return 9;

        case T_NOT : return 10;
        case T_LOGIC_AND: return 11;
        case T_LOGIC_OR: return 12;

        //case : return 13;
        //case : return 14;
        //case : return 15;
        
        case T_BRACKET_LROUND: return 16;
        case T_BRACKET_RROUND: return 17;
        case T_ID: case T_FULL_ID: case T_INT: case T_DOUBLE: case T_STRING: case T_BOOL: return 18;
        case T_COMMA: case T_SEMICOLON: return 19;
        default:
            fprintf(stderr, "Syntax error on line %lld.\n", line);
            exit(syntactic_analysis_error);
    }
}

/***
 *          ____  ___    ____  _____ __________ 
 *         / __ \/   |  / __ \/ ___// ____/ __ \
 *        / /_/ / /| | / /_/ /\__ \/ __/ / /_/ /
 *       / ____/ ___ |/ _, _/___/ / /___/ _, _/ 
 *      /_/   /_/  |_/_/ |_|/____/_____/_/ |_|  
 * 
 */

void printStack(t_Stack* s) {
    for (int i = 0; i <= s->top_element; i++) {
        switch (s->arr[i].type) {
            case EOS: printf("%p: $", (void*)s);
                break;
            case EXPRESSION: printf("A");
                break;
            case TOKEN: printf("%s", ((Ttoken *)s->arr[i].address)->c);
                break;
        };
        if (s->arr[i].stop_bit)
            printf("<");
    }
    printf("\n");
}

t_Expr_Parser_Init* ExprParserInit(Symbol_tree* global_tab, Symbol_tree* local_tab, char* class_name) {
    t_Expr_Parser_Init* init_struct = gc_alloc(sizeof(t_Expr_Parser_Init));
    init_struct->global_tab = global_tab;
    init_struct->local_tab = local_tab;
    init_struct->class_name = class_name;
    return init_struct;
}

Expression* parseExpression(t_Expr_Parser_Init* symbol_tabs, Tinit* scanner) {
    t_Stack* stack = gc_alloc(sizeof(t_Stack));
    stackInit(stack);
    stackPush(stack, EOS, NULL);

    void* a = stack->arr[stack->top_token].address;
    Ttoken* b = peek_token(scanner);

    while (!(a == NULL && (b->type == T_SEMICOLON || b->type == T_COMMA || b->type == T_BRACKET_RROUND))) {
        switch (precedence_tab[terminal2TabIndex(a, b->line)][terminal2TabIndex(b, b->line)]) {
            case '=':
                stackPush(stack, TOKEN, b);
                get_token(scanner);
                b = peek_token(scanner);
                break;

            case '<':
                stackSetStopBit(stack);
                stackPush(stack, TOKEN, b);
                get_token(scanner);
                b = peek_token(scanner);
                break;

            case '>':
                stackApplyRule(stack, symbol_tabs, b->line);
                break;

            case 'F':
                processFunCall(stack, scanner, symbol_tabs, b->line);
                b = peek_token(scanner);
                break;

            default:
                fprintf(stderr, "Syntax error on line %lld.\n", b->line);
                exit(syntactic_analysis_error);
        }

#ifdef DEBUG
        printStack(stack);
#endif
        a = stack->arr[stack->top_token].address;
    }

    return stack->arr[stack->top_element].address;
}
