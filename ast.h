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
