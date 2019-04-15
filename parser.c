/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include "parserDef.h"
#include "lexer.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct FirstFollow FirstandFollow[NONTERMINALCOUNT];
bool first[NONTERMINALCOUNT];
bool follow[NONTERMINALCOUNT];
struct LinkedListGrammar g[RULESIZE];
// void createParseTable(FirstandFollow F, Table T){
// struct node
// int parseTable[NONTERMINALCOUNT][TOKENCOUNT] = {0};
symbolnode *symbolTable[TABLESIZE];

STACK stack;
struct narytree *parseTree;
set traversed = false;
bool success;
int parsenode_count = 0;
int parseTable[NONTERMINALCOUNT][TOKENCOUNT] = {0};
// }
char *tokens1[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RECORDID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "LEX_ERROR",
    "EPS",
    "$"};

char *symbols[] = {
    "program",
    "mainFunction",
    "otherFunctions",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "primitiveDatatype",
    "constructedDatatype",
    "remaining_list",
    "stmts",
    "typeDefinitions",
    "typeDefinition",
    "fieldDefinitions",
    "fieldDefinition",
    "moreFields",
    "declaration",
    "declarations",
    "global_or_not",
    "otherStmts",
    "stmt",
    "assignmentStmt",
    "singleOrRecId",
    "new24",
    "funCallStmt",
    "outputParameters",
    "inputParameters",
    "iterativeStmt",
    "conditionalStmt",
    "elsePart",
    "ioStmt",
    "allVar",
    "newVar",
    "arithmeticExpression",
    "expPrime",
    "term",
    "termPrime",
    "factor",
    "highPrecedenceOperators",
    "lowPrecedenceOperators",
    "all",
    "temp",
    "booleanExpression",
    "var",
    "logicalOp",
    "relationalOp",
    "returnStmt",
    "optionalReturn",
    "idList",
    "more_ids"};

//set funcs

set getSet(enum tok t)
{
    set a = 0;
    return (1UL << t) | a;
}

set unionSet(set a, set b)
{
    return a | b;
}

bool checkSet(set s, enum tok t)
{
    set a = getSet(t);
    return (s & a) == a;
}

bool parseSuccess(){
 return success;
}
//STACK funcs
int getParsenodes(){
    return parsenode_count;
}

struct narytree *createChild(llNode n,struct narytree* parnode)
{
    //create stack node from grammar node
    struct narytree *a = NULL;
    while (n != NULL)
    {
        struct narytree *temp;
        if (a == NULL)
        {
            a = (struct narytree *)malloc(sizeof(struct narytree));
            parsenode_count++;
            temp = a;
            a->parent = n->node;
            n = n->next;
            a->next = NULL;
            a->children = NULL;
            a->lexeme = NULL;
            a->line = -1;
            a->parnode = parnode;
            continue;
        }
        temp->next = (struct narytree *)malloc(sizeof(struct narytree));
            parsenode_count++;
        temp = temp->next;
        temp->next = NULL;
        temp->children = NULL;
        temp->lexeme = NULL;
        temp->line = -1;
        temp->parent = n->node;
        temp->parnode = parnode;
        n = n->next;
    }
    return a;
}

STACK push(STACK a, stackNode b)
{
    //no longer used
    b->next = a.head;
    a.head = b;
    return a;
}

STACK pushChildren(STACK a, struct narytree *b)
{
    //push the entire production of grammar on stack in correct order
    stackNode s = NULL, temp;
    struct narytree *temptree = b;
    while (temptree != NULL)
    {
        if (s == NULL)
        {
            s = (stackNode)malloc(sizeof(struct StackNode));
            s->tree = temptree;

            temptree = temptree->next;
            temp = s;
            continue;
        }
        temp->next = (stackNode)malloc(sizeof(struct StackNode));

        temp = temp->next;
        temp->tree = temptree;

        temptree = temptree->next;
        // temp = s;
    }
    temp->next = a.head;
    a.head = s;
    return a;
}

