#include"astdef.h"
#include"symtabledef.h"
void firstPass(astnode root);
void printrecordtable();
void secondPass(astnode root);
void printGlobalTable();
void printIdTable(IdTable scope,char* id);
enum types getRecSubType(Rnode rec,char* field)
// void printGlobalTable();