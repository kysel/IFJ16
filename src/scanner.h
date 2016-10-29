/** Interpretr jazyka IFJ16
* @file scanner.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/
#include <stdio.h>
#ifndef SCANNER_H_
#define SCANNER_H_

typedef enum {
	/** 
	 * @todo přidat různé typy tokenů (identifikátor, typ, ...)
	 */
	 
	 // START OF TOKENS WHICH CANNOT BE REORDERED 
	 T_ADD,				// "+"
	 T_SUB,				// "-"
	 T_MUL,				// "*"
	 T_DIV,				// "/""
	 T_LOWER,			// "<"
	 T_GREATER,			// ">"
	 T_LOWER_EQUAL,		// "<="
	 T_GREATER_EQUAL,	// ">="
	 T_BOOL_EQUAL,		// "=="
	 T_NOT_EQUAL,		// "!="
	 T_BRACKET_LROUND,	// "("
	 T_BRACKET_RROUND,	// ")"
	 T_ID, 				// identifier
	 T_DOT,				// "."
	 // END OF TOKENS WHICH CANNOT BE REORDERED 

	 T_ASSIGN,			// "="
	 T_COMMA,			// ","
	 // T_COLON			// ":"
	 T_SEMICOLON,		// ";"

	 T_BRACKET_LSQUARE,	// "["
	 T_BRACKET_RSQUARE,	// "]"
	 T_BRACKET_LCURLY,	// "{"
	 T_BRACKET_RCURLY,	// "}"
	 
	 T_EOF,				// end of file
	 T_INT, 			// integer
	 T_DOUBLE,			// double
	 T_STRING,			// string
	 //T_COMMENT,			// comment

	 // keywords
	 K_BOOL,			// "boolean"
	 K_BREAK,			// "break"
	 K_CLASS,			// "class"
	 K_CONTINUE,		// "continue"
	 K_DO,				// "do"
	 K_DOUBLE,			// "double"
	 K_ELSE,			// "else"
	 K_FALSE,			// "false"
	 K_FOR,				// "for
	 K_IF,				// "if"
	 K_INT,				// "int"
	 K_RETURN,			// "return"
	 K_STRING,			// "String"
	 K_STATIC,			// "static"
	 K_TRUE,			// "true"
	 K_VOID,			// "void"
	 K_WHILE,			// "while"



}token_type;


typedef enum {
	FSM_INIT,
	FSM_ID,
	//FSM_K_ID_1,
	//FSM_K_ID,
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
	//FSM_LOWER_EQUAL,		// "<="
	//FSM_GREATER_EQUAL,	// ">="
	//FSM_BOOL_EQUAL,		// "=="
	FSM_NOT,			// "!"
	//FSM_NOT_EQUAL,		// "!="
	//FSM_ASSIGN,			// "="
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
	//FSM_EOF,

}states;

typedef struct {
	token_type type;
	unsigned int tlen;
	long long line;
	long whence;
	union {
		long int li;	// stacilo by aj int?
		double d;
		char *c;
	};
}Ttoken;

typedef struct {
	FILE *f;
	long long line;
}Tinit;

/**
 * \brief Return next token from input file. <B>It does NOT consume the token</B>
 * \return next available token
 * \todo nejspíš, tady bude argument něco jako *scanner context
 */
//token peek_token();
void char_append(char *tmp_string, unsigned int *tmp_string_len, unsigned char c);
/**
 * \brief Consume next token from input file
 * \return next available token
 * \todo nejspíš, tady bude argument něco jako *scanner context
 */
char *is_keyword(char *tmp_string);

Tinit *init_scanner(FILE *fp);

Ttoken *peek_token(Tinit *scanner_struct);

Ttoken *get_token(Tinit *scanner_struct) ;

#endif
