/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include"astdef.h"
#include"symtabledef.h"
#include"semantic.h"
void firstPass(astnode root,ErrorList errors);
void printrecordtable();
void secondPass(astnode root,ErrorList errors);
void printGlobalTable();
void printIdTable(IdTable scope,char* id);
enum types getRecSubType(Rnode rec,char* field);
idnode checkId(char * id,IdTable scope);
idnode checkIdGlobal(char * id);
bool isRecSubType(Rnode rec,char* field);
SymNode addToSymbolTable(astnode func,ErrorList errors);
SymNode getSymNode(char * funid);
void printfuncsize();
void printSymbolTable();
void freeBigTable();
void freerecordtable();
void freeGlobalTable();
// void printGlobalTable();
Rnode checkRecords(char * recid);
int getRecSubOffset(Rnode rec,char* field);