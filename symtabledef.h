/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
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
    int offset;
struct recordtypenode * next;
} * Rec;

typedef struct record{
    char * typeclass;
    Rec child;
    int width;
    struct record * next;
} * Rnode;

typedef struct record recTable;


typedef struct idNode{
    enum types type;
    bool assigned;
    bool updated;
    int offset;
    char * id;
    struct idNode * next;
    Rnode rec;//if record
}*idnode;

typedef struct parameterslist{
    struct parameterslist * next;
    enum types type;
    Rnode rec;
}* params;

typedef idnode * IdTable;

typedef struct symnode{
    char * id;
    IdTable scope;
    int width;
    params input;
    params output;
    struct symnode * next;
}* SymNode;

typedef struct symnode SymTable;

extern SymTable *Symbols[];
extern recTable *Records[];

#else
#endif