%{
#define YYPARSER
#include <stdio.h>
#define YYSTYPE TreeNode*
#include "tree.h"
extern char* yytext;
extern char* lastID;
extern int linha;
extern int coluna;
extern Type lastType;

char* savedID = NULL;

TreeNode *tree;

%}

%token DOUBLE INTEGER ID
%token IF THEN ELSE
%token END REPEAT FLOAT
%token UNTIL READ WRITE
%token RETURN INT EXP
%token PLUS MINUS TIMES
%token OVER EQUAL COMMA
%token LT LPARENT RPARENT
%token SEMICOLON ASSIGN ERROR
%token FINISH

%%

inicio 					   : stmt_seq 												   { tree = $1; return;}
       					   ;

stmt_seq    			   : stmt_global stmt_seq 									   { 
																		   		         YYSTYPE t = $2;
																                         if (t != NULL){
																                         	while (t->irmao != NULL)
																                         		t = t->irmao;
																                         	t->irmao = $1;
																                         	$$ = $2; 
																                     	 }else {
																                     	 	$$ = $1;
																                     	 }
																                 	   }
            			   | stmt_global{ $$ = $1; }
	    				   | FINISH { $$ = NULL; }
            			   ;

stmt_global 			   : declaracao sc 											   { $$ = $1; }
					       | declaracao_funcao 										   { $$ = $1; }
					       | chamada_funcao sc 										   { $$ = $1; }
					       | atribuicao sc 											   { $$ = $1; }
					       | lei_esc 												   { $$ = $1; }
					       | condicao 												   { $$ = $1; }
					       | repeticao sc 											   { $$ = $1; }
					       | error 													   { $$ = NULL; }
					       ;

stmt 					   : stmt declaracao sc 								   	   { $2->irmao = $1; $$ = $2; }
					       | stmt chamada_funcao sc 								   { $2->irmao = $1; $$ = $2; }
					       | stmt atribuicao sc 									   { $2->irmao = $1; $$ = $2; }
					       | stmt lei_esc 											   { $2->irmao = $1; $$ = $2; }
					       | stmt condicao 											   { $2->irmao = $1; $$ = $2; }
					       | stmt repeticao sc 										   { $2->irmao = $1; $$ = $2; }
					       | stmt retorno 											   { $2->irmao = $1; $$ = $2; }
					       | declaracao sc 											   { $$ = $1; }
					       | chamada_funcao sc 										   { $$ = $1; }
					       | atribuicao sc 											   { $$ = $1; }
					       | lei_esc 												   { $$ = $1; }
					       | condicao 												   { $$ = $1; }
					       | repeticao sc 											   { $$ = $1; }
					       | retorno												   { $$ = $1; }
					       | error 													   { $$ = NULL; }
					       ;

atribuicao 				   : id ASSIGN expressao_simples 							   { $$ = newNode("ASSIGN");
																				 	     $$->filhos[0] = $1;
																				 	     $$->irmao = NULL;
																				 	     $$->valor = NULL;
																				 	     $$->nodeKind = Assign;
																				 	     $$->filhos[1] = $3;
															      			 	  	   }

		   				   | id ASSIGN chamada_funcao 								   { $$ = newNode("ASSIGN");
									 													 $$->filhos[0] = $1;
									 													 $$->irmao = NULL;
									 													 $$->valor = NULL;
									 													 $$->nodeKind = Assign;
									 													 $$->filhos[1] = $3;
				      			   	  												   }
	     				   ;

id 						   : ID 													   { $$ = newNode("ID");
																					     $$->irmao = NULL;
																					     copy(&$$->valor,yytext);
																					     $$->nodeKind = id;
																			             $$->type = -1;
																			           }
        				   ;

expressao 				   : expressao_simples op_condicional expressao_simples 	   { $$ = newNode("CONDICAO");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $2;
																					     $$->filhos[2] = $3;
																					     $$->irmao = NULL;
																					     $$->nodeKind = Condicao;
																					   }
		 				   | expressao_simples 										   { $$ = $1;
		 					 														   }
		 				   ;

expressao_simples 		   : expressao_simples PLUS termo 							   { $$ = newNode("+");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					     $$->nodeKind = Op_Ari;
																					   }
				 		   | expressao_simples MINUS termo 							   { $$ = newNode("-");
														   								 $$->filhos[0] = $1;
														   								 $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					     $$->nodeKind = Op_Ari;
																					   }
				 		   | termo 													   { $$ = $1;
				 		 		 													   }
				 		   ;

