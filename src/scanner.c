#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "gc.h"

char *keywords[17] = {"boolean", "break", "class", "continue","do",
                    "double", "else", "false", "for", "if", "int",
                    "return", "String", "static", "true", "void", "while"};

void char_append(char *tmp_string, unsigned int *tmp_string_len, unsigned char c){
    (*tmp_string_len)++;
    tmp_string = (char *) gc_realloc(tmp_string,sizeof(char)*(*tmp_string_len));
    /*if (tmp = NULL) {
        fprintf(stderr,"Memory allocation failed");
        //je treba hodit return alebo exit??
    }*/
    tmp_string[(*tmp_string_len-2)] = c;
    tmp_string[(*tmp_string_len)-1] = '\0';
}


Ttoken *get_token(FILE *fp){
    char *endptr;
    char c;
    int read_file = 1;
    unsigned int tmp_string_len = 1;
    long double line_no = 1;

    states state = FSM_INIT;
    
    char *tmp_string = (char *) gc_alloc(sizeof(char)); //gcalloc??

    //Ttoken *tok = gcmalloc(sizeof(Ttoken));

    /*if (tmp_string = NULL) {
        fprintf(stderr, "Memory allocation failed");
        //je treba hodit return alebo exit??
    }
    */
    Ttoken *token = (Ttoken *) gc_alloc(sizeof(Ttoken)); //galloc??

    /*if (token = NULL) {
        fprintf(stderr, "Memory allocation failed");
    }*/

    do    {
        c = fgetc(fp);
        if (c != EOF) { // este porozmyslaj
            if(c == '\n') {
                line_no ++;
            }

        }
        switch (state){
            case FSM_INIT:
                    if (isspace(c)){
                        state = FSM_INIT;
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
                    else if (c == EOF) {
                        read_file = 0;
                        
                        continue;
                    }
                    else {
                        fprintf(stderr,"Unidentified lexem!");
                        exit(1);                        
                    }

                char_append(tmp_string, &tmp_string_len, c);
               break;

            case FSM_ID:
                if ((isalpha(c)) || (isdigit(c)) || (c == '_') || (c == '$')) {
                    char_append(tmp_string, &tmp_string_len, c);
                    state = FSM_ID;
                }
                else if (c == '.') {

                }


                break;
            
            case FSM_INT: //todo
                if (isdigit(c)) {
                    state = FSM_INT;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else if (c == '.') {
                    state = FSM_DOUBLE;
                }
                else if ((c == 'E') || (c == 'e')) {
                    state = FSM_EXPONENT;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else {
                    ungetc(c,fp);
                    token->type = T_INT;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->li = strtol(tmp_string, &endptr, 10); // bolo by dobre skontrolovat ci nepretieklo
                    printf("CHARAPPEND: %s\n",tmp_string);
                    return token;
                }

                break;

            case FSM_DOUBLE:
                if (isdigit(c)) {
                    state = FSM_DOUBLE;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else if ((c == 'E') || (c == 'e')) {
                    state = FSM_EXPONENT;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else {
                    ungetc(c,fp);
                    token->type = T_DOUBLE;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->d = strtof(tmp_string, &endptr);
                    return token;
                }

                break;

            case FSM_EXPONENT:
                if ((c == '+') || (c == '-')) {
                    state = FSM_EXPONENT_SIGN;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else if (isdigit(c)) {
                    state = FSM_EXPONENT_2;
                }
                else {
                    fprintf(stderr, "SCANNER ERROR: Exponent error!\n");
                    exit(1);
                }

                break;

            case FSM_EXPONENT_SIGN:
                if (isdigit(c)) {
                    state = FSM_EXPONENT_2;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else {
                    fprintf(stderr, "SCANNER ERROR: Exponent sign error!\n");
                    exit(1);
                }

                break;

            case FSM_EXPONENT_2:
                if (isdigit(c)) {
                    state = FSM_EXPONENT_2;
                    char_append(tmp_string, &tmp_string_len, c);  
                }
                else {
                    ungetc(c,fp);
                    token->type = T_DOUBLE;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->d = strtof(tmp_string, &endptr);
                    return token;
                }

                break;

            case FSM_MUL:
                token->type = T_MUL;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;


            case FSM_DIV:
                if (c == '/')
                    state = FSM_COMMENT_LINE;
                else if (c == '*')
                    state = FSM_COMMENT_BLOCK;
                else {
                    ungetc(c,fp);
                    //tmp_string[1] = '\0';
                    token->type = T_DIV;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }

                break;

            case FSM_ADD:
                token->type = T_ADD;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_SUB:
                token->type = T_SUB;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_COMMA:
                token->type = T_COMMA;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_SEMICOLON:
                token->type = T_SEMICOLON;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_BRACKET_LROUND:
                token->type = T_BRACKET_LROUND;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_BRACKET_RROUND:
                token->type = T_BRACKET_RROUND;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_BRACKET_LSQUARE:
                token->type = T_BRACKET_LSQUARE;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;
            
            case FSM_BRACKET_RSQUARE:
                token->type = T_BRACKET_RSQUARE;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_BRACKET_LCURLY:
                token->type = T_BRACKET_LCURLY;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            case FSM_BRACKET_RCURLY:
                token->type = T_BRACKET_RCURLY;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;     

            case FSM_EQUAL:
                if (c == '=') {
                    token->type = T_BOOL_EQUAL;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }
                else {
                    ungetc(c,fp);
                    //tmp_string[1] = '\0';
                    token->type = T_ASSIGN;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }

                break;

            case FSM_NOT:
                if (c == '=') {
                    token->type = T_NOT_EQUAL;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }
                else {
                    fprintf(stderr, "SCANNER ERROR: Unidentified lexem!\n");
                    exit(1);
                }

            case FSM_LOWER:
                if (c == '=') {
                    char_append(tmp_string, &tmp_string_len, c);
                    token->type = T_LOWER_EQUAL;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }
                else {
                    ungetc(c,fp);
                    //tmp_string[1] = '\0';
                    token->type = T_LOWER;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }

                break;

            case FSM_GREATER:
                if (c == '=') {
                    char_append(tmp_string, &tmp_string_len, c);
                    token->type = T_GREATER_EQUAL;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }
                else {
                    ungetc(c,fp);
                    tmp_string[1] = '\0';
                    token->type = T_GREATER;
                    token->tlen = tmp_string_len-1;
                    token->line = line_no;
                    token->c = tmp_string;
                    return token;
                }

                break;

            case FSM_QUOTE:
                if (c == '\\') {
                    state = FSM_ESCAPE;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else if (c == '"') {
                    state = FSM_STRING;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else {
                    state = FSM_QUOTE;
                    char_append(tmp_string, &tmp_string_len, c);
                }

                break;

            case FSM_ESCAPE:
                if ((c == '"') || (c == 't') || (c == 'n') || (c == '\\')) {
                    state = FSM_QUOTE;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else if ((c >= '0') && (c <= '3')) {
                    state = FSM_ESCAPE_OCTAL_1;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else {
                    fprintf(stderr, "SCANNER ERROR: String escape sequence error!\n");
                    exit(1);
                }

                break;

            case FSM_STRING:
                ungetc(c,fp);
                token->type = T_STRING;
                token->tlen = tmp_string_len-1;
                token->line = line_no;
                token->c = tmp_string;
                return token;

                break;

            // start of auxiliary states for string escape octal

            case FSM_ESCAPE_OCTAL_1:
                if ((c >= '0') && (c <= '7')) {
                    state = FSM_ESCAPE_OCTAL_2;
                    char_append(tmp_string, &tmp_string_len, c);
                }
                else {
                    fprintf(stderr, "SCANNER ERROR: String escape sequence error!\n");
                    exit(1);
                }

                break;

            case FSM_ESCAPE_OCTAL_2:
                 if ((c >= '0') && (c <= '7')) {
                    state = FSM_QUOTE;
                    char_append(tmp_string, &tmp_string_len, c);
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
                }                
                else {
                    memset(tmp_string,0,(tmp_string_len)*sizeof(char));
                    tmp_string_len = 1;
                    state = FSM_INIT;                    
                }

                break;

            case FSM_COMMENT_BLOCK:
                if (c == '*') {
                    state = FSM_COMMENT_BLOCK_FIN;
                }
                else {
                    state = FSM_COMMENT_BLOCK;   
                }

                break; 

            case FSM_COMMENT_BLOCK_FIN:
                if (c == '/') {
                    memset(tmp_string,0,(tmp_string_len)*sizeof(char));
                    tmp_string_len = 1;
                    state = FSM_INIT;  
                } 
                else if (c == '*') {
                    state = FSM_COMMENT_BLOCK_FIN;
                }
                else {
                    state = FSM_COMMENT_BLOCK;
                }

                break;
            // end of auxiliary states for comments                 
        }
 
    } while(read_file);

token->type = T_EOF;
token->tlen = 0;
token->line = line_no;
return token;
 
//tuna chce navratovu hodnotu
}
/*
int main(int argc, char *argv[])
{
    gc_init();
    for (int i=0; i!=50; i++) {
        gc_alloc(50);
    }
    return 0;

    if(argc == 1)
        return -1;

    FILE *fp;

    fp = fopen (argv[1],"r");

    if (fp == NULL)
    {
    fprintf(stderr, "Error opening file!\n");
    return 99;
    }

    //get_token(fp);

    fclose(fp);
    return 0;
}
*/
