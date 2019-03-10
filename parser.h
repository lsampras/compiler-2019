/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include "parserDef.h"


//private funcs not included

void seedSymbolTable();
void ComputeFirstAndFollowSets();
void initParseTable();
void printTree(FILE * a);
void printFirstFollow();
bool initTree();
void readGrammar(char * file);
void freesymbolTable();
void freeGrammar();
