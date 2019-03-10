/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include "lexerDef.h"
void removeComments(FILE * in, char * cleanFile);
token getNextToken();
FILE * getStream(FILE * fp);
void printToken(token);
unsigned long gethash(char *str);
bool file_empty();
void seedHashTable();
void freeHashTable();
