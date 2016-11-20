/** Interpretr jazyka IFJ16
* @file build_in.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "return_codes.h"
#include "build_in.h"
#include "ial.h"

// int readInt ();
Value readInt(Value_list vals) {
    if (vals.count != 0) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    Tstring* r_int;
    int i = 0; // delka, nasledne vysledne cislo
    int err = 0; // detekce erroru (podminky retezce)
    char c;

    r_int = gc_alloc(sizeof(struct Tstring));
    r_int->str = gc_alloc(sizeof(char) * INC_STRLEN);

    r_int->size = INC_STRLEN;
    r_int->len = 0;

    c = getchar(); // cteni prvniho znaku
    while (c >= '0' && c <= '9') {
        if (r_int->len == r_int->size) { // pokud je zaplnen alokovany prostor
            r_int->str = gc_realloc(r_int->str, r_int->size + (sizeof(char) * INC_STRLEN));
            r_int->size += INC_STRLEN;
        }
        r_int->str[i] = c;
        i++;
        r_int->len = i;
        c = getchar();
    }
    if (c == '\n' || c == EOF) // pokud je ukonceno cteni
    {
        if (r_int->len > 0) { // pokud se nacetlo cislo
            i = atoi(r_int->str);
        } else
            err = 1;
    }
    if (c != '\n' && c != EOF) // pokud byl zaznamenan jiny nepripustny znak
        err = 1;
    if (err == 0)
        return (Value) {.type = int_t, .init = true, .i = i};

    fprintf(stderr, "Input error\n");
    exit(runtime_input_error);
}

// double readDouble ();
Value readDouble(Value_list vals) {
    if (vals.count != 0) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    Tstring* r_dbl;
    int i = 0; // delka
    double d = 0; // vysledne desetinne cislo
    char* ptr; // prebytkovy retezec
    int err = 0; // detekce erroru (podminky retezce)
    char c; // nacteny znak

    r_dbl = gc_alloc(sizeof(struct Tstring)); 
    r_dbl->str = gc_alloc(sizeof(char) * INC_STRLEN);

    r_dbl->size = INC_STRLEN;
    r_dbl->len = 0;

    c = getchar(); // cteni prvniho znaku
    if (c == '\n' || c == EOF) { // pokud je vstupem prazdny retezec
        fprintf(stderr, "Empty input\n");
        exit(runtime_input_error);
    }
    if (c < '0' || c > '9') { // kontrola na spatny prvni znak
        err = 1;
    }
    while ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E') { // cteni stringu tvoreneho povolenými znaky
        if (r_dbl->len == r_dbl->size) { // pokud je zaplnen alokovany prostor
            r_dbl->str = gc_realloc(r_dbl->str, r_dbl->size + (sizeof(char) * INC_STRLEN));
            r_dbl->size += INC_STRLEN;
        }
        r_dbl->str[i] = c;
        i++;
        r_dbl->len = i;
        c = getchar();
    }
    while (c != '\n' && c != EOF) { // pokud byl zaznamenan jiny nepripustny znak
        err = 1;
        c = getchar();
    }
    if (c == '\n' || c == EOF) {
        d = strtod(r_dbl->str, &ptr);
        if (err == 1 || *ptr != '\0') {
            fprintf(stderr, "Input error\n");
            exit(runtime_input_error);
        }
    }
    return (Value) {.type = double_t, .init = true, .d = d};
}

// String readString ();
Value readString(Value_list vals) {
    if (vals.count != 0) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    Tstring* r_str; // deklarace struktury
    int i = 0; // delka, nasledne vysledne cislo
    char c; // nacteny znak

    r_str = gc_alloc(sizeof(struct Tstring)); 
    r_str->str = gc_alloc(sizeof(char) * INC_STRLEN); 

    r_str->size = INC_STRLEN;
    r_str->len = 0;

    c = getchar(); // cteni prvniho znaku
    while (c != '\n' && c != EOF) {
        if (r_str->len == r_str->size) { // pokud je zaplnen alokovany prostor
            r_str->str = gc_realloc(r_str->str, r_str->size + (sizeof(char) * INC_STRLEN));
            r_str->size += INC_STRLEN;
        }
        r_str->str[i] = c;
        i++;
        r_str->len = i;
        c = getchar();
    }
    if (r_str->len == r_str->size) { // pokud je zaplnen alokovany prostor
        r_str->str = gc_realloc(r_str->str, r_str->size + sizeof(char));
        r_str->size += 1;
    }
    r_str->str[i] = '\0';
    return (Value) {.type = string_t, .init = true, .s = r_str->str};
}

// void print ( term_nebo_konkatenace );
Value print(Value_list vals) {
    if (vals.count != 1) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }
    Value arg = vals.val[0];

    switch (arg.type) {
    case int_t:
        printf("%d", arg.i);
        break;
    case double_t:
        printf("%g", arg.d);
        break;
    case string_t:
        printf("%s", arg.s);
        break;
    default:
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }
    return (Value) { .init = true, .type = void_t };
}

// int length(String s);
Value length(Value_list vals) {
    if (vals.count != 1 || vals.val[0].type != string_t) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    return (Value) {.type = int_t, .init = true, .i = strlen(vals.val[0].s)};
}

// String substr(String s, int i, int n);
Value substr(Value_list vals) {
    if (vals.count != 3 || vals.val[0].type != string_t || vals.val[1].type != int_t || vals.val[2].type != int_t) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }
    char* s = vals.val[0].s;
    int i = vals.val[1].i;
    int n = vals.val[2].i;

    int len = strlen(s);

    if ((i + n) > len || n < 0) {
        fprintf(stderr, "Substring error\n");
        exit(runtime_error);
    }

    char* cs = gc_alloc(sizeof(char) * (n + 1));
    memcpy(cs, s + i, n);
    cs[n+1] = '\0';
    
    return (Value) {.type = string_t, .init = true, .s = cs};
}

// int compare(String s1, String s2);
Value compare(Value_list vals) {
    if (vals.count != 2 || vals.val[0].type != string_t || vals.val[1].type != string_t) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    return (Value) {.type = int_t, .init = true, .i = strcmp(vals.val[0].s, vals.val[1].s)};
}

// int find(String s, String search);
Value findBI(Value_list vals) {
    if (vals.count != 2 || vals.val[0].type != string_t || vals.val[1].type != string_t) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    return (Value) { .type = int_t, .init = true, .i = find(vals.val[0].s, vals.val[1].s) };
}

// String sort(String s);
Value sortBI(Value_list vals) {
    if (vals.count != 1 || vals.val[0].type != string_t) {
        fprintf(stderr, "Invalid arguments\n");
        exit(semantic_error_in_types);
    }

    return (Value) { .type = string_t, .init = true, .s = sort(vals.val[0].s) };
}