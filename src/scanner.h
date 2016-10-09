/** Interpretr jazyka IFJ16
* @file scanner.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#ifndef SCANNER_H_
#define SCANNER_H_

typedef enum {
	/** 
	 * @todo přidat různé typy tokenů (identifikátor, typ, ...)
	 */
	 
	 // tokens
	 T_MUL,				// "*"
	 T_DIV,				// "/""
	 T_ADD,				// "+"
	 T_SUB,				// "-"
	 T_LOWER,			// "<"
	 T_GREATER,			// ">"
	 T_LOWER_EQUAL,		// "<="
	 T_GREATER_EQUAL,	// ">="
	 T_BOOL_EQUAL,		// "=="
	 T_NOT_EQUAL,		// "!="
	 T_ASSIGN,			// "="
	 T_COMMA,			// ","
	 // T_COLON			// ":"
	 T_SEMICOLON,		// ";"
	 T_BRACKET_LROUND,	// "("
	 T_BRACKET_RROUND,	// ")"
	 T_BRACKET_LSQUARE,	// "["
	 T_BRACKET_RSQUARE,	// "]"
	 T_BRACKET_LCURLY,	// "{"
	 T_BRACKET_RCURLY,	// "}"
	 T_ID, 				// identifier
	 T_EOF,				// end of file
	 T_INT, 			// integer
	 T_DOUBLE,			// double
	 T_STRING,			// string
	 T_COMMENT,			// comment

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


/*struct string {
	char* Value;
	int Allocated;
};*/

typedef enum {
	S_INIT,
	S_ID,
	S_NUM,
}states;

typedef struct {
	token_type type;
	int tok_len;
	union {
		int i;
		double d;
		char c;
		//bool b;
		//struct string *str;
	} data;
}token;



/**
 * \brief Return next token from input file. <B>It does NOT consume the token</B>
 * \return next available token
 * \todo nejspíš, tady bude argument něco jako *scanner context
 */
token peek_token();

/**
 * \brief Consume next token from input file
 * \return next available token
 * \todo nejspíš, tady bude argument něco jako *scanner context
 */
token get_token();

#endif
