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
// void printGlobalTable();