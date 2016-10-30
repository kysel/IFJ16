/** Interpretr jazyka IFJ16
* @file scanner.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "hacks.h"
#include "ast.h"


#define FOREACH_TOKEN(Token)                              \
    Token(T_ADD,              "'+'")                      \
    Token(T_SUB,              "'-'")                      \
    Token(T_MUL,              "'*'")                      \
    Token(T_DIV,              "'/'")                      \
    Token(T_LOWER,            "'<'")                      \
    Token(T_GREATER,          "'>'")                      \
    Token(T_LOWER_EQUAL,      "'<='")                     \
    Token(T_GREATER_EQUAL,    "'>='")                     \
    Token(T_BOOL_EQUAL,       "'=='")                     \
    Token(T_NOT_EQUAL,        "'!='")                     \
    Token(T_BRACKET_LROUND,   "'('",           0x10000)   \
    Token(T_BRACKET_RROUND,   "')'",            0x8000)   \
    Token(T_ID,               "identifier",     0x4000)   \
    Token(T_DOT,              "'.'",            0x2000)   \
    Token(T_KEYWORD,          "keyword",        0x1000)   \
    Token(T_ASSIGN,           "assignment",      0x800)   \
    Token(T_COMMA,            "','",             0x400)   \
    Token(T_SEMICOLON,        "';'",             0x200)   \
    Token(T_BRACKET_LSQUARE,  "'['",             0x100)   \
    Token(T_BRACKET_RSQUARE,  "']'",              0x80)   \
    Token(T_BRACKET_LCURLY,   "'{'",              0x40)   \
    Token(T_BRACKET_RCURLY,   "'}'",              0x20)   \
    Token(T_EOF,              "end of file",      0x10)   \
    Token(T_INT,              "integer value",     0x8)   \
    Token(T_DOUBLE,           "double value",      0x4)   \
    Token(T_STRING,           "string value",      0x2)   \
    Token(T_TYPE,             "type",              0x1)

typedef enum {
    FOREACH_TOKEN(GENERATE_ENUM)
}token_type;

inline const char* token_to_string(token_type tok){
    switch (tok) {
        FOREACH_TOKEN(GENERATE_CASE)
    }
    assert(false);
    return "▲";
}

typedef enum {
	FSM_INIT,
	FSM_ID,
	FSM_INT,
	FSM_MUL,
	FSM_DIV,
	FSM_ADD,
	FSM_SUB,
	FSM_SEMICOLON,
	FSM_EQUAL,
	FSM_BRACKET_LROUND,
	FSM_BRACKET_RROUND,
	FSM_BRACKET_LSQUARE,
	FSM_BRACKET_RSQUARE,
	FSM_BRACKET_LCURLY,
	FSM_BRACKET_RCURLY,
	FSM_COMMA,
	FSM_LOWER,			// "<"
	FSM_GREATER,			// ">"
	FSM_NOT,			// "!"
	FSM_QUOTE,
	FSM_ESCAPE,
	FSM_ESCAPE_OCTAL_1,
	FSM_ESCAPE_OCTAL_2,
	FSM_DOUBLE,			// double
	FSM_EXPONENT,
	FSM_EXPONENT_SIGN,
	FSM_EXPONENT_2,
	FSM_STRING,			// string
	FSM_COMMENT_LINE,			// comment
	FSM_COMMENT_BLOCK,
	FSM_COMMENT_BLOCK_FIN,
	FSM_DOT, // '.'
}states;

#define FOREACH_KEYWORD(Keyword)                \
    Keyword(K_BREAK,     "break",       0x1)    \
    Keyword(K_CLASS,     "class",       0x2)    \
    Keyword(K_CONTINUE,  "continue",    0x4)    \
    Keyword(K_DO,        "do",          0x8)    \
    Keyword(K_ELSE,      "else",       0x10)    \
    Keyword(K_FALSE,     "false",      0x20)    \
    Keyword(K_FOR,       "for",        0x40)    \
    Keyword(K_IF,        "if",         0x80)    \
    Keyword(K_RETURN,    "return",    0x100)    \
    Keyword(K_STATIC,    "static",    0x200)    \
    Keyword(K_TRUE,      "true",      0x400)    \
    Keyword(K_WHILE,     "while",     0x800)    

typedef enum {
    FOREACH_KEYWORD(GENERATE_ENUM)
}Keyword;

inline const char* keyword_to_string(Keyword kw) {
    switch (kw) {
        FOREACH_TOKEN(GENERATE_CASE)
    }
    assert(false);
    return "▲";
}

typedef struct {
	token_type type;
	size_t tlen;
	long long line;
	long whence;
	union {
        Data_type dtype; //data types
        //operators op; //operators
        Keyword kw; //keywords
    };
    union {
        long int li;
        double d;
        char *c;
	};
}Ttoken;




typedef struct {
	FILE *f;
	long long line;
	Ttoken *token;
}Tinit;

/**
 * \brief Initialize scanner structure
 * \param fp File handle of the source
 * \return scanner context
 */
Tinit *init_scanner(FILE *fp);

/**
 * \brief Return next token from input file. <B>It does NOT consume the token</B>
 * \param scanner_struct scanner context
 * \return next available token
 */
Ttoken *peek_token(Tinit *scanner_struct);

/**
 * \brief Consume next token from input file
 * \param scanner_struct scanner context
 * \return next available token
 */
Ttoken *get_token(Tinit *scanner_struct);

#endif
