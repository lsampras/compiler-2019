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
#include <time.h>
int main(int argc,char ** argv){

    if(argc!=3){
        printf("invalid number of arguments\nUsage: ./stage1exe testcase.txt parsetreeOutFile.txt");
        return 1;
    }


    FILE *in,*parsetree;
    clock_t start_time,end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    printf("\n\nBoth Lexical and Syntax Analysis Modules Implemented\n - with error handling\n\n");
    int opt = -1;
    do{
        printf("\nEnter Option:");
        scanf(" %d", &opt);

        switch(opt){
            case 0:
                printf("exiting\n");
                return 0;

            case 1:
                in = fopen(argv[1],"rb");
                removeComments(in,NULL);
                fclose(in);
                break;

            case 2:
                in = fopen(argv[1],"rb");
                getStream(in);
                seedHashTable();
                while(!file_empty()){
                    printToken(getNextToken());
                }
                freeHashTable();
                fclose(in);
                break;

            case 3:
                in = fopen(argv[1],"rb");
                parsetree = fopen(argv[2],"wb");
                getStream(in);
                seedHashTable();
                seedSymbolTable();
                readGrammar("grammar.txt");
                ComputeFirstAndFollowSets();
                initParseTable();
                initTree();
                printTree(parsetree);
                freeHashTable();
                freesymbolTable();//i mean nonterminal table
                freeGrammar();
                fclose(parsetree);
                fclose(in);
                break;
            case 4:
                start_time = clock();
                // invoke your lexer and parser here

                in = fopen(argv[1],"rb");
                parsetree = fopen(argv[2],"wb");
                getStream(in);
                seedHashTable();
                seedSymbolTable();
                readGrammar("grammar.txt");
                ComputeFirstAndFollowSets();
                initParseTable();
                initTree();
                printTree(parsetree);
                freeHashTable();
                freesymbolTable();//i mean nonterminal table
                freeGrammar();
                fclose(parsetree);
                fclose(in);

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC ;
                printf("total_CPU_time %f\n",total_CPU_time);
                printf("total_CPU_time_in_seconds %f\n",total_CPU_time_in_seconds);
                break;

            case 9://debug
                in = fopen(argv[1],"rb");
                parsetree = fopen(argv[2],"wb");
                getStream(in);
                seedHashTable();
                seedSymbolTable();
                readGrammar("grammar.txt");
                ComputeFirstAndFollowSets();
                initParseTable();
                struct narytree* p = initTree();
                struct astNode ast = initAST(p);


                firstPass(&ast);
                printrecordtable();

                printTree(parsetree);
                freeHashTable();
                freesymbolTable();//i mean nonterminal table
                freeGrammar();
                fclose(parsetree);
                fclose(in);
                break;

                break;



            default:
                printf("invalid option.Try again\n");
                
        }

    }while(1);

    fclose(in);
    return 0;
}
