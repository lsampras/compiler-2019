    # GROUP 2
    # SAMPRAS LOPES - 2016A7PS0125P
    # LAKSH NANWANI - 2016A7PS0044P
    # MEHAK BINDRA - 2016B5A70685P

all : driver.o lexer.o parser.o
	gcc -o stage1exe driver.o lexer.o parser.o

driver.o: driver.c
	gcc -c driver.c

lexer.o:lexer.c lexerDef.h lexer.h
	gcc -c lexer.c

parser.o:parser.c parserDef.h parser.h
	gcc -c parser.c

clean:
	rm -f *.o *.out stage1exe

debug: lexer.c lexerDef.h lexer.h parser.c parserDef.h parser.h driver.c clean
	gcc -ggdb3 -std=c99 -c parser.c driver.c lexer.c
	gcc -ggdb3 -std=c99 -o stage1exe parser.o lexer.o driver.o

memcheck: testcase1.txt debug
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./stage1exe ../testcase1.txt ../a.txt 
