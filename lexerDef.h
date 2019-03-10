/*  GROUP 2
    SAMPRAS LOPES - 2016A7PS0125P
    LAKSH NANWANI - 2016A7PS0044P
    MEHAK BINDRA - 2016B5A70685P
*/
#ifndef LEXERDEF
#define LEXERDEF

#include<stdio.h>   
#include <stdlib.h>
#include <stddef.h>
#include<string.h>
#include<stdbool.h>
#define TABLESIZE 512
#define TOKENCOUNT 56//including eps and $

enum tok {
    ASSIGNOP,
    COMMENT,
    FIELDID,
    ID,
    NUM,
    RNUM,
    FUNID,
    RECORDID,
    WITH,
    PARAMETERS,
    END,
    WHILE,
    TYPE,
    MAIN,
    GLOBAL,
    PARAMETER,
    LIST,
    SQL,
    SQR,
    INPUT,
    OUTPUT,
    INT,
    REAL,
    COMMA,
    SEM,
    COLON,
    DOT,
    ENDWHILE,
    OP,
    CL,
    IF,
    THEN,
    ENDIF,
    READ,
    WRITE,
    RETURN,
    PLUS,
    MINUS,
    MUL,
    DIV,
    CALL,
    RECORD,
    ENDRECORD,
    ELSE,
    AND,
    OR,
    NOT,
    LT,
    LE,
    EQ,
    GT,
    GE,
    NE,
    LEX_ERROR,
    EPS,
    $
};


struct token{
    enum tok type;
    char * lexeme;
    int line_no;
};



typedef struct token token;

struct LinkedListToken{
    struct LinkedListToken *next;
    token tk;
};

typedef struct LinkedListToken * llToken;

struct HASH{
    struct HASH * next;
    char * key;
    enum tok value;
};

typedef struct HASH hashnode;


hashnode * TABLE[TABLESIZE];



#endif