stackNode pop()
{
    stackNode s = stack.head;
    if (stack.head != NULL)
    {
        stack.head = stack.head->next;
    }
    return s;
}

stackNode probe()
{
    //return without popping
    return stack.head;
}

bool isEmpty(STACK a)
{
    return a.head == NULL;
}

set findFirst(enum nonterminal a)
{
    if (first[a])
    {
        return FirstandFollow[a].first;
    }
    llNode temp;
    int i = 0;
    bool b;
    set c = 0, t = 0;
    set firstset = 0;
    for (int i = 0; i < RULESIZE; i++)
    { //traversing grammer for productions
        if (g[i].nt == a)
        {
            temp = g[i].prod;
            t = getSet(EPS);
            while (temp != NULL)
            {                           //traversing a production
                t = t & ~(getSet(EPS)); //removing eps

                if (temp->node.is_leaf)
                {
                    c = getSet(temp->node.data.term);
                    t = unionSet(t, c);
                    break;
                }
                else
                {
                    c = findFirst(temp->node.data.nonterm);
                }

                b = true;
                t = unionSet(t, c);

                if (!checkSet(c, EPS))
                {
                    //if no eps found wrap it up
                    break;
                }

                temp = temp->next;
            }
            firstset = unionSet(firstset, t); //add first of current production to firsts
            g[i].first = t;                   //store firsts of respective rule
        }
    }
    first[a] = true;
    FirstandFollow[a].first = firstset;
    return firstset;
}

set findFollow(enum nonterminal a, enum nonterminal parent)
{
    if (follow[a])
    {
        return FirstandFollow[a].follow;
    }
    llNode temp;
    bool b = true;
    set c = 0;
    set followset = 0;
    if (a == 0)
    {
        followset = getSet($);
        FirstandFollow[a].follow = followset;
        return followset;
    }
    set t = 0;
    for (int i = 0; i < RULESIZE; i++)
    { //traversing grammar for productions
        temp = g[i].prod;
        t = 0UL;
        // bool b = false;
        bool carry = false;

        while (temp != NULL)
        { //traversing single production

            if ((!temp->node.is_leaf && temp->node.data.nonterm == a) || carry)
            {
                carry = false;
                if (temp->next == NULL)
                {
                    //Old implementation (Hacky Stuff)
                    // if (g[i].nt == parent)
                    // {
                    //     b = false;
                    // }
                    // if (follow[g[i].nt] || (g[i].nt != parent && g[i].nt != temp->node.data.nonterm))
                    // {
                    //     c = findFollow(g[i].nt, parent);
                    //     t = unionSet(t, c);
                    // }

                    //New implementation
                    if(checkSet(traversed,(enum tok)g[i].nt)){
                        //if a already traversed
                        if(g[i].nt != a){//stop storage for all  recursion except self
                            b = false;
                            }
                    }else{
                        traversed = unionSet(traversed,getSet(g[i].nt));
                        c = findFollow(g[i].nt,parent);
                        t = unionSet(t,c);
                    }
                }
                else
                {
                    if (temp->next->node.is_leaf)
                    {
                        
                        c = getSet(temp->next->node.data.term);
                        t = unionSet(t, c);
                    }
                    else
                    {
                        c = findFirst(temp->next->node.data.nonterm);
                        if (checkSet(c, EPS))
                        {
                            c = c & ~(getSet(EPS)); //removing eps
                            carry = true;
                        }
                        t = unionSet(t, c);
                    }
                }
            }
            temp = temp->next;
        }

        followset = unionSet(followset, t);
    }
    follow[a] = b;
    FirstandFollow[a].follow = followset;
    return followset;
}

