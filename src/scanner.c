/** Interpretr jazyka IFJ16
* @file scanner.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "gc.h"

char *keywords[17] = { "boolean", "break", "class", "continue","do",
					"double", "else", "false", "for", "if", "int",
					"return", "String", "static", "true", "void", "while" };

Tinit *init_scanner(FILE *fp) {
	long long n = 1;
	Tinit* scanner_struct = gc_alloc(sizeof(Tinit));
	scanner_struct->f = fp;
	scanner_struct->line = n;
	scanner_struct->token = NULL;
	return scanner_struct;
}

char *char_append(char *tmp_string, unsigned int *tmp_string_len, unsigned char c) {
	(*tmp_string_len)++;
	tmp_string = (char *)gc_realloc(tmp_string, sizeof(char)*(*tmp_string_len));

	tmp_string[(*tmp_string_len - 2)] = c;
	tmp_string[*tmp_string_len - 1] = '\0';

	return tmp_string;
}

char *is_keyword(char *tmp_string) {
	for (int i = 0; i <= 16; i++) {
		if (!strcmp(tmp_string, keywords[i])) {
			return keywords[i];
		}
	}
	return NULL;
}

Ttoken *peek_token(Tinit *scanner_struct) {
	if (scanner_struct->token == NULL)
		scanner_struct->token = get_token(scanner_struct);
	return scanner_struct->token;
}

Ttoken *get_token_internal(Tinit *scanner_struct);

Ttoken *get_token(Tinit *scanner_struct) {
	Ttoken* ret;
	if (scanner_struct->token != NULL) {
		ret = scanner_struct->token;
		scanner_struct->token = NULL;
	}
	else
		ret = get_token_internal(scanner_struct);
	return ret;
}

Ttoken *get_token_internal(Tinit *scanner_struct) {
    char *kw_ptr;
    char *endptr;
    char c;
    int read_file = 1;
    unsigned int tmp_string_len = 1;

    states state = FSM_INIT;
    char *tmp_string = (char *)gc_alloc(sizeof(char));
    tmp_string[0] = 0;

    Ttoken *token = (Ttoken *)gc_alloc(sizeof(Ttoken));

    token->whence = ftell(scanner_struct->f);

    do {
        c = fgetc(scanner_struct->f);

        switch (state) {
        case FSM_INIT:
            if (isspace(c)) {
                state = FSM_INIT;
                //if (c != EOF) { // este porozmyslaj
                if (c == '\n') {
                    scanner_struct->line++;
                }
                //}
                continue;
            }
            else if ((isalpha(c)) || (c == '_') || (c == '$')) state = FSM_ID;
            else if (isdigit(c))  state = FSM_INT;
            else if (c == '*') state = FSM_MUL;
            else if (c == '/') state = FSM_DIV;
            else if (c == '+') state = FSM_ADD;
            else if (c == '-') state = FSM_SUB;
            else if (c == ',') state = FSM_COMMA;
            else if (c == ';') state = FSM_SEMICOLON;
            else if (c == '(') state = FSM_BRACKET_LROUND;
            else if (c == ')') state = FSM_BRACKET_RROUND;
            else if (c == '[') state = FSM_BRACKET_LSQUARE;
            else if (c == ']') state = FSM_BRACKET_RSQUARE;
            else if (c == '{') state = FSM_BRACKET_LCURLY;
            else if (c == '}') state = FSM_BRACKET_RCURLY;
            else if (c == '=') state = FSM_EQUAL;
            else if (c == '!') state = FSM_NOT;
            else if (c == '<') state = FSM_LOWER;
            else if (c == '>') state = FSM_GREATER;
            else if (c == '"') state = FSM_QUOTE;
            else if (c == '.') state = FSM_DOT;
            else if (c == EOF) {
                read_file = 0;
                continue;
            }
            else {
                fprintf(stderr, "Unidentified lexem!");
                exit(1);
            }

            tmp_string = char_append(tmp_string, &tmp_string_len, c);
            break;

        case FSM_ID:
            if (isalpha(c) || isdigit(c) || c == '_' || c == '$') {
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
                state = FSM_ID;
            }
            else {
                ungetc(c, scanner_struct->f);
                kw_ptr = (is_keyword(tmp_string));
                if (kw_ptr == NULL) {
                    token->type = T_ID;
                    token->tlen = strlen(tmp_string);
                    token->line = scanner_struct->line;
                    token->c = tmp_string;
                    return token;
                }
                if (strcmp(kw_ptr, "boolean") == 0) {
                    token->type = T_TYPE;
                    token->dtype = bool_t;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "break") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_BREAK;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "class") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_CLASS;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "continue") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_CONTINUE;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "do") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_DO;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "double") == 0) {
                    token->type = T_TYPE;
                    token->dtype = double_t;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "else") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_ELSE;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "false") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_FALSE;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "for") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_FOR;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "if") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_IF;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "int") == 0) {
                    token->type = T_TYPE;
                    token->dtype = int_t;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "return") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_RETURN;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "String") == 0) {
                    token->type = T_TYPE;
                    token->dtype = string_t;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "static") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_STATIC;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "true") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_TRUE;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "void") == 0) {
                    token->type = T_TYPE;
                    token->dtype = void_t;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
                if (strcmp(kw_ptr, "while") == 0) {
                    token->type = T_KEYWORD;
                    token->kw = K_WHILE;
                    token->tlen = strlen(kw_ptr);
                    token->line = scanner_struct->line;
                    token->c = kw_ptr;
                    return token;
                }
            }
            break;

        case FSM_DOT:
            ungetc(c, scanner_struct->f);
            token->type = T_DOT;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_INT:
            if (isdigit(c)) {
                state = FSM_INT;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else if (c == '.') {
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
                state = FSM_DOUBLE;
            }
            else if (c == 'E' || c == 'e') {
                state = FSM_EXPONENT;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                ungetc(c, scanner_struct->f);
                token->type = T_INT;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->li = strtol(tmp_string, &endptr, 10);
                return token;
            }

            break;

        case FSM_DOUBLE:
            if (isdigit(c)) {
                state = FSM_DOUBLE;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else if (c == 'E' || c == 'e') {
                state = FSM_EXPONENT;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                ungetc(c, scanner_struct->f);
                token->type = T_DOUBLE;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->d = strtod(tmp_string, &endptr);
                return token;
            }

            break;

        case FSM_EXPONENT:
            if (c == '+' || c == '-') {
                state = FSM_EXPONENT_SIGN;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else if (isdigit(c)) {
                state = FSM_EXPONENT_2;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                fprintf(stderr, "SCANNER ERROR: Exponent error!\n");
                exit(1);
            }

            break;

        case FSM_EXPONENT_SIGN:
            if (isdigit(c)) {
                state = FSM_EXPONENT_2;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                fprintf(stderr, "SCANNER ERROR: Exponent sign error!\n");
                exit(1);
            }

            break;

        case FSM_EXPONENT_2:
            if (isdigit(c)) {
                state = FSM_EXPONENT_2;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                ungetc(c, scanner_struct->f);
                token->type = T_DOUBLE;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->d = strtod(tmp_string, &endptr);
                return token;
            }

            break;

        case FSM_MUL:
            ungetc(c, scanner_struct->f);
            token->type = T_MUL;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_DIV:
            if (c == '/')
                state = FSM_COMMENT_LINE;
            else if (c == '*')
                state = FSM_COMMENT_BLOCK;
            else {
                ungetc(c, scanner_struct->f);
                //tmp_string[1] = '\0';
                token->type = T_DIV;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->c = tmp_string;
                return token;
            }
            break;

        case FSM_ADD:
            ungetc(c, scanner_struct->f);
            token->type = T_ADD;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_SUB:
            ungetc(c, scanner_struct->f);
            token->type = T_SUB;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_COMMA:
            ungetc(c, scanner_struct->f);
            token->type = T_COMMA;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_SEMICOLON:
            ungetc(c, scanner_struct->f);
            token->type = T_SEMICOLON;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_BRACKET_LROUND:
            ungetc(c, scanner_struct->f);
            token->type = T_BRACKET_LROUND;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_BRACKET_RROUND:
            ungetc(c, scanner_struct->f);
            token->type = T_BRACKET_RROUND;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_BRACKET_LSQUARE:
            ungetc(c, scanner_struct->f);
            token->type = T_BRACKET_LSQUARE;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_BRACKET_RSQUARE:
            ungetc(c, scanner_struct->f);
            token->type = T_BRACKET_RSQUARE;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_BRACKET_LCURLY:
            ungetc(c, scanner_struct->f);
            token->type = T_BRACKET_LCURLY;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_BRACKET_RCURLY:
            ungetc(c, scanner_struct->f);
            token->type = T_BRACKET_RCURLY;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

        case FSM_EQUAL: {
            if (c == '=') {
                token->type = T_BOOL_EQUAL;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->c = tmp_string;
                return token;
            }
            ungetc(c, scanner_struct->f);
            token->type = T_ASSIGN;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;
        }

        case FSM_NOT: {
            if (c == '=') {
                token->type = T_NOT_EQUAL;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->c = tmp_string;
                return token;
            }
            fprintf(stderr, "SCANNER ERROR: Unidentified lexem!\n");
            exit(1);
        }

        case FSM_LOWER: {
            if (c == '=') {
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
                token->type = T_LOWER_EQUAL;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->c = tmp_string;
                return token;
            }
            ungetc(c, scanner_struct->f);
            token->type = T_LOWER;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;
        }

        case FSM_GREATER: {
            if (c == '=') {
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
                token->type = T_GREATER_EQUAL;
                token->tlen = strlen(tmp_string);
                token->line = scanner_struct->line;
                token->c = tmp_string;
                return token;
            }
            ungetc(c, scanner_struct->f);
            token->type = T_GREATER;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;
        }

        case FSM_QUOTE:
            if (c == '\\') {
                state = FSM_ESCAPE;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else if (c == '"') {
                state = FSM_STRING;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else if (!isprint(c)) {
                fprintf(stderr, "SCANNER ERROR: Unidentified token!\n");
                exit(1);
            }
            else {
                state = FSM_QUOTE;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }

            break;

        case FSM_ESCAPE:
            if (c == '"' || c == 't' || c == 'n' || c == '\\') {
                state = FSM_QUOTE;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else if (c >= '0' && c <= '3') {
                state = FSM_ESCAPE_OCTAL_1;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                fprintf(stderr, "SCANNER ERROR: String escape sequence error!\n");
                exit(1);
            }
            break;

        case FSM_STRING:
            ungetc(c, scanner_struct->f);
            token->type = T_STRING;
            token->tlen = strlen(tmp_string);
            token->line = scanner_struct->line;
            token->c = tmp_string;
            return token;

            // start of auxiliary states for string escape octal

        case FSM_ESCAPE_OCTAL_1:
            if (c >= '0' && c <= '7') {
                state = FSM_ESCAPE_OCTAL_2;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                fprintf(stderr, "SCANNER ERROR: String escape sequence error!\n");
                exit(1);
            }
            break;

        case FSM_ESCAPE_OCTAL_2:
            if (c >= '0' && c <= '7') {
                state = FSM_QUOTE;
                tmp_string = (char_append(tmp_string, &tmp_string_len, c));
            }
            else {
                fprintf(stderr, "SCANNER ERROR: String escape sequence error!\n");
                exit(1);
            }
            break;


            // end of auxiliary states for string escape octal 

            // start of auxiliary states for comments

        case FSM_COMMENT_LINE:
            if (c != '\n') {
                state = FSM_COMMENT_LINE;
                if (c == EOF) {
                    ungetc(c, scanner_struct->f);
                    // domysli este, bude continue vhodne
                    state = FSM_INIT;
                }
            }
            else {
                memset(tmp_string, 0, (tmp_string_len) * sizeof(char));
                tmp_string_len = 1;
                /*if (c != EOF) { // este porozmyslaj
                        if(c == '\n') {
                        (scanner_struct->line)++;
                        printf("SOM V COMENTE: %lli\n", scanner_struct->line);
                        }
                } ak dam prec ungetc musim dat toto!*/
                ungetc(c, scanner_struct->f);
                state = FSM_INIT;
            }
            break;

        case FSM_COMMENT_BLOCK:
            if (c == '*') {
                state = FSM_COMMENT_BLOCK_FIN;
            }
            else if (c == EOF) {
                fprintf(stderr, "Unterminated comment\n");
                exit(1);
            }
            else {
                if (c == '\n') {
                    scanner_struct->line++;
                }
                state = FSM_COMMENT_BLOCK;
            }
            break;

        case FSM_COMMENT_BLOCK_FIN:
            if (c == '/') {
                memset(tmp_string, 0, (tmp_string_len) * sizeof(char));
                tmp_string_len = 1;
                state = FSM_INIT;
            }
            else if (c == '*') {
                state = FSM_COMMENT_BLOCK_FIN;
            }
            else if (c == EOF) {
                fprintf(stderr, "Unterminated comment\n");
                exit(1);
            }
            else {
                if (c == '\n') {
                    scanner_struct->line++;
                }
                state = FSM_COMMENT_BLOCK;
            }
            break;
            // end of auxiliary states for comments
        }

    } while (read_file);

    token->type = T_EOF;
    token->tlen = strlen(tmp_string);
    token->line = scanner_struct->line;
    return token;
}
