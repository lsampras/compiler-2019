/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include"astdef.h"

#include"parserDef.h"
char* clone(char* str);
astnode createAstLeafNode(struct narytree* parsetree);
astnode createAstNonleafNode(struct narytree* parsetree);
astnode converthelper(struct narytree * tree);
astnode convert(astnode term,struct narytree* exprime);
void processAstNode(struct narytree *parsetree);
void generateTree(struct narytree*parseTree);
struct astNode initAST(struct narytree * parseTree);
void printAST(astnode ast,astnode par);
int getastnodes();
void freeAST(astnode ast);
