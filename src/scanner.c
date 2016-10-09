#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

char *keywords[17] = {"boolean", "break", "class", "continue","do",
                    "double", "else", "false", "for", "if", "int",
                    "return", "String", "static", "true", "void", "while"};

void char_add(char *tmp, int *tok_len, unsigned char c){
    (*tok_len)++;
    tmp = (char *) realloc(sizeof(char)*(*tok_len)); //gcrealloc ??
    if (tmp = NULL) {
        fprintf(stderr,"Memory allocation failed");
        //je treba hodit return alebo exit??
    }
    tmp[(*tok_len-2)] = c;
    tmp[(*tok_len)-1] = \0;
}


token *get_token(FILE *fp){
    bool read_file = true;
    int tok_len = 1;
    token tok;
    states state = S_INIT;
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
            case S_INIT:
                    if (isspace(c)){
                        state = S_INIT;
                        continue;
                    }
                    else if (isalpha(c) || c = '_') state = S_ID;
                    else if (isnum(c))  state = S_NUM;
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