termo 					   : termo TIMES fator 										   { $$ = newNode("*"); 
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					     $$->nodeKind = Op_Ari;
																					   }
	 					   | termo OVER fator  										   { $$ = newNode("/"); 
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					     $$->nodeKind = Op_Ari;
						 															   }
					   	   | fator 													   { $$ = $1;
					 		 														   }
					 	   ;

fator 					   : LPARENT expressao_simples RPARENT 						   { $$ = $2;
										 											   }
	 					   | valor 													   { $$ = $1;
	 		 																		   }
	 					   | id 	 												   { $$ = $1;
	 		 																		   }
	 					   ;


declaracao 				   : tipo lista_declaracao 									   { $$ = newNode("DECLARACAO");
																	     				 $$->filhos[0] = $1;
																	     				 $$->filhos[1] = $2;
																	     				 $$->irmao = NULL;
																	     				 $$->valor = NULL;
																	     				 $$->nodeKind = Declara_Var;
																	   				   }
	  	   				   ;

lista_declaracao : 		   atribuicao COMMA lista_declaracao 						   { $$ = newNode("LISTA_DECLARACAO");
														       						     $$->filhos[0] = $1;
														       						     $$->filhos[1] = $3;
														       						     $$->irmao = NULL;
																	     			     $$->valor = NULL;
																	     			     $$->nodeKind = Declara_Var;
														     						   }
		 				   | atribuicao 											   { $$ = $1;
		 			  																   }
		 				   | id COMMA lista_declaracao 								   { $$ = newNode("LISTA_DECLARACAO");
														 							     $$->filhos[0] = $1;
														 							     $$->filhos[1] = $3; 
														 							     $$->irmao = NULL;
														 							     $$->valor = NULL;
														 							     $$->nodeKind = Declara_Var;
														 							   }
		 				   | id 													   { $$ = $1; 
		 	  																		   }
		 				   ;

condicao 				   : IF expressao THEN stmt END 							   { $$ = newNode("IF");
																					     $$->filhos[0] = $2;
																					     $$->filhos[1] = $4;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					     $$->nodeKind = SpecialWord;
																					     $$->kind = If;
																					   }

	 					   | IF expressao THEN stmt ELSE stmt END					   { $$ = newNode("IF");
																			    		 $$->filhos[0] = $2;
																			    		 $$->filhos[1] = $4;
																			    		 $$->filhos[2] = $6;
																			    		 $$->irmao = NULL;
																			    		 $$->valor = NULL;
																			    		 $$->nodeKind = SpecialWord;
																			    		 $$->kind = If;
										 											   }
	 					   ;

op_condicional 			   : EQUAL  												   { $$ = newNode("="); 
																					     $$->irmao = NULL;
																					   }
	       				   | LT 													   { $$ = newNode("<");
																	       			     $$->irmao = NULL;
																	   				   }
	       				   ;

repeticao 				   : REPEAT stmt UNTIL expressao 							   { $$ = newNode("REPETICAO");
											  											 $$->filhos[0] = $2;
																					     $$->filhos[1] = $4;
																					     $$->irmao = NULL;
																					     $$->nodeKind = SpecialWord;
																					     $$->kind = Repeat;
																					   }
	  					   ;

declaracao_funcao 		   : tipo id LPARENT lista_parametros_dec_func RPARENT stmt END{ $$ = newNode("DECL_FUNCAO");
																						 $$->filhos[0] = $1;
																						 $$->filhos[1] = $2;
																						 $$->filhos[2] = $4; 
																						 $$->filhos[3] = $6;
																						 $$->irmao = NULL;
																						 $$->valor = NULL;
																						 $$->nodeKind = Declara_Func;
												 								  	   }
				 		   | tipo id LPARENT lista_parametros_dec_func RPARENT END 	   { $$ = newNode("DECL_FUNCAO");
																						 $$->filhos[0] = $1;
																						 $$->filhos[1] = $2;
																						 $$->filhos[2] = $4;
																						 $$->irmao = NULL;
																						 $$->valor = NULL;
																						 $$->nodeKind = Declara_Func;
																		 			   }
				 		   | tipo id LPARENT RPARENT stmt END 						   { $$ = newNode("DECL_FUNCAO");
																						 $$->filhos[0] = $1;
																						 $$->filhos[1] = $2;
																						 $$->filhos[2] = $5;
																						 $$->irmao = NULL;
																						 $$->valor = NULL;
																						 $$->nodeKind = Declara_Func;
														 							   }
				 		   | tipo id LPARENT RPARENT END 							   { $$ = newNode("DECL_FUNCAO");
																						 $$->filhos[0] = $1;
																						 $$->filhos[1] = $2;
																						 $$->irmao = NULL;
																						 $$->valor = NULL;
																						 $$->nodeKind = Declara_Func; 
														 							   }
				 		   | tipo id LPARENT lista_parametros_dec_func RPARENT sc 	   { $$ = newNode("DECL_FUNCAO");
																						 $$->filhos[0] = $1;
																						 $$->filhos[1] = $2;
																						 $$->filhos[2] = $4; 
																						 $$->irmao = NULL;
																						 $$->valor = NULL;
																						 $$->nodeKind = Declara_Func;
																					   };
				 		   | tipo id LPARENT RPARENT sc 						  	   { $$ = newNode("DECL_FUNCAO");
																						 $$->filhos[0] = $1;
																						 $$->filhos[1] = $2;
																						 $$->irmao = NULL;
																						 $$->valor = NULL;
																						 $$->nodeKind = Declara_Func;
												  									   }; 
	    	       		   ;

