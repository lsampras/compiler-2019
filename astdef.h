

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
	IdTable current_scope;
	int line_no;

	char* lex;//for leaf
    int offset;
    struct astNode* last_child;
    struct astNode* last_node;
};

typedef struct astNode* astnode;



#endif