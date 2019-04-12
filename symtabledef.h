#ifndef SYMDEF
#define SYMDEF
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define IDSIZE 100
#define SYMSIZE 100
#define RECSIZE 100

enum types{
    INTEGER,
    REALT,
    RECORDT
};


typedef struct recordtypenode{
    char * id;
    enum types type;
struct recordtypenode * next;
} * Rec;

typedef struct record{
    char * typeclass;
    Rec child;
    struct record * next;
} * Rnode;

typedef struct record recTable;


typedef struct idNode{
    enum types type;
    bool assigned;
    char * id;
    struct idNode * next;
    Rnode rec;//if record
}*idnode;

typedef struct parameterslist{
    struct parameterslist * next;
    enum types type;
}* params;

typedef idnode * IdTable;

typedef struct symnode{
    char * id;
    struct idnode* scope[IDSIZE];
    int width;
    params input;
    params output;
    struct symnode* next;
}* SymNode;

typedef struct symnode SymTable;

#else
#endif