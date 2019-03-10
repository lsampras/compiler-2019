/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#include "lexerDef.h"
#define BUFFERSIZE 512
#define KEYWORD_NUM 24
char BUFFER1[BUFFERSIZE];
char BUFFER2[BUFFERSIZE];
char *BUFFER;
bool buff_count,buff_switch,file_end,buff_end;
int read_count;
int line_count;
int read_ptr=0;
FILE* file;

char * tokens[] = {
    "TK_ASSIGNOP" ,
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
    "eps"
    };

    char * keywords[] = {
        "with",
        "parameters",
        "end",
        "while",
        "type",
        "_main",
        "global",
        "parameter",
        "list",
        "input",
        "output",
        "int",
        "real",
        "endwhile",
        "if",
        "then",
        "endif",
        "read",
        "write",
        "return",
        "call",
        "record",
        "endrecord",
        "else"
    };

    enum tok key_tokens[] = {
        WITH,
        PARAMETERS,
        END,
        WHILE,
        TYPE,
        MAIN,
        GLOBAL,
        PARAMETER,
        LIST,
        INPUT,
        OUTPUT,
        INT,
        REAL,
        ENDWHILE,
        IF,
        THEN,
        ENDIF,
        READ,
        WRITE,
        RETURN,
        CALL,
        RECORD,
        ENDRECORD,
        ELSE
    };

void removeComments(FILE * in, char * cleanFile){
    int ptr=0;
    int size = 1024;
    char*buf=(char*)malloc(sizeof(char)*512);
    char * remaining = buf;
    char *result = NULL;
    while(!feof(in)){
        fread(buf,1,512,in);
        remaining = buf;
        while((result = strchr(remaining,'%')) != NULL){
            fwrite(remaining, 1, result - remaining, stdout);
            while((remaining = strchr(result,'\n')) == NULL){
                fread(buf,1,512,in);
                result = buf;
            }
        }
        fwrite(remaining, 1, buf + 512 - remaining, stdout);
    }
    free(buf);
}


