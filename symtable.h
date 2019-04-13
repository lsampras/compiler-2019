#include"astdef.h"
#include"symtabledef.h"
void firstPass(astnode root);
void printrecordtable();
void secondPass(astnode root);
void printGlobalTable();
void thirdPass(astnode root);
void printIdTable(IdTable scope,char* id);
// void printGlobalTable();