# Compiler Construction Project

### Group-2
---
##### Usage:
	$ make clean
	$ make all
	$ ./stage1exe code.txt parsetreeOutFile.txt
---
##### Makefile:
    make all
		compiles all the files into executable 
	make clean 
		deletes temporary files
	make driver
	make parser
	make lexer 
		compiles only required files

###### Debug functions
	make memcheck 
		run with valgrind
	make debug 
		compile with ggdb3 flag