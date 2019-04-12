
#include"astdef.h"
#include"symtabledef.h"
#include"lexer.h"
#include<stdbool.h>
#include"ast.h"
recTable *Records[RECSIZE];
SymTable *Symbols[SYMSIZE];
struct idNode  *global[IDSIZE];


// Id Table funcs
idnode createIdNode(char * id,enum types type,Rnode recptr){
    idnode a = (idnode)malloc(sizeof(struct idNode));
    a->id = id;
    a->type = type;
    a->assigned = false;
    a->next = NULL;
    a->rec = recptr;
    return a;
}

IdTable addIdtoScope(char * id,Rnode recptr,enum types type, IdTable scope){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = createIdNode(id,type,recptr);
    a->next = scope[key];
    scope[key] = a;
    return scope;
}

bool checkId(char * id,IdTable scope){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = scope[key];
    while(1){
        if(a == NULL){
            return false;
        }
        if(strcmp(id,a->id)==0){
            return true;
        }
        a = a->next;
    }
}

enum types getIdType(char * id,IdTable scope){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = scope[key];
    while(1){
        if(strcmp(id,a->id)==0){
            return a->type;
        }
        a = a->next;
    }
}

bool checkIfAssigned(char * id,IdTable scope){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = scope[key];
    while(1){
        if(strcmp(id,a->id)==0){
            return a->assigned;
        }
        a = a->next;
    }
}

IdTable setAssigned(char * id,IdTable scope){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = scope[key];
    while(1){
        if(strcmp(id,a->id)==0){
            a->assigned = true;
            return scope;
        }
        a = a->next;
    }
}

//RECORD funcs
void addRecType(astnode record){
    unsigned long hash =  gethash(record->children->lex);
    hash = hash%RECSIZE;
    Rnode rec = (Rnode)malloc(sizeof(struct record));
    rec->typeclass = clone(record->children->lex);
    rec->next = NULL;
    rec->child = NULL;
    astnode temp = record->children->next;
    Rec recpair = NULL;
    while(temp!=NULL){
        recpair = (Rec)malloc(sizeof(struct recordtypenode));
        recpair->id = clone(temp->children->next->lex);
        if(temp->children->label.is_leaf && temp->children->label.data.term == REAL){
            recpair->type = REALT;
        }else{
            recpair->type = INTEGER;
        }
        recpair->next = rec->child;
        rec->child = recpair;
        temp = temp->next;
    }
    rec->next = Records[hash];
    Records[hash] = rec;
}


Rnode checkRecords(char * recid){
    unsigned long key = gethash(recid);
    key = key % RECSIZE;
    Rnode a = Records[key];
    while(1){
        if(a == NULL){
            return NULL;
        }
        if(strcmp(recid,a->typeclass)==0){
            return a;
        }
        a = a->next;
    }
}



// SYMBOL table funcs
SymNode addToSymTable(char * funid, params input,params output){
    SymNode a = (SymNode)malloc(sizeof(struct symnode));
    a->id = funid;
    a->input = input;
    a->output = output;
    unsigned long key = gethash(funid);
    key = key % SYMSIZE;
    a->next = Symbols[key];
    Symbols[key] = a;
}

SymNode getSymNode(char * funid){
    unsigned long key = gethash(funid);
    key = key % SYMSIZE;
    SymNode a = Symbols[key];
    while(1){
        if(a == NULL){
            return NULL;
        }
        if(strcmp(a->id,funid)==0){
            return a;
        }
        a = a->next;
    }
}

void fillGlobaltable(astnode declarations){
//fill global variables
}
void fillRecordTypes(astnode typeddefinitions){
//fill record type table
    astnode record = typeddefinitions->children;
    while(record!=NULL){
        addRecType(record);
        record = record->next;
    }
}

void firstPass(astnode root){
    astnode temp = root->children;
    while(temp!=NULL){
        if(!temp->label.is_leaf && temp->label.data.nonterm == function ){
            fillGlobaltable(temp->children->next->next->next->next);
            fillRecordTypes(temp->children->next->next->next);
        }else if(!temp->label.is_leaf && temp->label.data.nonterm == mainFunction){
            fillGlobaltable(temp->children->next);
            fillRecordTypes(temp->children);
        }
        temp = temp->next;
    }
    
}

void printrecord(Rnode record){
    printf("%s\t\t",record->typeclass);
    Rec temp = record->child;
    while(temp!=NULL){
        if(temp->type == INTEGER){
            printf("INT-");
        }else{
            printf("REAL-");
        }
        printf("%s\t",temp->id);
        temp = temp->next;
    }
    printf("\n");
}

void printrecordtable(){
    int i=0;
    for(;i<RECSIZE;i++){
        if(Records[i]!=NULL){
            printrecord(Records[i]);
        }
    }
}