void ComputeFirstAndFollowSets()
{
    //set first and follow bool to false
    for (int i = 0; i < NONTERMINALCOUNT; i++)
    {
        first[i] = false;
        follow[i] = false;
    }
    for (int i = 0; i < NONTERMINALCOUNT; i++)
    {
        if (first[i])
            continue;
        findFirst(i);
    }
    for (int i = 0; i < NONTERMINALCOUNT; i++)
    {
        if (follow[i])
            continue;
        traversed = getSet(i);
        findFollow(i, i);
    }

    //set an array of struct FirstFollow of size of non term
}

void printFirstFollow()
{
    //debugging func
    for (int i = 0; i < NONTERMINALCOUNT; i++)
    {
        printf("\n%s :follow ", symbols[i]);
        for (int j = 0; j < TOKENCOUNT; j++)
        {
            if (checkSet(FirstandFollow[i].follow, j))
            {
                printf("%s, ", tokens1[j]);
            }
        }
    }
}

//parse table funcs
int parse(enum tok t, enum nonterminal n)
{
    set s;
    if (checkSet(FirstandFollow[n].first, t))
    {
        for (int i = 0; i < RULESIZE; i++)
        {
            if (g[i].nt == n)
            {
                if (checkSet(g[i].first, t))
                {
                    return i;
                }
            }
        }
    }

    if (checkSet(FirstandFollow[n].follow, t))
    {
        return -2;
    }
    return -1;
}

void initParseTable()
{
    for (int nt = 0; nt < NONTERMINALCOUNT; nt++)
    {
        for (int tk = 0; tk < TOKENCOUNT; tk++)
        {
            parseTable[nt][tk] = parse(tk, nt);
        }
    }
}

//grammar funcs

struct node getnode(char *str)
{
    int a = strlen(str);
    if (str[a - 1] == '\n')
    {

        if (str[a - 2] == '\r')
        {
            str[a - 2] = '\0';
        }
        str[a - 1] = '\0';
    }
    struct node tempnode;
    unsigned long hash = gethash(str);
    hash = hash % TABLESIZE;

    if (isupper(str[0]) == 0)
    {
        tempnode.is_leaf = false;
        if (symbolTable[hash] != NULL)
        {
            symbolnode *n = symbolTable[hash];
            while (strcmp(str, n->key) != 0)
            { //next if not equal
                n = n->next;
            }
            tempnode.data.nonterm = n->value.nonterm;
        }
    }
    else
    {
        tempnode.is_leaf = true;
        if (symbolTable[hash] != NULL)
        {
            symbolnode *n = symbolTable[hash];
            while (strcmp(str, n->key) != 0)
            { //skip if not equal
                n = n->next;
            }
            tempnode.data.term = n->value.term;
        }
    }
    return tempnode;
}

void readGrammar(char *file)
{
    FILE *in = fopen(file, "rb");
    char buffer[200];
    char *token;
    struct node temp;
    llNode tempnode;
    for (int i = 0; i < RULESIZE; i++)
    {
        g[i].prod = (llNode)malloc(sizeof(struct LinkedListNode) * 1);
        tempnode = g[i].prod;
        fgets(buffer, 200, in);
        token = strtok(buffer, " "); //rhs
        temp = getnode(token);
        g[i].nt = temp.data.nonterm;
        token = strtok(NULL, " "); // '===>'
        token = strtok(NULL, " "); // first prod
        temp = getnode(token);
        tempnode->node = temp;
        tempnode->next = NULL;

        token = strtok(NULL, " ");
        while (token != NULL)
        {
            temp = getnode(token);
            tempnode->next = (llNode)malloc(sizeof(struct LinkedListNode));
            tempnode = tempnode->next;
            tempnode->next = NULL;
            tempnode->node = temp;
            token = strtok(NULL, " ");
        }
    }
    fclose(in);
}

void freegnode(llNode a)
{
    if (a->next != NULL)
    {
        freegnode(a->next);
    }
    free(a);
}

void freeGrammar()
{
    for (int i = 0; i < RULESIZE; i++)
    {
        freegnode(g[i].prod);
        g[i].prod = NULL;
    }
}

