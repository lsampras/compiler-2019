

#ifndef ASTDEF
#define ASTDEF

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include"symtabledef.h"
// #include"parserDef.h"
#include"lexerDef.h"




struct astNode{

	//For Non-Leaf Nodes
    struct node label;
	struct astNode* parent;
	struct astNode* children;

	struct astNode* next; 
	//For symbol Table
	IdTable* current_scope;
	int line_no;

	char* lex;//for leaf
    char * rectype;//subtype if record
	int ival;
    float rval;
    int width;
    struct astNode* last_child;
    struct astNode* last_node;
};

typedef struct astNode* astnode;



#endif