/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#ifndef PARSEDEF
#define PARSEDEF

#include "lexerDef.h"
#include<stdint.h>
#include"symtabledef.h"
#include<inttypes.h>
#include"astdef.h"
#define NONTERMINALCOUNT 51
#define RULESIZE 92


typedef struct LinkedListNode{
    struct LinkedListNode *next;
    struct node node;
} *llNode;

// typedef struct LinkedListnode* llNode;
typedef uint64_t set;


struct LinkedListGrammar{
    set first;
    llNode prod;
    enum nonterminal nt;
};

// typedef struct LinkedListGrammar* llGrammar;


typedef struct LinkedListTerm{
    struct LinkedListTerm *next;
    enum tok t;
} *llTerm;

struct FirstFollow{
    set first;
    set  follow;
};


struct narytree{
    struct narytree* children;
    struct narytree* next;
    struct node parent;
    struct narytree * parnode;
    char * lexeme;
    int line;
    int rule;
    astnode ast;
};


typedef struct StackNode{
    struct narytree *tree;
    struct StackNode * next;
} *stackNode;

typedef struct Stack{
    stackNode head;
} STACK;

// typedef struct FirstFollow *FirstandFollow;


struct SYMBOL{
    struct SYMBOL * next;
    char * key;
    union {
        enum nonterminal nonterm;
        enum tok term;
    }value;
    
};


typedef struct SYMBOL symbolnode;

#endif
