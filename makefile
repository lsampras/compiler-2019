    # GROUP 2
    # SAMPRAS LOPES - 2016A7PS0125P
    # LAKSH NANWANI - 2016A7PS0044P
    # MEHAK BINDRA - 2016B5A70685P

all : driver.o lexer.o parser.o ast.o  symtable.o semantic.o
	gcc -o compiler driver.o lexer.o parser.o ast.o symtable.o semantic.o

driver.o: driver.c
	gcc -c driver.c

lexer.o:lexer.c lexerDef.h lexer.h
	gcc -c lexer.c

parser.o:parser.c parserDef.h parser.h
	gcc -c parser.c

semantic.o: semantic.c semantic.h semanticdef.h
	gcc -c semantic.c

symtable.o:symtable.c symtable.h symtabledef.h
	gcc -c symtable.c

ast.o: ast.c astdef.h ast.h
	gcc -c ast.c

clean:
	rm -f *.o *.out compiler

debug: lexer.c lexerDef.h lexer.h parser.c parserDef.h parser.h driver.c clean ast.c astdef.h ast.h symtable.c symtable.h symtabledef.h semantic.c semantic.h semanticdef.h
	gcc -ggdb3 -std=c99 -c parser.c driver.c lexer.c ast.c symtable.c semantic.c
	gcc -ggdb3 -std=c99 -o compiler parser.o lexer.o driver.o ast.o symtable.o semantic.o

memcheck: testcase1.txt debug
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=no ./compiler temptest.txt a.txt 

ast.o :ast.c astdef.h ast.h
	gcc -c ast.c