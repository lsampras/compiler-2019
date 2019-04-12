#include"lexerDef.h"
#include"lexer.h"
#include"parserDef.h"
#include"ast.h"

struct astNode astroot;

char * clone(char* str){
    if(str == NULL){
        return NULL;
    }
    int len = strlen(str);
    char * dup = (char*)malloc(sizeof(char)*(len+1));
    strcpy(dup,str);
    return dup;
}

astnode createAstLeafNode(struct narytree* parsetree){
    astnode node = (astnode) malloc(sizeof(struct astNode));
    // parsetree = parsetree->children;
    node->children = NULL;
    node->current_scope = NULL;
    node->line_no = parsetree->line;
    node->lex = clone(parsetree->lexeme);
    node->ival= -1;
    node->label = parsetree->parent;
    node->next = NULL;
    node->parent = NULL;
    node->rectype = NULL;
    node->rval  = -1;
    // parsetree->ast = node;
    return node;
}
astnode createAstNonleafNode(struct narytree* parsetree){
    astnode node = (astnode) malloc(sizeof(struct astNode));
    // parsetree = parsetree->children;
    node->children = NULL;
    node->current_scope = NULL;
    node->line_no = -1;
    node->lex = NULL;
    node->ival= -1;
    node->label = parsetree->parent;
    node->next = NULL;
    node->parent = NULL;
    node->rectype = NULL;
    node->rval  = -1;
    // parsetree->ast = node;
    return node;
}

astnode converthelper(struct narytree * tree){
    if(!tree->parent.is_leaf && tree->parent.data.nonterm == term){
        return convert(converthelper(tree->children),tree->children->next);
    }
    if(!tree->parent.is_leaf && tree->parent.data.nonterm == factor){
        switch(tree->rule){
            case 62:{
                astnode a = converthelper(tree->children->next->children);
                return convert(a,tree->children->next->children->next);
            }
            case 63:{
                switch(tree->children->rule){
                    case 68:
                    case 69:{
                        return createAstLeafNode(tree->children->children);
                    }
                    case 70:{
                        astnode a = createAstLeafNode(tree->children->children);
                        if(tree->children->children->next->rule == 71){
                            a->next = createAstLeafNode(tree->children->children->next->children->next);
                        }
                        return a;
                    }
                }
            }
        }
    }
}

astnode convert(astnode term,struct narytree* exprime){
    if(exprime->children->parent.is_leaf && exprime->children->parent.data.term  == EPS){
        return term;
    }
    astnode a = createAstLeafNode(exprime->children->children);//operator
    a->children = term;//initial tree
    a->children->next = converthelper(exprime->children->next);//saanitized term/factor
    return convert(a,exprime->children->next->next);//next exp'
}

