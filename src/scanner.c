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
#include "return_codes.h"

char* keywords[17] = {"boolean", "break", "class", "continue","do",
    "double", "else", "false", "for", "if", "int",
    "return", "String", "static", "true", "void", "while"
};

Tinit* init_scanner(FILE* fp) {
    long long n = 1;
    Tinit* scanner_struct = gc_alloc(sizeof(Tinit));
    scanner_struct->f = fp;
    scanner_struct->line = n;
    scanner_struct->token = NULL;
    return scanner_struct;
}

char* octal_append(char* octal_string, unsigned char c) {
    size_t octal_string_len;
    octal_string_len = strlen(octal_string);
    octal_string[octal_string_len] = c;
    octal_string[octal_string_len + 1] = '\0';
    return octal_string;
}


char* char_append(char* tmp_string, unsigned int* alloc_len, unsigned char c) {
    unsigned int tmp_string_len;
    tmp_string_len = strlen(tmp_string);
    if (*alloc_len < (tmp_string_len + 2)) {
        tmp_string = (char *)gc_realloc(tmp_string, sizeof(char) * (tmp_string_len + 5));
        *alloc_len = (*alloc_len) + 4;
    }
    tmp_string[tmp_string_len] = c;
    tmp_string[tmp_string_len + 1] = '\0';
    return tmp_string;
}

char* is_keyword(char* tmp_string) {
    for (int i = 0; i <= 16; i++) {
        if (!strcmp(tmp_string, keywords[i])) {
            return keywords[i];
        }
    }
    return NULL;
}

Ttoken* peek_token(Tinit* scanner_struct) {
    if (scanner_struct->token == NULL)
        scanner_struct->token = get_token(scanner_struct);
    return scanner_struct->token;
}

Ttoken* get_token_internal(Tinit* scanner_struct);

Ttoken* get_token(Tinit* scanner_struct) {
    Ttoken* ret;
    if (scanner_struct->token != NULL) {
        ret = scanner_struct->token;
        scanner_struct->token = NULL;
    } else
        ret = get_token_internal(scanner_struct);
    return ret;
}

