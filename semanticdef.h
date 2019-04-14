#ifndef SEMANTIC_DEF
#define SEMANTIC_DEF

#include"astdef.h"
#include"symtabledef.h"
#include"lexer.h"
#include<stdbool.h>
#include"ast.h"

#define MAX_ERROR_LENGTH 150


struct error{
    char * error;
    struct error*next;
};


struct errorList{
	struct error * head;
	struct error * tail;
};


typedef struct errorList* ErrorList;

typedef struct arithmethic_check{
    enum types type;
    bool isError;
    Rnode rec;
}arith_check;
#endif