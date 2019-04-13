
#include"astdef.h"
#include"symtabledef.h"
#include"lexer.h"
#include<stdbool.h>
#include"ast.h"


struct error{
    int line;
    char * error;
    struct error*next;
};


struct errorList{
	struct error * head;
	struct error * tail;
};


typedef struct errorList* ErrorList;