retorno 				   : RETURN expressao_simples sc 							   { $$ = newNode("RETORNA");
																					     $$->filhos[0] = $2;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
	   					   | RETURN chamada_funcao sc 								   { $$ = newNode("RETORNA");
																					     $$->filhos[0] = $2;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
						   ; 

lista_parametros_dec_func  : tipo id COMMA lista_parametros_dec_func 				   { $$ = newNode("PARAMETROS_DEC_FUNC");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $2;
																					     $$->filhos[2] = $4;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
		 				   | tipo id 												   { $$ = newNode("PARAMETROS_DEC_FUNC");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $2;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
		 				   ;

chamada_funcao 			   : id LPARENT lista_parametros_cham_func RPARENT 			   { $$ = newNode("CHAMADA_FUNC");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
			   			   | id LPARENT RPARENT 									   { $$ = newNode("CHAMADA_FUNC");
															     					     $$->filhos[0] = $1;
															     					     $$->irmao = NULL;
															     					     $$->valor = NULL;
																					   }
		     			   ;

lista_parametros_cham_func : id COMMA lista_parametros_cham_func    				   { $$ = newNode("PARAMETROS_CHAM_FUNC");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
			   			   | valor COMMA lista_parametros_cham_func 				   { $$ = newNode("PARAMETROS_CHAM_FUNC");
																					     $$->filhos[0] = $1;
																					     $$->filhos[1] = $3;
																					     $$->irmao = NULL;
																					     $$->valor = NULL;
																					   }
			   			   | id    													   { $$ = $1;
			   			   		  													   }
			   			   | valor 													   { $$ = $1;
			   			   		   													   }
			   			   ;


tipo 					   : INT 													   { $$ = newNode("INT"); 
																					     $$->irmao = NULL;
																					   }
     					   | FLOAT 													   { $$ = newNode("FLOAT");
													     					   			 $$->irmao = NULL;
													     							   }
     					   ;

sc 						   : SEMICOLON 												   {  }
   						   ;

valor 					   : INTEGER 												   { $$ = newNode("VAL_INT"); 
															      					     $$->irmao = NULL;
															      					     copy(&$$->valor,yytext);
															    					   }
      					   | DOUBLE 												   { $$ = newNode("VAL_DOUBLE");
												      			 					     $$->irmao = NULL;
												      			 					     copy(&$$->valor,yytext);
												  			   						   }
      					   ;

lei_esc 				   : READ lista_lei_esc sc 									   { $$ = newNode("READ");
																						 $$->filhos[0] = $2;
																						 $$->irmao = NULL;
																					   }
					       | WRITE lista_lei_esc sc 								   { $$ = newNode("WRITE"); 
													        						     $$->filhos[0] = $2;
													        						     $$->irmao = NULL;
													    							   }
	;

lista_lei_esc 			   : id COMMA lista_lei_esc 								   { $$ = newNode("LISTA_LEIT_ESCRI");
																	    			     $$->filhos[0] = $1;
																	    			     $$->filhos[1] = $3;
																	    			     $$->irmao = NULL;
																	    			     $$->valor = NULL;
																	  				   }
	     				   | id 													   { $$ = $1;
	     	  					 													   }
	     				   ;


%%

yyerror(char *s){
    printf("linha: %d coluna: %d  - ocorreu o seguinte erro: %s\n", linha, coluna, s);
    exit(1);
}

static int yylex(void)
{ return getToken(); }


/*int main(int argc, char *argv[]){
    if(argc != 3){
	printf("\nParametros invalidos\n");
	return 0;
    }
	
    if(!initializeLex(argv[1], argv[2]))
	return 0;

    yyparse();

    printTree(tree);

    return 0;
}*/