Ttoken* get_token_internal(Tinit* scanner_struct) {
    char* kw_ptr;
    char* endptr;
    char c;
    int read_file = 1;
    unsigned int alloc_len = 2;
    states state = FSM_INIT;

    char* tmp_string = (char *)gc_alloc(sizeof(char) * alloc_len);
    tmp_string[0] = 0;

    char* octal_string = (char *)gc_alloc(sizeof(char) * 4);
    octal_string[0] = 0;

    Ttoken* token = (Ttoken *)gc_alloc(sizeof(Ttoken));

    token->whence = ftell(scanner_struct->f);

    do {
        c = fgetc(scanner_struct->f);
        switch (state) {
            case FSM_INIT:
                if (isspace(c)) {
                    state = FSM_INIT;
                    if (c == '\n') {
                        scanner_struct->line++;
                    }
                    continue;
                } else if ((isalpha(c)) || (c == '_') || (c == '$')) {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_ID;
                } else if (isdigit(c)) {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_INT;
                } else if (c == '*') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_MUL;
                } else if (c == '/') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_DIV;
                } else if (c == '+') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_ADD;
                } else if (c == '-') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_SUB;
                } else if (c == ',') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_COMMA;
                } else if (c == ';') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_SEMICOLON;
                } else if (c == '(') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_BRACKET_LROUND;
                } else if (c == ')') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_BRACKET_RROUND;
                } else if (c == '[') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_BRACKET_LSQUARE;
                } else if (c == ']') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_BRACKET_RSQUARE;
                } else if (c == '{') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_BRACKET_LCURLY;
                } else if (c == '}') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_BRACKET_RCURLY;
                } else if (c == '=') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_EQUAL;
                } else if (c == '!') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_NOT;
                } else if (c == '<') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_LOWER;
                } else if (c == '>') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_GREATER;
                } else if (c == '"') {
                    state = FSM_QUOTE;
                } else if (c == '.') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_DOT;
                } else if (c == EOF) {
                    read_file = 0;
                    continue;
                } else {
                    fprintf(stderr, "Unidentified lexem on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                }
                break;

            case FSM_ID:
                if (isalpha(c) || isdigit(c) || c == '_' || c == '$') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_ID;
                } else {
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
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else if (c == '.') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    state = FSM_DOUBLE;
                } else if (c == 'E' || c == 'e') {
                    state = FSM_EXPONENT;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else {
                    ungetc(c, scanner_struct->f);
                    token->type = T_INT;
                    token->tlen = strlen(tmp_string);
                    token->line = scanner_struct->line;
                    token->li = strtol(tmp_string, &endptr, 10);
                    token->c = tmp_string;
                    return token;
                }
                break;

            case FSM_DOUBLE:
                if (isdigit(c)) {
                    state = FSM_DOUBLE;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else if (c == 'E' || c == 'e') {
                    state = FSM_EXPONENT;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else {
                    ungetc(c, scanner_struct->f);
                    token->type = T_DOUBLE;
                    token->tlen = strlen(tmp_string);
                    token->line = scanner_struct->line;
                    token->d = strtod(tmp_string, &endptr);
                    token->c = tmp_string;
                    return token;
                }
                break;

            case FSM_EXPONENT:
                if (c == '+' || c == '-') {
                    state = FSM_EXPONENT_SIGN;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else if (isdigit(c)) {
                    state = FSM_EXPONENT_2;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else {
                    fprintf(stderr, "SCANNER ERROR: Exponent error on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                }
                break;

            case FSM_EXPONENT_SIGN:
                if (isdigit(c)) {
                    state = FSM_EXPONENT_2;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else {
                    fprintf(stderr, "SCANNER ERROR: Exponent sign error on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                }
                break;

            case FSM_EXPONENT_2:
                if (isdigit(c)) {
                    state = FSM_EXPONENT_2;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                } else {
                    ungetc(c, scanner_struct->f);
                    token->type = T_DOUBLE;
                    token->tlen = strlen(tmp_string);
                    token->line = scanner_struct->line;
                    token->d = strtod(tmp_string, &endptr);
                    token->c = tmp_string;
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
                    tmp_string = char_append(tmp_string, &alloc_len, c);
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
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                    token->type = T_NOT_EQUAL;
                    token->tlen = strlen(tmp_string);
                    token->line = scanner_struct->line;
                    token->c = tmp_string;
                    return token;
                }
                fprintf(stderr, "SCANNER ERROR: Unidentified lexem on line %lld!\n", scanner_struct->line);
                exit(lexical_analysis_error);
            }

            case FSM_LOWER: {
                if (c == '=') {
                    tmp_string = char_append(tmp_string, &alloc_len, c);
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
                    tmp_string = char_append(tmp_string, &alloc_len, c);
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
                } else if (c == '"') {
                    state = FSM_STRING;
                } else if (!isprint(c)) {
                    fprintf(stderr, "SCANNER ERROR: Unidentified token on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                } else {
                    state = FSM_QUOTE;
                    tmp_string = char_append(tmp_string, &alloc_len, c);
                }
                break;

            case FSM_ESCAPE:
                if (c == '\\') {
                    state = FSM_QUOTE;
                    tmp_string = (char_append(tmp_string, &alloc_len, 92));
                } else if (c == '"') {
                    state = FSM_QUOTE;
                    tmp_string = (char_append(tmp_string, &alloc_len, 34));
                } else if (c == 't') {
                    state = FSM_QUOTE;
                    tmp_string = (char_append(tmp_string, &alloc_len, 9));
                } else if (c == 'n') {
                    state = FSM_QUOTE;
                    tmp_string = (char_append(tmp_string, &alloc_len, 10));
                } else if (c >= '0' && c <= '3') {
                    state = FSM_ESCAPE_OCTAL_1;
                    octal_string = (octal_append(octal_string, c));
                } else {
                    fprintf(stderr, "SCANNER ERROR: String escape sequence error on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
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
                    octal_string = (octal_append(octal_string, c));
                } else {
                    fprintf(stderr, "SCANNER ERROR: String escape sequence error on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                }
                break;

            case FSM_ESCAPE_OCTAL_2:
                if (c >= '0' && c <= '7') {
                    state = FSM_QUOTE;
                    octal_string = (octal_append(octal_string, c));
                    long int ascii_c = strtol(octal_string, &endptr, 8);
                    tmp_string = (char_append(tmp_string, &alloc_len, ascii_c));
                } else {
                    fprintf(stderr, "SCANNER ERROR: String escape sequence error on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                }
                break;
                // end of auxiliary states for string escape octal

                // start of auxiliary states for comments
            case FSM_COMMENT_LINE:
                if (c != '\n') {
                    state = FSM_COMMENT_LINE;
                    if (c == EOF) {
                        ungetc(c, scanner_struct->f);
                        state = FSM_INIT;
                    }
                } else {
                    memset(tmp_string, 0, (strlen(tmp_string)) * sizeof(char));
                    ungetc(c, scanner_struct->f);
                    state = FSM_INIT;
                }
                break;

            case FSM_COMMENT_BLOCK:
                if (c == '*') {
                    state = FSM_COMMENT_BLOCK_FIN;
                } else if (c == EOF) {
                    fprintf(stderr, "Unterminated comment on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                } else {
                    if (c == '\n') {
                        scanner_struct->line++;
                    }
                    state = FSM_COMMENT_BLOCK;
                }
                break;

            case FSM_COMMENT_BLOCK_FIN:
                if (c == '/') {
                    memset(tmp_string, 0, (strlen(tmp_string)) * sizeof(char));
                    state = FSM_INIT;
                } else if (c == '*') {
                    state = FSM_COMMENT_BLOCK_FIN;
                } else if (c == EOF) {
                    fprintf(stderr, "Unterminated comment on line %lld!\n", scanner_struct->line);
                    exit(lexical_analysis_error);
                } else {
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

char* tokens_to_string(token_type tokens) {
    char* ret = gc_alloc(sizeof(char) * 2);
    ret[0] = 0;
    for (int i = 1; i <= 1 << ((sizeof(int) * 8) - 3); i <<= 1) {
        if (i & tokens) {
            const char* tokString = token_to_string((token_type)i);
            ret = gc_realloc(ret, sizeof(char) * (strlen(tokString) + strlen(ret) + 3));
            strcat(ret, tokString);
            strcat(ret, ", ");
        }
    }
    //remove trailing ', '
    if (strlen(ret))
        ret[strlen(ret) - 2] = 0;
    return ret;
}

char* keywords_to_string(Keyword keywords) {
    char* ret = gc_alloc(sizeof(char) * 2);
    ret[0] = 0;
    for (int i = 1; i <= 1 << ((sizeof(int) * 8) - 1); i <<= 1) {
        if (i & keywords) {
            const char* kwString = keyword_to_string((Keyword)i);
            ret = gc_realloc(ret, sizeof(char) * (strlen(kwString) + strlen(ret) + 3));
            strcat(ret, kwString);
            strcat(ret, ", ");
        }
    }
    //remove trailing ', '
    if (strlen(ret))
        ret[strlen(ret) - 2] = 0;
    return ret;
}

Ttoken* check_and_get_token(Tinit* scanner_struct, token_type type) {
    Ttoken* tok = check_and_peek_token(scanner_struct, type);
    get_token(scanner_struct);
    return tok;
}

Ttoken* check_and_peek_token(Tinit* scanner_struct, token_type type) {
    Ttoken* tok = peek_token(scanner_struct);
    if (((int)tok->type & type) == 0) {
        fprintf(stderr, "Expected %s got '%s', on line %lld", tokens_to_string(type), tok->c, tok->line);
        exit(semantic_error_in_code);
    }
    return tok;
}

Keyword check_and_get_keyword(Tinit* scanner_struct, Keyword keyword) {
    Keyword kw = check_and_peek_keyword(scanner_struct, keyword);
    get_token(scanner_struct);
    return kw;
}

Keyword check_and_peek_keyword(Tinit* scanner_struct, Keyword keyword) {
    Ttoken* tok = check_and_peek_token(scanner_struct, T_KEYWORD);
    if (((int)tok->kw & keyword) == 0) {
        fprintf(stderr, "Expected %s got '%s', on line %lld", keywords_to_string(keyword), tok->c, tok->line);
        exit(semantic_error_in_code);
    }
    return tok->kw;
}
