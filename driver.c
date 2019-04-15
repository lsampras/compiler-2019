/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include<stdio.h>
#include"lexer.h"
#include"parser.h"
#include"ast.h"
#include"symtable.h"
#include"semantic.h"
#include <time.h>
int main(int argc,char ** argv){

    if(argc!=3){
        printf("invalid number of arguments\nUsage: ./stage1exe testcase.txt code.nasm");
        return 1;
    }


    FILE *in,*parsetree;
    clock_t start_time,end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    struct narytree* p;
    struct astNode ast;
    printf("\n\nBoth Lexical and Syntax Analysis Modules Implemented\n - with error handling\n\n");
    int opt = -1;
    do{
        printf("\nEnter Option:");
        scanf(" %d", &opt);

        switch(opt){
            case 0:
                printf("exiting\n");
                return 0;

            case 1://print token list
                in = fopen(argv[1],"rb");
                getStream(in);
                seedHashTable();
                while(!file_empty()){
                    printToken(getNextToken());
                }
                freeHashTable();
                fclose(in);
                break;

            case 2://print parsetree inroder on console
                in = fopen(argv[1],"rb");
                // parsetree = fopen(argv[2],"wb");
                getStream(in);
                seedHashTable();
                seedSymbolTable();
                readGrammar("grammar.txt");
                ComputeFirstAndFollowSets();
                initParseTable();
                initTree();
                printTree();
                freeHashTable();
                freesymbolTable();
                freeGrammar();
                // fclose(parsetree);
                fclose(in);
                break;
            case 3://print AST on console
                in = fopen(argv[1],"rb");
                // parsetree = fopen(argv[2],"wb");
                getStream(in);
                seedHashTable();
                seedSymbolTable();
                readGrammar("grammar.txt");
                ComputeFirstAndFollowSets();
                initParseTable();
                p = initTree();
                if(parseSuccess){
                    ast = initAST(p);
                    printf("\n\n---Printing AST in InOrder traversal---\n\n");
                    printAST(&ast,NULL);
                    freeAST(&ast);
                }else{
                    printf("parsetree construction failed\nAst not created\n");
                }
                // printTree();
                freeparsetree(p);

                freeHashTable();
                freesymbolTable();
                freeGrammar();
                // fclose(parsetree);
                fclose(in);
                break;


            case 4://compression ratio
                in = fopen(argv[1],"rb");
                // parsetree = fopen(argv[2],"wb");
                getStream(in);
                seedHashTable();
                seedSymbolTable();
                readGrammar("grammar.txt");
                ComputeFirstAndFollowSets();
                initParseTable();
                p = initTree();
                if(parseSuccess){
                    ast = initAST(p);
                    int a = getParsenodes();
                    int b = getastnodes();
                    int a1 =a*sizeof(struct narytree);
                    int b1 =b*sizeof(struct narytree);
                    printf("Parse tree Number of nodes: %d\tAllocated memory :%d\n",a,a1);
                    printf("AST Number of nodes: %d\tAllocated memory :%d\n",b,b1);
                    int comp = (((float)a1-(float)b1)/(float)a1)*100;
                    printf("Compression percentage = %d\n",comp);
                    // printf("\n\n---Printing AST in InOrder travdersal---\n\n");
                    // printAST(&ast,NULL);
                    freeAST(&ast);
                }else{
                    printf("parsetree construction failed\nAst not created\n");
                }
                // printTree();
                freeparsetree(p);

                freeHashTable();
                freesymbolTable();
                freeGrammar();
                // fclose(parsetree);
                fclose(in);
                break;

    

        case 5://print Symbol table
            in = fopen(argv[1],"rb");
            // parsetree = fopen(argv[2],"wb");
            getStream(in);
            seedHashTable();
            seedSymbolTable();
            readGrammar("grammar.txt");
            ComputeFirstAndFollowSets();
            initParseTable();
            p = initTree();
            if(parseSuccess){
                ast = initAST(p);
                // printf("\n\n---Printing AST in InOrder travdersal---\n\n");
                // printAST(&ast,NULL);
                struct errorList errors;
                errors.head = NULL;
                errors.tail = NULL;
                firstPass(&ast,&errors);
                // printrecordtable();

                secondPass(&ast,&errors);
                // printGlobalTable();
                thirdPass(&ast,&errors);
                printSymbolTable();
                // printfuncsize();

                freeAST(&ast);
                freerecordtable();
                freeGlobalTable();
                freeBigTable();//the symbol table
            }else{
                printf("parsetree construction failed\nAst not created\n");
            }
            // printTree();
            freeparsetree(p);

            freeHashTable();
            freesymbolTable();
            freeGrammar();
            // fclose(parsetree);
            fclose(in);
            break;


        case 6://print global table
            in = fopen(argv[1],"rb");
            // parsetree = fopen(argv[2],"wb");
            getStream(in);
            seedHashTable();
            seedSymbolTable();
            readGrammar("grammar.txt");
            ComputeFirstAndFollowSets();
            initParseTable();
            p = initTree();
            if(parseSuccess){
                ast = initAST(p);
                // printf("\n\n---Printing AST in InOrder travdersal---\n\n");
                // printAST(&ast,NULL);
                struct errorList errors;
                errors.head = NULL;
                errors.tail = NULL;
                firstPass(&ast,&errors);
                // printrecordtable();

                secondPass(&ast,&errors);
                printGlobalTable();
                // thirdPass(&ast,&errors);
                // printSymbolTable();
                // printfuncsize();
                freerecordtable();
                freeGlobalTable();

                freeAST(&ast);
            }else{
                printf("parsetree construction failed\nAst not created\n");
            }
            // printTree();
            freeparsetree(p);

            freeHashTable();
            freesymbolTable();
            freeGrammar();
            // fclose(parsetree);
            fclose(in);
            break;


        case 7://print funcs and size
            in = fopen(argv[1],"rb");
            // parsetree = fopen(argv[2],"wb");
            getStream(in);
            seedHashTable();
            seedSymbolTable();
            readGrammar("grammar.txt");
            ComputeFirstAndFollowSets();
            initParseTable();
            p = initTree();
            if(parseSuccess){
                ast = initAST(p);
                // printf("\n\n---Printing AST in InOrder travdersal---\n\n");
                // printAST(&ast,NULL);
                struct errorList errors;
                errors.head = NULL;
                errors.tail = NULL;
                firstPass(&ast,&errors);
                // printrecordtable();

                secondPass(&ast,&errors);
                // printGlobalTable();
                thirdPass(&ast,&errors);
                // printSymbolTable();
                printfuncsize();
                freerecordtable();
                freeGlobalTable();
                freeBigTable();

                freeAST(&ast);
            }else{
                printf("parsetree construction failed\nAst not created\n");
            }
            // printTree();
            freeparsetree(p);

            freeHashTable();
            freesymbolTable();
            freeGrammar();
            // fclose(parsetree);
            fclose(in);
            break;


        case 8://print rectable
            in = fopen(argv[1],"rb");
            // parsetree = fopen(argv[2],"wb");
            getStream(in);
            seedHashTable();
            seedSymbolTable();
            readGrammar("grammar.txt");
            ComputeFirstAndFollowSets();
            initParseTable();
            p = initTree();
            if(parseSuccess){
                ast = initAST(p);
                // printf("\n\n---Printing AST in InOrder travdersal---\n\n");
                // printAST(&ast,NULL);
                struct errorList errors;
                errors.head = NULL;
                errors.tail = NULL;
                firstPass(&ast,&errors);
                printrecordtable();

                // secondPass(&ast,&errors);
                // printGlobalTable();
                // thirdPass(&ast,&errors);
                // printSymbolTable();
                // printfuncsize();
                freerecordtable();

                freeAST(&ast);
            }else{
                printf("parsetree construction failed\nAst not created\n");
            }
            // printTree();
            freeparsetree(p);

            freeHashTable();
            freesymbolTable();
            freeGrammar();
            // fclose(parsetree);
            fclose(in);
            break;
    
        case 9://print semantic errors
            in = fopen(argv[1],"rb");
            // parsetree = fopen(argv[2],"wb");
            getStream(in);
            seedHashTable();
            seedSymbolTable();
            readGrammar("grammar.txt");
            ComputeFirstAndFollowSets();
            initParseTable();
            p = initTree();
            if(parseSuccess){
                ast = initAST(p);
                // printf("\n\n---Printing AST in InOrder travdersal---\n\n");
                // printAST(&ast,NULL);
                struct errorList errors;
                errors.head = NULL;
                errors.tail = NULL;
                firstPass(&ast,&errors);
                // printrecordtable();

                secondPass(&ast,&errors);
                // printGlobalTable();
                thirdPass(&ast,&errors);
                // printSymbolTable();
                if(errors.head != NULL){
                    printErrors(&errors);
                }else{
                    //no errors
                    printf("no semantic errors\n");
                }
                // printfuncsize();

                freeAST(&ast);
                freerecordtable();
                freeBigTable();//the symbol table
                freeGlobalTable();
            }else{
                printf("parsetree construction failed\nAst not created\n");
            }
            // printTree();
            freeparsetree(p);

            freeHashTable();
            freesymbolTable();
            freeGrammar();
            // fclose(parsetree);
            fclose(in);
            break;




            default:
                printf("invalid option.Try again\n");
                
        }

    }while(1);

    fclose(in);
    return 0;
}
