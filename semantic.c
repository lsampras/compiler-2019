

#include"astdef.h"
#include"symtabledef.h"
#include"symtable.h"
#include"lexer.h"
#include<stdbool.h>
#include"ast.h"
#include"semanticdef.h"

arith_check checkArithCompat(astnode arith,IdTable scope,ErrorList errors){
    arith_check temp;
    temp.rec = NULL;
    temp.isError = false;
    temp.type = -1;
    switch (arith->label.data.term)
    {
        case MUL:
        case DIV:{
            arith_check a = checkArithCompat(arith->children,scope,errors);
            arith_check b = checkArithCompat(arith->children->next,scope,errors);
            if(a.isError || b.isError){
                temp.isError = true;
                return temp;
            }
            if(a.type == RECORDT && b.type == RECORDT){

                //error
                temp.isError = true;
                return temp;
            }
            return a;
        }
        case PLUS:
        case MINUS:{
            arith_check a = checkArithCompat(arith->children,scope,errors);
            arith_check b = checkArithCompat(arith->children->next,scope,errors);
            if(a.isError || b.isError){
                temp.isError = true;
                return temp;
            }
            if(a.type == RECORDT && b.type == RECORDT){
                if(a.rec != b.rec){
                    //error
                    temp.isError = true;
                    return temp;
                }else{
                    return a;
                }
            }else if(a.type == RECORDT || b.type == RECORDT){
                //error
                temp.isError = true;
                return temp;
            }else{
                return a;
            }
        }
        case NUM:{
            temp.type = RNUM;
            return temp;
        }
        case RNUM:{
            temp.type = RNUM;
            return temp;
        }
        case ID:{
            idnode tempid = checkId(arith->lex,scope);
            if(tempid == NULL){
                tempid = checkIdGlobal(arith->lex);
            }
            if(tempid==NULL){
                temp.isError = true;
                return temp;
            }else{
                if(tempid->type == RECORDT){
                    if(arith->children != NULL){
                        if(arith->children->children->next != NULL){
                            if(!isRecSubType(tempid->rec,arith->children->lex)){
                                //invalid rec subfield
                                temp.isError = true;
                                return temp;
                            }else{
                                temp.type = getRecSubType(tempid->rec,arith->children->lex);
                                return temp;
                            }
                        }else{
                            temp.type = RECORDT;
                            return temp;
                        }
                    }
                }
            }

        }

    
        default:
            break;
    }
}

void checkarithmetic(astnode arith,ErrorList errors,IdTable scope);
void checkarithmetic(astnode arith,ErrorList errors,IdTable scope){
    switch (arith->label.data.term)
    {
        case MUL:
        case DIV:
        case PLUS:
        case MINUS:{
            checkarithmetic(arith->children,errors,scope);
            checkarithmetic(arith->children->next,errors,scope);
        }
        case ID:{
            idnode temp = checkId(arith->lex,scope);
            if(temp == NULL){
                temp = checkIdGlobal(arith->lex);
            }
            if(temp==NULL){
                //error handling
            }else{
                if(temp->type == RECORDT){
                    if(arith->children != NULL){
                        if(arith->children->children->next != NULL){
                            if(!isRecSubType(temp->rec,arith->children->lex)){
                                //invalid rec subfield
                            }
                        }
                    }
                }
            }
        }
    
        default:
            break;
    }
}

void clearBoolUpdateFlag(astnode booleanexp,IdTable scope);
void clearBoolUpdateFlag(astnode booleanexp,IdTable scope){
    switch(booleanexp->label.data.term){
        case AND:
        case OR:
        {
            clearBoolUpdateFlag(booleanexp->children,scope);
            clearBoolUpdateFlag(booleanexp->children->next,scope);
            break;
        }
        case NOT:{
            clearBoolUpdateFlag(booleanexp->children,scope);
            break;
        }
        case LT:
        case LE:
        case EQ:
        case GT:
        case GE:
        case NE:{
            idnode a = NULL;
            if(booleanexp->children->label.data.term == ID){
                a = checkId(booleanexp->children->lex,scope);
                if(a == NULL){
                    a = checkIdGlobal(booleanexp->children->lex);
                }
                if(a!=NULL){
                    a->updated = false;
                }
            }
            if(booleanexp->children->next->next->label.data.term == ID){
                a = checkId(booleanexp->children->next->next->lex,scope);
                if(a == NULL){
                    a = checkIdGlobal(booleanexp->children->next->next->lex);
                }
                if(a!=NULL){
                    a->updated = false;
                }
            }
        }
    }
}

bool checkBoolStmts(astnode boolexp,ErrorList errors,IdTable scope);
bool checkBoolStmts(astnode boolexp,ErrorList errors,IdTable scope){
    bool b1 = false;
    bool b2 = false;
    switch(boolexp->label.data.term){
        case AND:
        case OR:
        {
            b1 = checkBoolStmts(boolexp->children,errors,scope);
            b2 = checkBoolStmts(boolexp->children->next,errors,scope);
            return b1||b2;
            break;
        }
        case NOT:{
            return checkBoolStmts(boolexp->children,errors,scope);
            break;
        }
        case LT:
        case LE:
        case EQ:
        case GT:
        case GE:
        case NE:{
            idnode a = NULL;
            if(boolexp->children->label.data.term == ID){
                a = checkId(boolexp->children->lex,scope);
                if(a == NULL){
                    a = checkIdGlobal(boolexp->children->lex);
                }
                if(a==NULL){
                    //undeclared var
                }
                b1 = a->updated;
            }
            if(boolexp->children->next->next->label.data.term == ID){
                a = checkId(boolexp->children->next->next->lex,scope);
                if(a == NULL){
                    a = checkIdGlobal(boolexp->children->next->next->lex);
                }
                if(a==NULL){
                    //undeclared var
                }
                b2 = a->updated;
            }
            return b2 || b1;
        }
    }

}
bool checkparams(params p,astnode list){

}

