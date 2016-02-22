%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "util.h"
extern int yywrap(void) { }
int linha;
int coluna;
char* tokenString;
char* lastID = NULL;
Type lastType;

%} 

%%

[s][e]						          {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return IF;}
"então"					              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return THEN;}
"senão"					              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return ELSE;}
[f][i][m]					          {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return END;}
[r][e][p][i][t][a]				      {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return REPEAT;}
[f][l][u][t][u][a][n][t][e]			  {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); lastType = Double; return FLOAT;}
"até"					              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return UNTIL;}
[l][e][i][a]					      {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return READ;}
[e][s][c][r][e][v][e]				  {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return WRITE;}
[r][e][t][o][r][n][a]                 {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return RETURN;}
[i][n][t][e][i][r][o]				  {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); lastType = Integer; return INT;}
[E]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return EXP;}
[+]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return PLUS;}
[-]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return MINUS;}
[*]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return TIMES;}
[/]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return OVER;}
[=]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return EQUAL;}
[,]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return COMMA;}
[<]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return LT;}
[(]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return LPARENT;}
[)]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return RPARENT;}
[;]						              {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return SEMICOLON;}
[:][=]						          {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return ASSIGN;}
[0-9]+\.[0-9]*  				      {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return DOUBLE;}
[0-9]+						          {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); return INTEGER;}
[a-zA-Zá-ñÁ-Ñ][a-zA-Zá-ñÁ-Ñ0-9]*	  {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext); copy(&lastID,yytext); return ID;}
[\n]						          {linha++; coluna = 1; tokenString = NULL;}
[ ]+						          {refreshColumn(&coluna, tokenString); copy(&tokenString,yytext);}
[\t]						          {refreshColumn(&coluna, tokenString); copy(&tokenString,"        ");}
"{"			                          { char c;
			                             do{
	           	                           c = input();
	                                       if (c == EOF) break;
                    	                   if (c == '\n') { linha++; coluna = 1; tokenString = NULL; }
                                           else { coluna++; }
                                         }while (c != '}');
                 	                  }

<<EOF>>			                      { return FINISH; }

[.]               	                  {return ERROR;}	

%%

int initializeLex(char *in, char *out){
    yyin = fopen(in,"r+"); if(yyin == NULL) { printf("Erro na abertura do arquivo!"); return 0; }
    yyout = fopen(out,"w+"); if(yyout == NULL) { printf("Erro na abertura do arquivo!"); return 0; }

    return 1;
}

int hasMoreToken(){
    FILE *aux = yyin;
    return !feof(aux);
}

TokenType getToken(){
    static int firstTime = 1;

    Token token;
    TokenType tokenType;

    if(firstTime){
	firstTime = 0;
	linha = 1;
   	coluna = 1;
	tokenString = NULL;
	//yyin = source;
	//yyout = destination;
    }

    tokenType = yylex();

    fprintf(yyout, "[ %d, %s, %s ]\n", linha, getTokenString(tokenType), yytext);
      

    token.tokenVal = tokenType;
    token.attribute.stringVal = (char*)calloc(strlen(yytext)+1,sizeof(char));
    strcpy(token.attribute.stringVal, yytext);

    return tokenType;
}

/*int main(int argc, char *argv[]){

    if(argc != 3){
	printf("\nParametros invalidos\n");
	return 0;
    }

    yyin = fopen(argv[1],"r+"); if(yyin == NULL) { printf("Erro na abertura do arquivo!"); return 0; }
    yyout = fopen(argv[2],"w+"); if(yyout == NULL) { printf("Erro na abertura do arquivo!"); return 0; }
	
    while(hasMoreToken()){
	getToken();
        getToken();
	getToken();
	getToken();
	getToken();
	getToken();
	getToken();


    }

    return 0;
}*/