void printGrammar()
{
    for (int i = 0; i < RULESIZE; i++)
    {
        printf("%s --> ", symbols[g[i].nt]);
        llNode temp = g[i].prod;

        while (temp != NULL)
        {
            if (temp->node.is_leaf)
            {
                printf("%s ,", tokens1[temp->node.data.term]);
            }
            else
            {
                printf("%s ", symbols[temp->node.data.nonterm]);
            }
            temp = temp->next;
        }
        printf("\n");
    }
}

void seedSymbolTable()
{
    int i;
    unsigned long j;

    for (i = 0; i < NONTERMINALCOUNT; i++)
    {
        j = gethash(symbols[i]);
        j = j % TABLESIZE;
        symbolnode *b = symbolTable[j];
        if (b == NULL)
        {
            symbolTable[j] = (symbolnode *)malloc(sizeof(symbolnode) * 1);
            symbolTable[j]->key = symbols[i];
            symbolTable[j]->value.nonterm = i;
            symbolTable[j]->next = NULL;
            // symbolTable[j]->n.is_leaf = true;
            continue;
        }
        symbolnode *a = (symbolnode *)malloc(sizeof(symbolnode) * 1);
        a->key = symbols[i];
        a->value.nonterm = i;
        a->next = NULL;
        // a->n.is_leaf =true;
        while (b->next != NULL)
        {
            b = b->next;
        }
        b->next = a;
    }

    for (i = 0; i < TOKENCOUNT; i++)
    {
        j = gethash(tokens1[i]);
        j = j % TABLESIZE;
        symbolnode *b = symbolTable[j];
        if (b == NULL)
        {
            symbolTable[j] = (symbolnode *)malloc(sizeof(symbolnode) * 1);
            symbolTable[j]->key = tokens1[i];
            symbolTable[j]->value.term = i;
            symbolTable[j]->next = NULL;
            // symbolTable[j]->n.is_leaf = true;
            continue;
        }
        symbolnode *a = (symbolnode *)malloc(sizeof(symbolnode) * 1);
        a->key = tokens1[i];
        a->value.term = i;
        a->next = NULL;
        // a->n.is_leaf =true;
        while (b->next != NULL)
        {
            b = b->next;
        }
        b->next = a;
    }
}

void freesymnode(symbolnode *a)
{
    if (a->next != NULL)
    {
        freesymnode(a->next);
    }
    free(a);
}

void freesymbolTable()
{
    for (int i = 0; i < TABLESIZE; i++)
    {
        if (symbolTable[i] == NULL)
        {
            continue;
        }
        freesymnode(symbolTable[i]);
        symbolTable[i] = NULL;
    }
}

//parse tree funcs

struct narytree * initTree()
{
    //push
    success = true;
    parsenode_count = 0;
    stackNode n = (stackNode)malloc(sizeof(struct StackNode));
    n->tree = (struct narytree *)malloc(sizeof(struct narytree));
            parsenode_count++;
    n->tree->parent.is_leaf = true;
    n->tree->parent.data.term = $;
    n->next = NULL;
    

    stack = push(stack, n);
    n = (stackNode)malloc(sizeof(struct StackNode));
    n->tree = (struct narytree *)malloc(sizeof(struct narytree));
            parsenode_count++;
    parseTree = n->tree;
    n->tree->parent.is_leaf = false;
    n->tree->parent.data.nonterm = program;
    n->next = NULL;
    n->tree->lexeme = NULL;
    n->tree->next = NULL;
    n->tree->children = NULL;
    // n->tree->rule = 1;
    stack = push(stack, n);

