#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

char *keywords[17] = {"boolean", "break", "class", "continue","do",
                    "double", "else", "false", "for", "if", "int",
                    "return", "String", "static", "true", "void", "while"};

void char_add(char *tmp, int *tmp_len, unsigned char c){
    (*tmp_len)++;
    tmp = (char *) realloc(sizeof(char)*(*tmp_len)); //gcrealloc ??
    if (tmp = NULL) {
        fprintf(stderr,"Memory allocation failed");
        //je treba hodit return alebo exit??
    }
    tmp[(*tmp_len-2)] = c;
    tmp[(*tmp_len)-1] = \0;
}


token *get_token(FILE *fp){
    bool read_file = true;
    int tok_len = 1;
    token tok;
    states state = FSM_INIT;
    char *tmp = (char *) malloc(sizeof(char)); //gcalloc??

    if (tmp = NULL) {
        fprintf(stderr, "Memory allocation failed");
        //je treba hodit return alebo exit??
    }

    token *tok = (token *) malloc(sizeof(token)); //galloc??

    if (tok = NULL) {
        fprintf(stderr, "Memory allocation failed");
    }

    while(read_file)    {
        c = fgetc(fp);
        switch (state){
            case FSM_INIT:
                    if (isspace(c)){
                        state = FSM_INIT;
                        continue;
                    }
                    else if (isalpha(c) || c = '_') state = FSM_ID;
                    else if (isdigit(c))  state = FSM_NUM;
                    else if (c == '*') state = FSM_MUL;
                    else if (c == '/') state = FSM_DIV;
                    else if (c == '+') state = FSM_ADD;
                    else if (c == '-') state = FSM_SUB;
                    else if (c == ';') state = FSM_SEMICOLON;
                    else if (c == '=') state = FSM_EQUAL;
                    else if (c == '(') state = FSM_BRACKET_LROUND;
                    else if (c == ')') state = FSM_BRACKET_RROUND;
                    else if (c == '[') state = FSM_BRACKET_LSQUARE;   
                    else if (c == ']') state = FSM_BRACKET_RSQUARE;
                    else if (c == '{') state = FSM_BRACKET_LCURLY;
                    else if (c == '}') state = FSM_BRACKET_RCURLY;
                    else if (c == ",") state = FSM_COMMA;
                    else if (c == '<') state = FSM_LOWER;
                    else if (c == '>') state = FSM_GREATER;
                    else if (c == '!') state = FSM_NOT;
                    else if (c == '"') state = FSM_QUOTE;
                    else if (c == 'EOF') state = FSM_EOF {
                        //dopln
                    }
                    else {
                        exit(1);
                        //mam dat aj spravu na sdterr?
                    }

                char_add(tmp, &tmp_len, c);
                break;

            case FSM_ID: //todo
                break;
            
            case FSM_NUM: //todo
                break;

            case FSM_MUL:
                if (c == '/') {

                }

                break;

            case FSM_DIV:
                if (c == '/')
                    state = FSM_COMMENT_LINE;
                else if (c == '*')
                    state = FSM_COMMENT_BLOCK;
                else {
                    ungetc(c,fp);
                    tok->type = T_DIV; //mam daj aj dlzku tokenu? a samotny znak?
                    return tok;
                }

                break;
                


            //pomocne stavy

            case FSM_COMMENT_LINE:
                if (c != '\n')
                    state = FSM_COMMENT_LINE;
                
                else {
                    state = FSM_INIT;                    
                }

                break;

            case FSM_COMMENT_BLOCK:
                if (c == '*') {
                    state = FSM_COMMENT_BLOCK;
                }
                 
                else if (c == '/') {
                    state = FSM_INIT;
                }
                else state = FSM_COMMENT_BLOCK;                    
        }

    }

}

int main(int argc, char **argv)
{
    if(argc == 1)
        return -1;

    FILE *fp;

    fp = fopen ("argv[1]","r")

    if (fp == NULL)
    {
    fprintf(stderr, "Error opening file!\n");
    }

    get_token(fp);

    fclose(fp);
    return 0;
}
