#ifndef SCANNER_H_
#define SCANNER_H_

typedef enum {
	/** 
	 * @todo přidat různé typy tokenů (identifikátor, typ, ...)
	 */
}token_type;

typedef struct {
	token_type type;
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