    token t = getNextToken();
    n = pop();
    int rule = -1;
    while (1)
    {
        if (n->tree->parent.data.term == $)
        {
            if (file_empty())
            {
                printf("parsing done\n");
            }
            else
            {
                printf("parsing failed\n");
            }
            break;
        }

        if (t.type == COMMENT)
        {
            if (file_empty())
            {
                break;
            }
            free(t.lexeme);
            t = getNextToken();
            continue;
        }

        if (t.type == LEX_ERROR)
        {
            if (strlen(t.lexeme) > 1)
            {
                success = false;
                printf("Line %d: Unknown pattern %s\n", t.line_no, t.lexeme);
            }
            else if (strlen(t.lexeme) == 1)
            {
                success = false;
                printf("Line %d: Unknown symbol %s\n", t.line_no, t.lexeme);
            }
            if (file_empty())
            {
                break;
            }
            free(t.lexeme);
            t = getNextToken();
            continue;
        }

        if (t.type == ID && strlen(t.lexeme) > 20)
        {
            success = false;
            printf("Line %d:Identifier is longer than the prescribed length of 20 characters\n", t.line_no);
            if (file_empty())
            {
                break;
            }
            free(t.lexeme);
            t = getNextToken();
            continue;
        }

        if (t.type == FUNID && strlen(t.lexeme) > 30)
        {
            success = false;
            printf("Line %d:Function Identifier is longer than the prescribed length of 30 characters\n", t.line_no);
            if (file_empty())
            {
                break;
            }
            free(t.lexeme);
            t = getNextToken();
            continue;
        }

        if (n->tree->parent.is_leaf)
        { //terminal matches
            if (n->tree->parent.data.term == t.type)
            {
                if (file_empty())
                {
                    break;
                }
                n->tree->line = t.line_no;
                n->tree->lexeme = t.lexeme;
                n->tree->children = NULL;
                n->tree->ast = NULL;
                n->tree->rule = 0;
                t = getNextToken();
                free(n);
                n = pop();
            }
            else
            {
                success = false;
                //mismatch error
                //e.g Line 20: The token TK_END for lexeme end does not
                // match with the expected token TK_RETURN
                printf("Line No:%d, The token %s for lexeme %s does not match with expected token %s\n", t.line_no, tokens1[t.type], t.lexeme, tokens1[n->tree->parent.data.term]);
                stackNode a = probe();
                while (1)
                {
                    if (n->tree->parent.data.term == t.type)
                    {
                        break;
                    }

                    if (a->tree->parent.is_leaf)
                    {
                        if (a->tree->parent.data.term == t.type)
                        {
                            free(n);
                            n = pop();
                            break;
                        }
                    }
                    else
                    {
                        rule = parseTable[a->tree->parent.data.nonterm][t.type];
                        if (rule >= 0)
                        {
                            free(n);
                            n = pop();
                            break;
                        }
                    }

                    if (t.type == SEM)
                    {
                        free(n);
                        n = pop();
                        break;
                    }

                    if (file_empty())
                    {
                        break;
                    }
                    free(t.lexeme);
                    t = getNextToken();

                    if (t.type == COMMENT)
                    {
                        if (file_empty())
                        {
                            break;
                        }
                        free(t.lexeme);
                        t = getNextToken();
                        continue;
                    }

                    if (t.type == LEX_ERROR)
                    {
                        if (strlen(t.lexeme) > 1)
                        {
                            success = false;
                            printf("Line %d: Unknown pattern %s\n", t.line_no, t.lexeme);
                        }
                        else if (strlen(t.lexeme) == 1)
                        {
                            success = false;
                            printf("Line %d: Unknown symbol %s\n", t.line_no, t.lexeme);
                        }
                        if (file_empty())
                        {
                            break;
                        }
                        free(t.lexeme);
                        t = getNextToken();
                        continue;
                    }

                    if (t.type == ID && strlen(t.lexeme) > 20)
                    {
                        success = false;
                        printf("Line %d:Identifier is longer than the prescribed length of 20 characters\n", t.line_no);
                        if (file_empty())
                        {
                            break;
                        }
                        free(t.lexeme);
                        t = getNextToken();
                        continue;
                    }

                    if (t.type == FUNID && strlen(t.lexeme) > 30)
                    {
                        success = false;
                        printf("Line %d:Function Identifier is longer than the prescribed length of 30 characters\n", t.line_no);
                        if (file_empty())
                        {
                            break;
                        }
                        free(t.lexeme);
                        t = getNextToken();
                        continue;
                    }
                }
            }
            continue;
        }

        if (!n->tree->parent.is_leaf)
        {
            rule = parseTable[n->tree->parent.data.nonterm][t.type];
            if (rule >= 0)
            {
                llNode temp = g[rule].prod;
                n->tree->children = createChild(temp,n->tree);
                stack = pushChildren(stack, n->tree->children);
                n->tree->line = t.line_no;
                n->tree->ast = NULL;
                n->tree->rule = rule+1;
                free(n);
                n = pop();
            }
            else
            {
                if (checkSet(FirstandFollow[n->tree->parent.data.nonterm].first, EPS))
                {
                    rule = parseTable[n->tree->parent.data.nonterm][EPS];
                    if (rule >= 0)
                    {
                        llNode temp = g[rule].prod;
                        n->tree->children = createChild(temp,n->tree);
                        n->tree->line = t.line_no;
                        n->tree->ast = NULL;
                        n->tree->rule = rule+1;

                        //since eps will not be put in stack we need to assign values now
                        n->tree->children->line = t.line_no;
                        n->tree->children->rule = 0;

                        free(n);
                        n = pop();
                    }
                }
                else
                {
                    success = false;
                    printf("Line No:%d, The token %s for lexeme %s does not match with expected token of type %s\n", t.line_no, tokens1[t.type], t.lexeme, symbols[n->tree->parent.data.nonterm]);
                    stackNode a = probe();
                    while (1)
                    {
                        rule = parseTable[n->tree->parent.data.nonterm][t.type];
                        if (rule >= 0)
                        {
                            break;
                        }
                        if (a->tree->parent.is_leaf)
                        {
                            if (a->tree->parent.data.term == t.type)
                            {
                                free(n);
                                n = pop();
                                break;
                            }
                        }
                        else
                        {
                            rule = parseTable[a->tree->parent.data.nonterm][t.type];
                            if (rule >= 0)
                            {
                                free(n);
                                n = pop();
                                break;
                            }
                        }

                        if (t.type == SEM)
                        {
                            free(n);
                            n = pop();
                            break;
                        }

                        if (file_empty())
                        {
                            break;
                        }
                        free(t.lexeme);
                        t = getNextToken();

                        if (t.type == COMMENT)
                        {
                            if (file_empty())
                            {
                                break;
                            }
                            free(t.lexeme);
                            t = getNextToken();
                            continue;
                        }

                        if (t.type == LEX_ERROR)
                        {
                            if (strlen(t.lexeme) > 1)
                            {
                                success = false;
                                printf("Line %d: Unknown pattern %s\n", t.line_no, t.lexeme);
                            }
                            else if (strlen(t.lexeme) == 1)
                            {
                                success = false;
                                printf("Line %d: Unknown symbol %s\n", t.line_no, t.lexeme);
                            }
                            if (file_empty())
                            {
                                break;
                            }
                            free(t.lexeme);
                            t = getNextToken();
                            continue;
                        }

                        if (t.type == ID && strlen(t.lexeme) > 20)
                        {
                            success = false;
                            printf("Line %d:Identifier is longer than the prescribed length of 20 characters\n", t.line_no);
                            if (file_empty())
                            {
                                break;
                            }
                            free(t.lexeme);
                            t = getNextToken();
                            continue;
                        }

                        if (t.type == FUNID && strlen(t.lexeme) > 30)
                        {
                            success = false;
                            printf("Line %d:Function Identifier is longer than the prescribed length of 30 characters\n", t.line_no);
                            if (file_empty())
                            {
                                break;
                            }
                            free(t.lexeme);
                            t = getNextToken();
                            continue;
                        }
                    }
                }
            }
        }
    }
    if (t.lexeme != NULL)
    {
        //free last lexeme not used
        free(t.lexeme);
    }
    while (n != NULL)
    {   
        //drain stack
        if (n->tree->parent.is_leaf && n->tree->parent.data.term == $)
        {
            free(n->tree);
        }
        free(n);
        n = pop();
    }
    return parseTree;
}

