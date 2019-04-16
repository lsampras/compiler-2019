/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include "ast.h"
#include "astdef.h"
#include "semantic.h"
#include "symtable.h"

FILE* fp;
int jump_points = 0;

void initialization(){
    fprintf(fp,"segment\t.text");
}
void add_declaration(char* id,int width){
    fprintf(fp,"%s resb %d\n",id,width);
}
void add_declarations(astnode decs,IdTable scope){
    fprintf(fp,"section .bss\n\tbuffer1 resb 50\n\tbuffer2 resb 50");
    int width = 0;
    while(decs!=NULL){
        idnode temp = checkId(decs->children->next->lex,scope);
        if(temp == NULL){
            temp = checkIdGlobal(decs->children->next->lex);
        }
        switch (temp->type)
        {
            case REALT:{
                width = 4;
                add_declaration(decs->children->next->lex,width);
                break;
            }
            case INTEGER:{
                width = 2;
                add_declaration(decs->children->next->lex,width);
                break;
            }
            case RECORDT:{
                Rnode r = checkRecords(decs->children->next->lex);
                Rec a = r->child;
                char fields[10];
                while(a!=NULL){
                    fprintf(fp,"%s.%s resb %d\n",decs->children->next->lex,a->id,width);
                    a = a->next;
                }
                width = r->width;
                break;
            }
        }
        decs = decs->next;
    }

}

void do_bool(astnode boolexp,FILE*fp,IdTable scope){
    switch(boolexp->label.data.term){
        case OR:{
            do_bool(boolexp->children,fp,scope);
            do_bool(boolexp->children->next,fp,scope);
            fprintf(fp,"\tpop ebx\n\t pop eax\n\tor ebx, eax\n\tpush ebx");
            break;
        }
        case AND:{
            do_bool(boolexp->children,fp,scope);
            do_bool(boolexp->children->next,fp,scope);
            fprintf(fp,"\tpop ebx\n\t pop eax\n\tand ebx, eax\n\tpush ebx");
            break;
        }
        case NOT:{
            do_bool(boolexp->children,fp,scope);
            fprintf(fp,"\tpop eax\n\tnot eax\n\tpush eax\n");
            break;
        }
        case LE:{
            fprintf(fp,"\tmov eax,dword[%s]\n\tmov ebx,dword[%s]\n",boolexp->children->lex,boolexp->children->next->lex);
            fprintf(fp,"push 0ffffh\ncmp eax,ebx\njle .rel%d\npop eax\npush 0000h\n.rel%d:\n",jump_points,jump_points);
            jump_points++;
            break;
        }
        case LT:{
            fprintf(fp,"\tmov eax,dword[%s]\n\tmov ebx,dword[%s]\n",boolexp->children->lex,boolexp->children->next->lex);
            fprintf(fp,"push 0ffffh\ncmp eax,ebx\njl .rel%d\npop eax\npush 0000h\n.rel%d:\n",jump_points,jump_points);
            jump_points++;
            break;
        }
        case GE:{
            fprintf(fp,"\tmov eax,dword[%s]\n\tmov ebx,dword[%s]\n",boolexp->children->lex,boolexp->children->next->lex);
            fprintf(fp,"push 0ffffh\ncmp eax,ebx\njge .rel%d\npop eax\npush 0000h\n.rel%d:\n",jump_points,jump_points);
            jump_points++;
            break;
        }
        case GT:{
            fprintf(fp,"\tmov eax,dword[%s]\n\tmov ebx,dword[%s]\n",boolexp->children->lex,boolexp->children->next->lex);
            fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njg .rel%d\npop eax\npush 0000h\n.rel%d:\n",jump_points,jump_points);
            jump_points++;
            break;
        }
        case NE:{
            fprintf(fp,"\tmov eax,dword[%s]\n\tmov ebx,dword[%s]\n",boolexp->children->lex,boolexp->children->next->lex);
            fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\njne .rel%d\npop eax\npush 0000h\n.rel%d:\n",jump_points,jump_points);
            jump_points++;
            break;
        }
        case EQ:{
            fprintf(fp,"\tmov eax,dword[%s]\n\tmov ebx,dword[%s]\n",boolexp->children->lex,boolexp->children->next->lex);
            fprintf(fp,"pop ebx\npop eax\npush 0ffffh\ncmp eax,ebx\nje .rel%d\npop eax\npush 0000h\n.rel%d:\n",jump_points,jump_points);
            jump_points++;
            break;
        }
    }
}

int do_arith(astnode arith){
    switch(arith->label.data.term){
        case PLUS:{
            int a = do_arith(arith->children);
            do_arith(arith->children->next);
            fprintf(fp,"\tpop eax\n\tpop ebx\n");
            fprintf(fp,"\t add eax ebx\n\tpush eax\n");
            break;
            
        }
        case MINUS:{
            do_arith(arith->children);
            do_arith(arith->children->next);
            fprintf(fp,"\tpop eax\n\tpop ebx\n");
            fprintf(fp,"\t sub eax ebx\n\tpush eax\n");
            break;
        }
        case DIV:{
            do_arith(arith->children);
            do_arith(arith->children->next);
            fprintf(fp,"\tmov edx, 0\n\tpop eax\n\tpop ebx\n");
            fprintf(fp,"\tdiv ebx\n\tpush eax\n");
            break;
        }
        case MUL:{
            do_arith(arith->children);
            do_arith(arith->children->next);
            fprintf(fp,"\tpop eax\n\tpop edx\n");
            fprintf(fp,"\t imul edx\n\tpush eax");
            break;
        }
        case NUM:{
            fprintf(fp,"\tmov eax,%s\n\tpush eax\n",arith->lex);
            break;
        }
        case ID:{
            fprintf(fp,"\tmov eax dword[%s]\n",arith->lex);
            //tbd records
        }
    }
}
void do_assignment(astnode assignstmt,IdTable scope){
    do_arith(assignstmt->children->next);
    idnode a = checkId(assignstmt->children->children->lex,scope);
    if(a == NULL){
         a = checkIdGlobal(assignstmt->children->children->lex);
    }
    enum types type = a->type;
    int n = do_arith(assignstmt->children->next);
    if(a->type == RECORDT){
        if(assignstmt->children->children->next!=NULL){
            int offset = getRecSubOffset(a->rec,assignstmt->children->children->next);
            type = getRecSubType(a->rec,assignstmt->children->children->next);
            if(type == INTEGER){
                fprintf(fp,"\tmov dword[%s+%d] eax\n",assignstmt->children->children->lex,offset);
            }
        }else{
            

        }
    }
    if(a->type == INTEGER){
        fprintf(fp,"\tmov dword[%s] eax\n",assignstmt->children->children->lex);
    }
}

void evaluate_statements(astnode stmts,IdTable scope){
    if(stmts == NULL){
        return;
    }
    switch(stmts->label.data.nonterm){
        case assignmentStmt:{

            break;
        }
        case conditionalStmt:{

            break;
        }
        case iterativeStmt:{

            break;
        }
        case ioStmt:{

            break;
        }
        default: break;
    }
    evaluate_statements(stmts->next);
}

void generate_code(astnode ast){
    //declare variables
    add_declarations(ast->children->next->children,getSymNode("_main"));

    //evaluate assignments
}