void processAstNode(struct narytree *parsetree){
    if(parsetree == NULL){
        return ;
    }

    int rule = parsetree->rule;

    switch (rule)
    {
        //empty nodes
        case 4:
        case 8:
        case 16:
        case 24:
        case 26:
        case 29:
        case 31:
        case 40:
        case 48:
        case 55:
        case 43:
        case 58:
        case 61:
        case 72:
        case 89:
        case 92:{
            parsetree->ast = NULL;
            break;
        }

        //subtree raising
        case 10:
        case 11:
        case 32:
        case 36:
        case 33:
        case 34:
        case 35:
        case 63:{
            parsetree->ast = parsetree->children->ast;
            break;
        }


        //basic leaf nodes
        case 12:
        case 13:
        case 52:
        case 53:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 76:
        case 77:
        case 78:
        case 79:
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86 :{
            parsetree -> ast = createAstLeafNode(parsetree->children);//ast also assigned inside the function
            break;
        }


        //other nodes
        case 1:{
            //program==> other main
            //raise other funcs and add main to it
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = parsetree->children->ast;
            parsetree->children->ast->last_node->next = parsetree->children->next->ast;
            parsetree->ast->children->last_node = parsetree->children->next->ast;
            break;
        }
        
        case 2:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = parsetree->children->next->ast;
            // parsetree->ast->label = parsetree->parent;
            //raise and change type to main
            break;
        }

        case 3:{
            //form a list of functions
            //and return concat list from other with func
            parsetree->ast = parsetree->children->ast;
            parsetree->ast->next = parsetree->children->next->ast;
            if(parsetree->children->next->ast == NULL){
                parsetree->ast->last_node = parsetree->children->ast;
            }else
            {
                parsetree->ast->last_node = parsetree->children->next->ast->last_node;
            }
            break;
        }
        case 5:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstLeafNode(parsetree->children);//funid
            parsetree->ast->children->next = parsetree->children->next->ast;//input parameters
            parsetree->ast->children->next->next = parsetree->children->next->next->ast;//output parameters
            parsetree->ast->children->next->next->next = parsetree->children->next->next->next->next->ast;//stmts
            parsetree->ast->children->last_node = parsetree->children->next->next->next->next->ast;
            break;
        }

        case 6:
        case 7:{
            //raise and convert param list to in/outpar
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = parsetree->children->next->next->next->next->ast;
            // parsetree->ast->label = parsetree->parent;
            break;
        }

        case 9:{
            //form a list of datatype&ids
            //and return concat list from other with func
            parsetree->ast = createAstNonleafNode(parsetree);//parameter list
            parsetree->ast->children = parsetree->children->ast;//raised DATATYPE 
            parsetree->ast->children->next = createAstLeafNode(parsetree->children->next);//TKID
            parsetree->ast->children->last_node = parsetree->ast->children->next;//set last node for child
            //get next node from remaining list
            parsetree->ast->next = parsetree->children->next->next->ast;
            if(parsetree->children->next->ast == NULL){
                parsetree->ast->last_node = parsetree->children->ast;
            }else
            {
                parsetree->ast->last_node = parsetree->children->next->next->ast->last_node;
            }
            break;

        }

        case 14:{
            parsetree->ast = createAstLeafNode(parsetree->children->next);
            break;
        }
        case 15:{
            parsetree->ast = parsetree->children->next->ast;
            break;
        }

        case 17:{
            parsetree->ast = createAstNonleafNode(parsetree->children);//typedefintions
            parsetree->ast->next = createAstNonleafNode(parsetree->children->next);//declarations
            parsetree->ast->next->next = createAstNonleafNode(parsetree->children->next->next);//otherstmts
            parsetree->ast->next->next->next = createAstNonleafNode(parsetree->children->next->next->next); //returnstmt
            parsetree->ast->children = parsetree->children->ast;//typedefintions
            parsetree->ast->next->children = parsetree->children->next->ast;//declarations
            parsetree->ast->next->next->children = parsetree->children->next->next->ast;//otherstmts
            parsetree->ast->next->next->next->children = parsetree->children->next->next->next->ast; //returnstmt
            parsetree->ast->last_node  = parsetree->children->next->next->next->ast;
            break;
        }

        case 18:{
            parsetree->ast = parsetree->children->ast;
            parsetree->children->ast->next = parsetree->children->next->ast;
            //join declaration and declarations 
            //update lastnode to be lastnode from declarations
            if(parsetree->children->next->ast == NULL){
                parsetree->ast->last_node = parsetree->ast;
            }else{
                parsetree->ast->last_node = parsetree->children->next->ast->last_node;
            }
            break;
        }

        case 20:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstLeafNode(parsetree->children->next);
            parsetree->ast->children->next = parsetree->children->next->next->ast;
            parsetree->ast->children->last_node = parsetree->children->next->next->ast->last_node;
            break;
        }

        case 21:{
            parsetree->ast = parsetree->children->ast;
            parsetree->ast->next = parsetree->children->next->ast;
            //store first 2 fields and concat the more fileds
            if(parsetree->children->next->next->ast == NULL){
                parsetree->ast->last_node = parsetree->children->next->ast;
            }else{
                parsetree->ast->next->next = parsetree->children->next->next->ast;
                parsetree->ast->last_node = parsetree->children->next->next->ast->last_node;
            }
            break;
        }

        case 22:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = parsetree->children->next->ast;
            parsetree->ast->children->next = createAstLeafNode(parsetree->children->next->next->next);
            parsetree->ast->children->last_node = parsetree->ast->children->next;
            break;
        }


        case 23:{
            parsetree->ast = parsetree->children->ast;
            
            if(parsetree->children->next->ast==NULL){
                parsetree->ast->last_node = parsetree->children->ast;
            }else{
                parsetree->ast->next = parsetree->children->next->ast;
                parsetree->ast->last_node = parsetree->children->next->ast->last_node;
            }
            break;
        }


        case 25:{
            parsetree->ast = parsetree->children->ast;
            if(parsetree->children->next->ast == NULL){
                parsetree->ast->last_node = parsetree->ast;
            }else{
                parsetree->ast->next = parsetree->children->next->ast;
                parsetree->ast->last_node = parsetree->children->next->ast->last_node;
            }
            break;
        }

        case 27:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = parsetree->children->next->ast;//datatype
            parsetree->ast->children->next = createAstLeafNode(parsetree->children->next->next->next);//tkid
            parsetree->ast->children->next->next = parsetree->children->next->next->next->next->ast;//global or not
            break;
        }

        case 28:{
            parsetree->ast = createAstLeafNode(parsetree->children->next);
            break;
        }

        case 30:{
            parsetree->ast = parsetree->children->ast;
            parsetree->ast->next = parsetree->children->next->ast;
            if(parsetree->children->next->ast == NULL){
                parsetree->ast->last_node = parsetree->ast;
            }else{
                parsetree->ast->last_node = parsetree->children->next->ast->last_node;
            }
            break;
        }

        case 37:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = parsetree->children->ast;
            parsetree->ast->children->next= parsetree->children->next->next->ast;
            break;
        }

        case 38:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstLeafNode(parsetree->children);
            parsetree->ast->children->next = parsetree->children->next->ast;
            break;
        }

        case 39:{
            parsetree->ast = createAstLeafNode(parsetree->children->next);
            break;
        }

        case 41:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstNonleafNode(parsetree->children);//output parameters
            parsetree->ast->children->children = parsetree->children->ast;//assigning output parameters
            parsetree->ast->children->next= createAstLeafNode(parsetree->children->next->next);//funid
            parsetree->ast->children->next->next = createAstNonleafNode(parsetree->children->next->next->next->next->next);
            parsetree->ast->children->next->next->children = parsetree->children->next->next->next->next->next->ast;//input parameters
            break;
        }

        case 42:
        case 44:{
            parsetree->ast = parsetree->children->next->ast;
            break;
        }

        case 45:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstNonleafNode(parsetree->children->next->next);//boolean exp
            parsetree->ast->children->next = createAstNonleafNode(parsetree->children->next->next->next->next);//stmt
            parsetree->ast->children->children = parsetree->children->next->next->ast;//assigning bool
            parsetree->ast->children->next->children = parsetree->children->next->next->next->next->ast;//assigning stmt
            parsetree->ast->children->next->children->next = parsetree->children->next->next->next->next->next->ast;//assigning other stmts
            break;
            
        }

        case 46:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstNonleafNode(parsetree->children->next->next);//boolean exp
            parsetree->ast->children->next = createAstNonleafNode(parsetree->children->next->next->next->next->next);//stmts
            parsetree->ast->children->children = parsetree->children->next->next->ast;
            parsetree->ast->children->next->children = parsetree->children->next->next->next->next->next->ast;//assigning stmts
            parsetree->ast->children->next->children->next = parsetree->children->next->next->next->next->next->next->ast;//concat other stmts
            parsetree->ast->children->next->next = createAstNonleafNode(parsetree->children->next->next->next->next->next->next->next);//elsepart
            parsetree->ast->children->next->next->children = parsetree->children->next->next->next->next->next->next->next->ast;//elsepart assign
            break;

        }

        case 47:{
            parsetree->ast = parsetree->children->next->ast;//assign stmt
            parsetree->ast->next = parsetree->children->next->next->ast;//assign otherstmt
            break;
            
        }

        case 49:
        case 50:{
            parsetree->ast = createAstLeafNode(parsetree);
            parsetree->ast->children = createAstLeafNode(parsetree->children);//read/write
            parsetree->ast->children->next = parsetree->children->next->next->ast;//allvar/singleor recid
            
            break;
        }

        case 51:{
            parsetree->ast  = createAstLeafNode(parsetree);
            parsetree->ast->children = createAstLeafNode(parsetree->children);
            parsetree->ast->children->next = parsetree->children->next->ast;
            break;
        }

        case  54:{
            parsetree->ast = createAstLeafNode(parsetree->children->next);
            break;
        }

      
        case 88:
        case 87:
        case 91:{
            parsetree->ast = parsetree->children->next->ast;
            break;
        }

        case 90:{
            parsetree->ast = createAstLeafNode(parsetree->children);
            parsetree->ast->next = parsetree->children->next->ast;
            break;
        }


        ///rules for arithmetic exp
        case 56:{
            astnode a = converthelper(parsetree->children);
            parsetree->ast =  convert(a,parsetree->children->next);
            break;
        }


        //rules for boolean

        case 73:{
            parsetree->ast = createAstLeafNode(parsetree->children->next->next->next->children);//log op
            parsetree->ast->children = parsetree->children->next->ast;//1st bool
            parsetree->ast->children->next = parsetree->children->next->next->next->next->next->ast;//2nd bool
            break;
        }

        case 74:{
            parsetree->ast = createAstLeafNode(parsetree->children->next->children);//relop
            parsetree->ast->children = createAstLeafNode(parsetree->children->children);//1st var
            parsetree->ast->children->next = createAstLeafNode(parsetree->children->next->next->children);//2nd var
            break;
        }


        case 75:{
            parsetree->ast = createAstNonleafNode(parsetree);
            parsetree->ast->children = createAstLeafNode(parsetree->children);
            parsetree->ast->children->next = parsetree->children->next->next->ast;
            break;
            
        }



    }

}

void generateTree(struct narytree*parseTree);
void generateTree(struct narytree*parseTree){
    if(parseTree==NULL){
        return;
    }

    struct narytree * temp = parseTree->children;
    while(temp != NULL){
        generateTree(temp);
        temp = temp->next;
    }

    //after postorder
    processAstNode(parseTree);
}

struct astNode initAST(struct narytree * parseTree){
    parseTree->ast = &astroot;
    generateTree(parseTree);
    return *(parseTree->ast);
}