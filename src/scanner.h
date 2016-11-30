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
#include <assert.h>
#include "hacks.h"


#define FOREACH_TOKEN(TOKEN)                              \
    TOKEN(T_ADD,              "'+'",         0x4000000)   \
    TOKEN(T_SUB,              "'-'",         0x2000000)   \
    TOKEN(T_MUL,              "'*'",         0x1000000)   \
    TOKEN(T_DIV,              "'/'",          0x800000)   \
    TOKEN(T_LOWER,            "'<'",          0x400000)   \
    TOKEN(T_GREATER,          "'>'",          0x200000)   \
    TOKEN(T_LOWER_EQUAL,      "'<='",         0x100000)   \
    TOKEN(T_GREATER_EQUAL,    "'>='",          0x80000)   \
    TOKEN(T_BOOL_EQUAL,       "'=='",          0x40000)   \
    TOKEN(T_NOT_EQUAL,        "'!='",          0x20000)   \
    TOKEN(T_BRACKET_LROUND,   "'('",           0x10000)   \
    TOKEN(T_BRACKET_RROUND,   "')'",            0x8000)   \
    TOKEN(T_ID,               "identifier",     0x4000)   \
    TOKEN(T_FULL_ID,          "class.id",       0x2000)   \
    TOKEN(T_KEYWORD,          "keyword",        0x1000)   \
    TOKEN(T_ASSIGN,           "assignment",      0x800)   \
    TOKEN(T_COMMA,            "','",             0x400)   \
    TOKEN(T_SEMICOLON,        "';'",             0x200)   \
    TOKEN(T_BRACKET_LSQUARE,  "'['",             0x100)   \
    TOKEN(T_BRACKET_RSQUARE,  "']'",              0x80)   \
    TOKEN(T_BRACKET_LCURLY,   "'{'",              0x40)   \
    TOKEN(T_BRACKET_RCURLY,   "'}'",              0x20)   \
    TOKEN(T_EOF,              "end of file",      0x10)   \
    TOKEN(T_INT,              "integer value",     0x8)   \
    TOKEN(T_DOUBLE,           "double value",      0x4)   \
    TOKEN(T_STRING,           "string value",      0x2)   \
    TOKEN(T_TYPE,             "type",              0x1)

typedef enum {
    FOREACH_TOKEN(GENERATE_ENUM)
} token_type;

static inline const char* token_to_string(token_type tok) {
    switch (tok) {
            FOREACH_TOKEN(GENERATE_CASE)
        default:
            return "";
    }
}

typedef enum {
    FSM_INIT,
    FSM_ID,
    FSM_FID,
    FSM_FID1,
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
    FSM_LOWER, // "<"
    FSM_GREATER, // ">"
    FSM_NOT, // "!"
    FSM_QUOTE, // """
    FSM_ESCAPE,
    FSM_ESCAPE_OCTAL_1,
    FSM_ESCAPE_OCTAL_2,
    FSM_DOUBLE,
    FSM_EXPONENT,
    FSM_EXPONENT_SIGN,
    FSM_EXPONENT_2,
    FSM_ZERO,
    FSM_OCTAL,
    FSM_BIN_1,
    FSM_HEX,
    FSM_HEX_1,
    FSM_BIN,
    FSM_NUM1,
    FSM_HEX_D,
    FSM_HEX_P,
    FSM_STRING,
    FSM_COMMENT_LINE,
    FSM_COMMENT_BLOCK,
    FSM_COMMENT_BLOCK_FIN,
} states;

#define FOREACH_KEYWORD(KEYWORD)                \
    KEYWORD(K_BREAK,     "break",       0x1)    \
    KEYWORD(K_CLASS,     "class",       0x2)    \
    KEYWORD(K_CONTINUE,  "continue",    0x4)    \
    KEYWORD(K_DO,        "do",          0x8)    \
    KEYWORD(K_ELSE,      "else",       0x10)    \
    KEYWORD(K_FALSE,     "false",      0x20)    \
    KEYWORD(K_FOR,       "for",        0x40)    \
    KEYWORD(K_IF,        "if",         0x80)    \
    KEYWORD(K_RETURN,    "return",    0x100)    \
    KEYWORD(K_STATIC,    "static",    0x200)    \
    KEYWORD(K_TRUE,      "true",      0x400)    \
    KEYWORD(K_WHILE,     "while",     0x800)

typedef enum {
    FOREACH_KEYWORD(GENERATE_ENUM)
} Keyword;

static inline const char* keyword_to_string(Keyword kw) {
    switch (kw) {
            FOREACH_KEYWORD(GENERATE_CASE)
        default:
            return "";
    }
}

typedef enum {
    void_t,
    int_t,
    double_t,
    bool_t,
    string_t
} Data_type;

typedef struct {
    token_type type;
    size_t tlen; //lenght of token string
    long long line; //line number
    char* c; //token string literally
    union {
        Data_type dtype; //data types eg.double,int..
        Keyword kw; //keywords eg.while,break..
    };
    union {
        long int li; //if string is num,int value of char*c
        double d; //if string is num,double value of char*c
    };
} Ttoken;


typedef struct {
    FILE* f;
    long long line; //save line number among calling get_token
    Ttoken* token;
} Tinit;

/**
 * \brief Initialize scanner structure
 * \param fp File handle of the source
 * \return scanner context
 */
Tinit* init_scanner(FILE* fp);

/**
 * \brief Return next token from input file. <B>It does NOT consume the token</B>
 * \param scanner_struct scanner context
 * \return next available token
 */
Ttoken* peek_token(Tinit* scanner_struct);

/**
 * \brief Consume next token from input file
 * \param scanner_struct scanner context
 * \return next available token
 */
Ttoken* get_token(Tinit* scanner_struct);

/**
 * \brief Check if next token match type, consume and returns it, otherwise exit with error.
 * \param scanner_struct scanner context
 * \param type Desired type of next token
 * \return next token
 */
Ttoken* check_and_get_token(Tinit* scanner_struct, token_type type);

/**
* \brief Check if next token match type and returns it, otherwise exit with error. Does <b>NOT</b> consume token.
* \param scanner_struct scanner context
* \param type Desired type of next token
* \return next token
*/
Ttoken* check_and_peek_token(Tinit* scanner_struct, token_type type);

/**
* \brief Check if next token keyword match type, consume and returns it, otherwise exit with error.
* \param scanner_struct scanner context
* \param type Desired type of next keyword
* \return next token
*/
Keyword check_and_get_keyword(Tinit* scanner_struct, Keyword type);

/**
* \brief Check if next token keyword match type and returns it, otherwise exit with error. Does <b>NOT</b> consume token.
* \param scanner_struct scanner context
* \param type Desired type of next keyword
* \return next token
*/
Keyword check_and_peek_keyword(Tinit* scanner_struct, Keyword type);
#endif
