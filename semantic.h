/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include"semanticdef.h"
void thirdPass(astnode root,ErrorList errors);
void printErrors(ErrorList errors);
void reportError(ErrorList errors,char * temp);
SymNode getSymNode(char * funid);