unsigned long  gethash(char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


void seedHashTable(){
    int i;
    unsigned long j;
    
    for(i=0;i<KEYWORD_NUM;i++){
        j = gethash(keywords[i]);
        j = j%TABLESIZE;
        hashnode * b =TABLE[j];
        if(b==NULL){
            TABLE[j] = (hashnode*)malloc(sizeof(hashnode)*1);
            TABLE[j]->key = keywords[i];
            TABLE[j]->value = key_tokens[i];
            TABLE[j]->next = NULL;
            continue;
        }
        hashnode * a = (hashnode*)malloc(sizeof(hashnode)*1);
        a->key = keywords[i];
        a->value = key_tokens[i];
        a->next = NULL;
        while(b->next!=NULL){
            b = b->next;
        }
        b->next = a;
    }
}
void freenode(hashnode *a){
    if(a->next!=NULL){
        freenode(a->next);
    }
    free(a);
}

void freeHashTable(){
    
    for(int i = 0;i<TABLESIZE;i++){
        if(TABLE[i]==NULL){
            continue;
        }
        freenode(TABLE[i]);
        TABLE[i]=NULL;
    }
}


void getStream(FILE * fp){
    file  = fp;
    file_end = false;
    buff_count = false;
    buff_switch = false;
    buff_end = false;
    line_count = 1;
    read_ptr=0;
    read_count = fread(BUFFER1,1,BUFFERSIZE,file);
    if(read_count < BUFFERSIZE){
        BUFFER1[read_count] = '\0';
        return;
    }
    read_count = fread(BUFFER2,1,BUFFERSIZE,file);
    if(read_count < BUFFERSIZE){
        BUFFER2[read_count] = '\0';
    }
    
    return ;
}

token createToken(enum tok type,int start,int curr){
    token a;
    a.line_no = line_count;
    a.type = type;
    if(buff_end){
        //if using both buffers
        a.lexeme = (char*)malloc(sizeof(char)*(BUFFERSIZE - start + curr + 2));
        if(buff_count){
            memcpy(a.lexeme,BUFFER1+start,BUFFERSIZE-start);
            memcpy(a.lexeme+BUFFERSIZE-start,BUFFER2,curr+1);
        }else{
            memcpy(a.lexeme,BUFFER2+start,BUFFERSIZE-start);
            memcpy(a.lexeme+BUFFERSIZE-start,BUFFER1,curr+1);
        }
        a.lexeme[BUFFERSIZE - start + curr + 1] = '\0';
        read_ptr++;
        return a;
    }
    a.lexeme = (char*)malloc(sizeof(char)*(curr-start+2));
    memcpy(a.lexeme,BUFFER+start,curr-start+1);
    a.lexeme[curr-start+1] = '\0';
    read_ptr++;
    return a;
}


token lookup(enum tok type,int start,int curr){
    token a = createToken(type,start,curr);

    unsigned long hash = gethash(a.lexeme);
    hash = hash%TABLESIZE;
    if(TABLE[hash] != NULL){
        hashnode * n = TABLE[hash];
        while(strcmp(a.lexeme,n->key) != 0){ //skip if not equal
            if(n->next==NULL) return a; //return default if not found
            n = n->next;
        }
        a.type = n->value;
    }
    return a;
}

void updateBuffer(){
    if(!feof(file)){
        //if not end of file read in buffer not being used
        if(buff_count){
            read_count = fread(BUFFER1,1,BUFFERSIZE,file);
            if(read_count < BUFFERSIZE){
                BUFFER1[read_count] = '\0';
            }
        }else{
            read_count = fread(BUFFER2,1,BUFFERSIZE,file);
            if(read_count < BUFFERSIZE){
                BUFFER2[read_count] = '\0';
            }
        }
    }else{

    }
}

void printToken(token t){
    if(strlen(t.lexeme)==0){
        free(t.lexeme);
        return;
    }
    printf("token: %s \t lex: %s\t  line: %d\n", tokens[t.type],t.lexeme,t.line_no);
    free(t.lexeme);
    return;
}

bool file_empty(){
    return file_end;
}

token getNextToken(){
    buff_end = false;
    BUFFER = buff_count ? BUFFER2 : BUFFER1;
    int state=0;

    if(read_ptr < 0){
        //go back to previous buffer
        read_ptr=BUFFERSIZE+read_ptr;
        buff_switch = false;
    }
    if(buff_switch){
        updateBuffer();
        buff_switch=false;
    }
    int start = read_ptr;
    char temp;
    while(1){
        temp = BUFFER[read_ptr];
        if (read_ptr == BUFFERSIZE){
            //change buffer if read till end
            read_ptr = 0;
            buff_count=!buff_count;
            BUFFER = buff_count ? BUFFER2 : BUFFER1;
                buff_switch = true;
                buff_end = true;
            if(start == BUFFERSIZE){
                start = 0;
                buff_end = false;
            }
        }

        if(file_end){
            return createToken(LEX_ERROR,start,start);
        }

        if(BUFFER[read_ptr]=='\0'){
            file_end = true;
        }
        

        switch(state){
            case 0://start state
                switch(BUFFER[read_ptr]){
                    case '.':
                        return createToken(DOT,start,read_ptr);

                    case ',':
                        return createToken(COMMA,start,read_ptr);
                    
                    case ';' : 
                        return createToken(SEM,start,read_ptr);
                    
                    case ':':
                        return createToken(COLON,start,read_ptr);

                    case '[':
                        return createToken(SQL,start,read_ptr);

                    case ']':
                        return createToken(SQR,start,read_ptr);

                    case'(':
                        return createToken(OP,start,read_ptr);

                    case')':
                        return createToken(CL,start,read_ptr);

                    case'+':
                        return createToken(PLUS,start,read_ptr);

                    case'-':
                        return createToken(MINUS,start,read_ptr);

                    case'/':
                        return createToken(DIV,start,read_ptr);
                

                    case'*':                     
                        return createToken(MUL,start,read_ptr);

                    case'%':                     
                        state = 15;                     
                        break;

                    case'>':                     
                        state = 17;                     
                        break;

                    case'<':                     
                        state = 20;                     
                        break;

                    case'=':                     
                        state = 28;                     
                        break;

                    case'!':                     
                        state = 31;                     
                        break;

                    case'#':                     
                        state = 33;                     
                        break;

                    case'_':                     
                        state = 37;                     
                        break;

                    case '0'...'9':                     
                        state = 42;                     
                        break;
                    
                    case 'a':                     
                        state = 49;                     
                        break;

                    case 'e'...'z':                     
                        state = 49;                     
                        break;
                    
                    case 'b' ... 'd':
                        state = 51;
                        break;
                    
                    case'&':                     
                        state = 55;                     
                        break;

                    case'@':                     
                        state = 59;                     
                        break;

                    case'~':    
                        return createToken(NOT,start,read_ptr);

                    case'\n':
                        state = 0;
                        line_count++;
                        start = read_ptr+1;
                        break;
                    
                    case'\t':
                        state = 0;
                        start = read_ptr+1;
                        break;
                        
                    case' ':
                        state = 0;
                        start = read_ptr+1;
                        break;

                    default:
                    return createToken(LEX_ERROR,start,read_ptr);
                }
                break;

            case 13:
                start = read_ptr;
                state = 0;
                break;

            case 15:
                if(BUFFER[read_ptr]=='\n'){
                    state = 0;
                    read_ptr--;
                    return createToken(COMMENT,start,read_ptr+1);//change
                }else{
                    break;
                }

            case 17:
                if(BUFFER[read_ptr] == '='){
                    return createToken(GE,start,read_ptr);
                }else{
                    read_ptr--;
                    return createToken(GT,start,read_ptr);
                }

            case 20:
                if(BUFFER[read_ptr] == '='){
                    return createToken(LE,start,read_ptr);
                }else if(BUFFER[read_ptr] == '-'){
                    state = 23;
                    break;
                }else{
                    read_ptr--;
                    return createToken(LT,start,read_ptr);
                }
            case 23:
                if(BUFFER[read_ptr] == '-'){
                    state = 24;
                    break;
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }


            case 24:
                if(BUFFER[read_ptr] == '-'){
                    return createToken(ASSIGNOP,start,read_ptr);
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 28:
                if(BUFFER[read_ptr]=='='){
                    return createToken(EQ,start,read_ptr);
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 31:
                if(BUFFER[read_ptr]=='='){
                    return createToken(NE,start,read_ptr);
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 33:
                if(BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122){
                    //is lower alphabet
                    state = 34;
                    break;
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 34:
                if(BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122){
                    //is lower alphabet
                    state = 34;
                    break;
                }else{
                    read_ptr--;
                    return createToken(RECORDID,start,read_ptr);
                }

            case 37:
                if((BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122) || (BUFFER[read_ptr]>=65 && BUFFER[read_ptr]<=90)){
                    //is alphabet
                    state = 38;
                    break;
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 38:
                if((BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122) || (BUFFER[read_ptr]>=65 && BUFFER[read_ptr]<=90)){
                    //is alphabet
                    state = 38;
                    break;
                }else if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 39;
                    break;
                }else{
                    read_ptr--;
                    return lookup(FUNID,start,read_ptr);
                }

            case 39:
                if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 39;
                    break;
                }else{
                    read_ptr--;
                    return lookup(FUNID,start,read_ptr);
                }

            case 42:
                if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 42;
                    break;
                }else if(BUFFER[read_ptr]=='.'){
                    state = 44;
                    break;
                }else{
                    read_ptr--;
                    return createToken(NUM,start,read_ptr);
                }

            case 44:
                if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 45;
                    break;
                }else {
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }


            case 45:
                if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    return createToken(RNUM,start,read_ptr);
                }else {
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 49:
                if(BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122){
                    //is lower alphabet
                    state = 49;
                    break;
                }else{
                    read_ptr--;
                    return lookup(FIELDID,start,read_ptr);
                }

            case 51:
                if(BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122){
                    //is lower alphabet
                    state = 49;
                    break;
                }else if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 52;
                    break;
                }else {
                    read_ptr--;
                    return lookup(FIELDID,start,read_ptr);
                }

            case 52:
                if(BUFFER[read_ptr]>=97 && BUFFER[read_ptr]<=122){
                    //is lower alphabet
                    state = 52;
                    break;
                }else if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 53;
                    break;
                }else {
                    read_ptr--;
                    return createToken(ID,start,read_ptr);
                }
            case 53:
                if(BUFFER[read_ptr]>=48 && BUFFER[read_ptr]<=57){
                    state = 53;
                    break;
                }else {
                    read_ptr--;
                    return createToken(ID,start,read_ptr);
                }

            case 55:
                if(BUFFER[read_ptr]=='&'){
                    state = 56;
                    break;
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }

            case 56:
                if(BUFFER[read_ptr]=='&'){
                    return createToken(AND,start,read_ptr);
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }
            case 59:
                if(BUFFER[read_ptr]=='@'){
                    state = 60;
                    break;
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }
            case 60:
                if(BUFFER[read_ptr]=='@'){
                    return createToken(OR,start,read_ptr);
                }else{
                    read_ptr--;
                    return createToken(LEX_ERROR,start,read_ptr);
                }
        }
        read_ptr++;
    }
}
