
Para compilar:

1. flex lexica.l

2. yacc -d sintax.y

3. make

4. ./main codigo.txt saida.txt

---------------

Alterar a linha 1462 do arquivo y.tab.c para: yychar = getToken() + 258;


------------------------------------------------------


g++ -c -Wall -Werror -fpic hash.cpp -I/home/samuel/libraries/boost/include/

g++ -shared -o libhash.so hash.o

gcc -L/home/samuel/Documentos/BCC/6°\ Semestre/Compiladores/compilador_tiny/ -Wl,-rpath=/home/samuel/Documentos/BCC/6°\ Semestre/Compiladores/compilador_tiny/ -Wall -o teste teste.c -lhash

gcc -L/home/samuel/Documentos/BCC/6°\ Semestre/Compiladores/compilador_tiny/ -Wl,-rpath=/home/samuel/Documentos/BCC/6°\ Semestre/Compiladores/compilador_tiny/ -Wall -o main util.c y.tab.c lex.yy.c semantica.c -lhash