void tempprinttree(struct narytree *n, enum nonterminal parent);

void tempprintnode(struct narytree *n, enum nonterminal parent);

void tempprintcurr(struct narytree *n, enum nonterminal parent)
{

    if (n->line == -1)
    {
        return;
    }
    char a[] = "---";
    char yes[] = "yes";
    char no[] = "no";
    char root[] = "root";
    char *lexeme;
    int lineno = n->line;
    char *tokenname;
    char *value;
    char *parentsym;
    char *yesorno;
    char *nodesym;
    if (parent != -1)
    {
        parentsym = symbols[parent];
    }
    else
    {
        parentsym = root;
    }

    if (n->parent.is_leaf)
    {
        if (n->parent.data.term == NUM || n->parent.data.term == RNUM)
        {
            value = n->lexeme;
        }
        else
        {
            value = a;
        }
        lexeme = n->lexeme;
        tokenname = tokens1[n->parent.data.term];
        yesorno = yes;
        nodesym = a;
        if (n->parent.data.term == EPS)
        {
            lexeme = tokenname;
        }
        printf( "%-30s %d\t%-8s %-8s %-15s %-5s %-15s\n", lexeme, lineno, tokenname, value, parentsym, yesorno, nodesym);
    }
    else
    {
        value = a;
        lexeme = a;
        nodesym = symbols[n->parent.data.nonterm];
        yesorno = no;
        tokenname = a;
        printf("%-30s %s\t%-8s %-8s %-15s %-5s %-15s\n", lexeme, a, tokenname, value, parentsym, yesorno, nodesym);
    }
    if (n->parent.is_leaf)
    {
        if (n->parent.data.term != EPS)
        {
            free(n->lexeme);
        }
    }
}