void printSymbolTable();
void checkStmtErr(astnode stmt,ErrorList errors,IdTable scope);
void checkStmtErr(astnode stmt,ErrorList errors,IdTable scope){
    if(stmt == NULL){
        return;
    }
    idnode temp;
    switch(stmt->label.data.nonterm){
        case assignmentStmt:{
            temp = checkId(stmt->children->children->lex,scope);//RHS
            if (temp == NULL){
                temp = checkIdGlobal(stmt->children->children->lex);
                if(temp == NULL){
                    //undefined var RHS
                }
            }
            enum types temtype = temp->type;
            if(temp!=NULL && temp->type == RECORDT){
                if(stmt->children->children->next !=NULL){
                    if(!isRecSubType(temp->rec,stmt->children->children->next->lex)){
                        //invalid sub field
                    }else{
                        temtype = getRecSubType(temp->rec,stmt->children->children->next->lex);
                    }
                }
            }
            arith_check ac = checkArithCompat(stmt->children->next,errors,scope);
            if(!ac.isError){
                if(ac.type == RECORDT || temtype == RECORDT){
                    if(ac.type == RECORDT && temtype == RECORDT){
                        if(ac.rec != temp->rec){
                            //different types of results assignned
                        }
                    }else{
                        if(ac.type == RECORDT){
                            //error 1
                        }else{
                            // error 2
                        }
                    }

                }
            }
            checkarithmetic(stmt->children->next,errors,scope);
            if(temp!=NULL){
                // temp->assigned = true;
                temp->updated = true;
            }
            break;
        }
        case conditionalStmt:{
            checkBoolStmts(stmt->children->children,errors,scope);//check bool
            checkStmtErr(stmt->children->next->children,errors,scope);//check if part
            checkStmtErr(stmt->children->next->next->children,errors,scope);//check else part
            break;
        }
        case iterativeStmt:{
            clearBoolUpdateFlag(stmt->children->children,scope);

            checkStmtErr(stmt->children->next->children,errors,scope);

            if(!checkBoolStmts(stmt->children->children,errors,scope)){
                //value not updated
            }



            break;
        }
        case funCallStmt:{
            SymNode s = getSymNode(stmt->children->next->lex);
            if(s!=NULL){
                if(!checkparams(s->input,stmt->children->next->next->children)){
                    //input params not matching
                }
                if(!checkparams(s->output,stmt->children->children)){
                    //output params not matching
                }

            }else{
                //function not existing
            }
            break;
        }


        case ioStmt:{
            if(stmt->children->label.is_leaf && stmt->children->label.data.term == WRITE){
                if(stmt->children->next->label.is_leaf){
                    if(stmt->children->next->label.data.term == NUM || stmt->children->next->label.data.term == RNUM ){
                        //nothing to be done here
                        break;
                    }
                    // if(stmt->children->next->label.data.term = ID)

                }else
                {
                    if(stmt->children->next->label.data.nonterm = allVar ){
                        temp = checkId(stmt->children->next->children->lex,scope);
                        if(temp==NULL){
                            temp = checkIdGlobal(stmt->children->next->children->lex);
                            // if(temp)
                        }
                        if(temp == NULL){
                            //id does not exists
                        }
                        if(temp->type == RECORDT){
                            if(stmt->children->next->children->next == NULL){
                                //no recordsubtype (no error)
                            }else{
                                if(!isRecSubType(temp->rec,stmt->children->next->children->next->lex)){
                                    //invalid rec sub type
                                }
                            }
                        }
                    }
                }
                
            }
            break;  
        }
        case returnStmt:{
            astnode t = stmt->children;
            while(t!=NULL){
                if(!checkId(t->lex,scope)){
                    if(!checkIdGlobal(t->lex)){
                        //id does not exist
                    }
                }
                t = t->next;
            }

            break;
        }
        default:{
            break;
        }
    }

    checkStmtErr(stmt->next,errors,scope);
}

void thirdPass(astnode root,ErrorList errors){
    astnode temp = root->children;
    SymNode a = NULL;
    while(temp!=NULL){
        a = addToSymbolTable(temp,errors);
        if(temp->label.data.nonterm = mainFunction){
            checkStmtErr(temp->children->next->next->next->next->next->children,errors,a->scope);//other stmts
            checkStmtErr(temp->children->next->next->next->next->next->next,errors,a->scope);//return stmt
        }else{
            checkStmtErr(temp->children->next->next->next->next->next->children,errors,a->scope);//other stmts
            checkStmtErr(temp->children->next->next->next->next->next->next,errors,a->scope);//return stmt
        }

        temp = temp->next;
    }
    printSymbolTable();
}
