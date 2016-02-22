CC=gcc
CXX=g++
DEBUG=-ggdb
CFLAGS= -c
LDFLAGS=
CXXFLAGS=-std=gnu++11 -c
OPT=-O0

all:	lex_sintax corrigir_arquivo lib

lex_sintax:
	flex lexica.l
	yacc -d sintax.y

lib:	
	g++ -c -Wall -Werror -fpic hash.cpp -I/home/samuel/libraries/boost/include/
	g++ -shared -o libhash.so hash.o
	gcc -L/home/samuel/Documentos/BCC/6°\ Semestre/Compiladores/compilador_tiny/ -Wl,-rpath=/home/samuel/Documentos/BCC/6°\ Semestre/Compiladores/compilador_tiny/ -Wall -o teste y.tab.c lex.yy.c util.c semantica.c -lhash

corrigir_arquivo:
	sed  -i 's/yychar = yylex ();/yychar = getToken() + 258;/' y.tab.c

clean:
	rm -f *.o 
	rm -f lex.yy.c y.tab.c y.tab.h 
	rm -f meuTiny