void tempprintnode(struct narytree *n, enum nonterminal parent)
{

    struct narytree *temp = NULL;
    if (n->children != NULL)
    {
        temp = n->children->next;
        tempprintnode(n->children, n->parent.data.nonterm);
    }
    //print current
    tempprintcurr(n, parent);

    // print remaining children
    if (temp != NULL)
    {
        tempprinttree(temp, n->parent.data.nonterm);
    }

    free(n);
}

void tempprinttree(struct narytree *n, enum nonterminal parent)
{

    struct narytree *temp = NULL;
    if (n->children != NULL)
    {
        temp = n->children->next;
        tempprintnode(n->children, n->parent.data.nonterm);
    }
    //print current
    tempprintcurr(n, parent);

    // print remaining children
    if (temp != NULL)
    {
        tempprinttree(temp, n->parent.data.nonterm);
    }
    //print next nodes
    if (n->next != NULL)
    {
        tempprinttree(n->next, parent);
    }

    free(n);
}

void printTree()
{   //set a global FILE * in this file and use fprintf instead on printf to save output to file
    tempprinttree(parseTree, -1);
}



void freeparsenode(struct narytree *n){
    if (n->parent.is_leaf)
    {
        if (n->parent.data.term != EPS)
        {
            free(n->lexeme);
        }
    }else if(n->parent.data.nonterm == program){
        return;
    }
    free(n);
}
void freeparsetree(struct narytree *n);
void freeparsetree(struct narytree *n){
    if(n == NULL){
        return;
    }
        struct narytree * temp = n->children;
        while(temp!=NULL){
            freeparsetree(temp);
            temp = temp->next;
        }
    freeparsenode(n);
}

