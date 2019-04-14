
#include"astdef.h"
#include"symtabledef.h"
#include"lexer.h"
#include<stdbool.h>
#include"ast.h"
#include"semanticdef.h"
#define REALWIDTH 4
#define INTWIDTH 2
recTable *Records[RECSIZE];
SymTable *Symbols[SYMSIZE];
struct idNode  *global[IDSIZE];


// Id Table funcs

// global[IDSIZE] == NULL

idnode checkId(char * id,IdTable scope){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = scope[key];
    while(1){
        if(a == NULL){
            return NULL;
        }
        if(strcmp(id,a->id)==0){
            return a;
        }
        a = a->next;
    }
}
idnode checkIdGlobal(char * id){
    unsigned long key = gethash(id);
    key = key % IDSIZE;
    idnode a = global[key];
    while(1){
        if(a == NULL){
            return NULL;
        }
        if(strcmp(id,a->id)==0){
            return a;
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
    rec->width = 0;
    astnode temp = record->children->next;
    Rec recpair = NULL;
    while(temp!=NULL){
        recpair = (Rec)malloc(sizeof(struct recordtypenode));
        recpair->id = clone(temp->children->next->lex);
        recpair->offset = rec->width;
        if(temp->children->label.is_leaf && temp->children->label.data.term == REAL){
            recpair->type = REALT;
            rec->width = rec->width+REALWIDTH;
        }else{
            recpair->type = INTEGER;
            rec->width = rec->width+INTWIDTH;
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

bool isRecSubType(Rnode rec,char* field){
    Rec temp = rec->child;
    while(temp!=NULL){
        if(strcmp(temp->id,field)==0){
            return true;
        }
    }
    return false;
}

enum types getRecSubType(Rnode rec,char* field){
    Rec temp = rec->child;
    while(temp!=NULL){
        if(strcmp(temp->id,field)==0){
            return temp->type;
        }
    }
    return -1;
}


// SYMBOL table funcs
// SymNode addToSymTable(){
//     SymNode a = (SymNode)malloc(sizeof(struct symnode));
//     a->id = funid;
//     a->input = input;
//     a->output = output;
//     unsigned long key = gethash(funid);
//     key = key % SYMSIZE;
//     Symbols[key] = a;
//     return a;
// }
void addToIdTable(IdTable scope,astnode var,ErrorList errors,int* offset){

    if(checkId(var->children->next->lex,scope) != NULL || checkId(var->children->next->lex,global)!=NULL){
        //error for duplicate var
        return;


    }
    idnode node = (idnode)malloc(sizeof(struct idNode));
    unsigned long hash = gethash(var->children->next->lex);
    hash = hash%IDSIZE;

    node->id = clone(var->children->next->lex);
    var->children->next->current_scope = scope;
    node->assigned = false;
    node->updated = false;
    node->next = NULL;
    node->rec = NULL;
    
    if(var->children->label.is_leaf && var->children->label.data.term == RECORD){
        //is a record type
        node->rec = checkRecords(var->children->children->lex);
        if(node->rec==NULL){
            //handle type not existing error
        }else{
            *offset = *offset + node->rec->width;
        }
        node->type = RECORDT;
    }else if(var->children->label.data.term == INT){
        node->type = INTEGER;
        *offset = *offset + INTWIDTH;
    }else{
        node->type = REALT;
        *offset = *offset + REALWIDTH;
    }
    node->next = scope[hash];
    scope[hash] = node;
}

params getParams(astnode list,IdTable scope,ErrorList errors,int * width,bool inp);
params getParams(astnode list,IdTable scope,ErrorList errors,int * width,bool inp){
    
    if(list == NULL){
        return NULL;
    }
    if(checkId(list->next->lex,scope)!=NULL){
        //handle duplicate
    }
    idnode node = (idnode)malloc(sizeof(struct idNode));
    unsigned long hash = gethash(list->next->lex);
    hash = hash%IDSIZE;

    node->id = clone(list->next->lex);
    list->next->current_scope = scope;
    node->assigned = inp;
    node->next = NULL;
    node->rec = NULL;

    params p = (params)malloc(sizeof(struct parameterslist));
    p->rec = NULL;
    if(list->label.is_leaf && list->label.data.term == INT){
        p->type = INTEGER;
        *width = *width + INTWIDTH;
    }else if(list->label.is_leaf && list->label.data.term == REAL){
        p->type = REALT;
    }else{
        p->type = RECORDT;
        p->rec = checkRecords(list->children->lex);
        *width = *width + p->rec->width;
        if(p->rec==NULL){
            //handle missing record type
        }
    }
    node->rec = p->rec;
    node->type = p->type;
    node->next = scope[hash];
    scope[hash] = node;
    
    p->next = getParams(list->next->next,scope,errors,width,inp);
    return p;

}

SymNode addToSymbolTable(astnode func,ErrorList errors){
    SymNode a = (SymNode) malloc(sizeof(struct symnode));
    a->id = NULL;
    a->width = 0;
    a->scope = (IdTable)calloc(IDSIZE,sizeof(idnode*));
    a->input = NULL;
    a->output = NULL;
    astnode dec = NULL;
    if(!func->label.is_leaf && func->label.data.nonterm == function ){
        a->input = getParams(func->children->next->children,a->scope,errors,&(a->width),true);
        a->output = getParams(func->children->next->next->children,a->scope,errors,&(a->width),false);
        dec = func->children->next->next->next->next->children;
        a->id = clone(func->children->lex);
    }else if(!func->label.is_leaf && func->label.data.nonterm == mainFunction){
        dec = func->children->next->children;
        a->id = (char*)calloc(6,sizeof(char));
        a->id = "_main";
    }
    while(dec!=NULL){
        addToIdTable(a->scope,dec,errors,&(a->width));
        dec = dec->next;
    }
    unsigned long hash = gethash(a->id);
    hash = hash%SYMSIZE;
    a->next = Symbols[hash];
    Symbols[hash] = a;
    return a;
}

SymNode getSymNode(char * funid){
    unsigned long key = gethash(funid);
    key = key % SYMSIZE;
    SymNode a = Symbols[key];
    while(a!=NULL){
        if(strcmp(a->id,funid)==0){
            return a;
        }
    }
    return NULL;
}


void addToGlobal(astnode var,int * offset){

    if(checkId(var->children->next->lex,global)!=NULL){
        //error for duplicate var
    }
    idnode node = (idnode)malloc(sizeof(struct idNode));
    unsigned long hash = gethash(var->children->next->lex);
    var->children->next->offset = *offset;
    hash = hash%IDSIZE;
    node->id = clone(var->children->next->lex);
    var->children->next->current_scope = global;
    node->assigned = false;
    node->next = NULL;
    node->rec = NULL;
    
    if(var->children->label.is_leaf && var->children->label.data.term == RECORD){
        //is a record type
        node->rec = checkRecords(var->children->children->lex);
        if(node->rec==NULL){
            //handle type not existing error
        }else{
            *offset = *offset + node->rec->width;
        }
        node->type = RECORDT;
    }else if(var->children->label.data.term == INT){
        node->type = INTEGER;
        *offset = *offset + INTWIDTH;
    }else{
        node->type = REALT;
        *offset = *offset + REALWIDTH;
    }
    node->next = global[hash];
    global[hash] = node;
}

void fillGlobaltable(astnode declarations){
//fill global variables
    astnode temp = declarations->children;
    int offset = 0;
    while(temp!=NULL){
        if(temp->children->next->next !=NULL){//Global node exists
            addToGlobal(temp,&offset);
        }
        temp = temp->next;
    }
}
void fillRecordTypes(astnode typeddefinitions){
//fill record type table
    astnode record = typeddefinitions->children;
    while(record!=NULL){
        addRecType(record);
        record = record->next;
    }
}

void firstPass(astnode root,ErrorList errors){
    astnode temp = root->children;
    while(temp!=NULL){
        if(!temp->label.is_leaf && temp->label.data.nonterm == function ){
            fillRecordTypes(temp->children->next->next->next);
        }else if(!temp->label.is_leaf && temp->label.data.nonterm == mainFunction){
            fillRecordTypes(temp->children);
        }
        temp = temp->next;
    }
    
}


void secondPass(astnode root,ErrorList errors){
    astnode temp = root->children;
    while(temp!=NULL){
        if(!temp->label.is_leaf && temp->label.data.nonterm == function ){
            fillGlobaltable(temp->children->next->next->next->next);
        }else if(!temp->label.is_leaf && temp->label.data.nonterm == mainFunction){
            fillGlobaltable(temp->children->next);
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
    if(record->next !=NULL){
        printrecord(record->next);
    }
}

void printrecordtable(){
    printf("\n--Printing Record Table--\n");
    int i=0;
    for(;i<RECSIZE;i++){
        if(Records[i]!=NULL){
            printrecord(Records[i]);
        }
    }
    printf("--End Of Record Table--\n\n");
}


void printglobal(idnode global){
    printf("%s\t\t",global->id);
    if(global->type == REALT){
        printf("type:REAL\t");
    }
    if(global->type == INTEGER){
        printf("type:INT\t");
    }
    if(global->type == RECORDT){
        printf("type:RECORD\t");
    }
    if(global->assigned){
        printf("assigned\t");
    }else{
        printf("not assigned\t");
    }
    if(global->rec!=NULL){
        printf("rec type%s",global->rec->typeclass);
    }
    printf("\n");
    if(global->next!=NULL){
        printglobal(global->next);
    }

}

void printGlobalTable(){
    printf("\n--Printing GLobal Table--\n");
    int i=0;
    for(;i<IDSIZE;i++){
        if(global[i]!=NULL){
            printglobal(global[i]);
        }
    }
    printf("--End Of GLobal Table--\n\n");
}


void printIdTable(IdTable scope,char* id){
    // if(scope)
    printf("\n--Printing ID Table of %s--\n",id);
    int i=0;
    for(;i<IDSIZE;i++){
        if(scope[i]!=NULL){
            printglobal(scope[i]);
        }
    }
    printf("--End Of ID Table--\n\n");
}

void printSymbolTable(){
    int i = 0;
    for(;i<SYMSIZE;i++){
        if(Symbols[i]!=NULL){
            printIdTable(Symbols[i]->scope,Symbols[i]->id);
        }
    }
}