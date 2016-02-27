#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

extern TreeNode *tree;

char escopo[100];

/*****************************************************/

void imprimirString(char str[]);
		int usaHash();
		int containsKey(char *key, char* scope);
		void putVariable(char *variable, char *type, char *scope);
		void adicionaParametro(char* id, char* parametro);
		void adicionaId(char* identificadorFuncao, char* id);
		char* getType(char* variable, char* scope);

/*****************************************************/

void analiseSemantica(TreeNode* tree);

void trataRetorno(TreeNode* tree);

void trataRepeticao(TreeNode* tree);

void trataIF(TreeNode* tree);

void trataExpressao(TreeNode* tree);

void trataDeclaracaoFuncao(TreeNode* tree);

void trataParametrosDeclaracaoDeFuncao(TreeNode* tree, char* id);

void trataDeclaracao(TreeNode* treeNode);

void trataListaDeclaracao(TreeNode* aux, char* tipo);

void trataAtribuicaoNaDeclaracao(TreeNode* aux, char* tipo);

void aritmetica(TreeNode* treeNode);

/****************************************************************/


void aritmetica(TreeNode* treeNode){

	if(!strcmp(treeNode->nameNode,"+") || !strcmp(treeNode->nameNode,"-") || !strcmp(treeNode->nameNode,"*") || !strcmp(treeNode->nameNode,"/")){

		aritmetica(treeNode->filhos[0]);
		aritmetica(treeNode->filhos[1]);

	}else if(!strcmp(treeNode->nameNode,"ID")){

		if(strcmp(escopo,"&") && !containsKey(treeNode->valor, escopo)){
			if(!containsKey(treeNode->valor, "&")){
				printf("\n\n A variavel %s não foi declarada \n\n", treeNode->valor);
				exit(1);
			}
		}else if(!containsKey(treeNode->valor, escopo)){
			printf("\n\n A variavel %s não foi declarada \n\n", treeNode->valor);
			exit(1);
		}
	}


}

void trataAtribuicaoNaDeclaracao(TreeNode* aux, char* tipo){
	    char* tipoId2;

		if(containsKey(aux->filhos[0]->valor, escopo)){
			printf("\n\nErro do tipo semantico. Variavel %s ja declarada...\n\n", aux->filhos[0]->valor);
		    exit(1);
		}

		putVariable(aux->filhos[0]->valor,tipo,escopo);

		if(!strcmp(aux->filhos[1]->nameNode,"ID")){

			if(strcmp(escopo,"&") && !containsKey(aux->filhos[1]->valor, escopo)){
				if(!containsKey(aux->filhos[1]->valor, "&")){
					printf("\n\nErro do tipo semantico. Variavel %s nao foi declarada... ***\n\n", aux->filhos[1]->valor);
					exit(1);
				}else{
					tipoId2 = getType(aux->filhos[1]->valor,"&");
				}
			}else if(!containsKey(aux->filhos[1]->valor, escopo)){
				printf("\n\nErro do tipo semantico. Variavel %s nao foi declarada... ***\n\n", aux->filhos[1]->valor);
				exit(1);
			}else{
				tipoId2 = getType(aux->filhos[1]->valor, escopo);
			}

			if(!strcmp(tipo,"FLOAT") && !strcmp(tipoId2,"INT")){
				printf("\n\nO valor inteiro não pode ser adicionado a uma variável do tipo flutuante\n\n");
				exit(1);
			}

			if(!strcmp(tipo,"INT") && !strcmp(tipoId2,"FLOAT")){
				printf("\n\nO valor flutuante não pode ser adicionado a uma variável do tipo inteiro\n\n");
				exit(1);
			}

		}

		if(!strcmp(aux->filhos[1]->nameNode,"VAL_INT")) {

			if(!strcmp(tipo,"FLOAT")){
				printf("\n\nO valor inteiro não pode ser adicionado a uma variável do tipo flutuante\n\n");
				exit(1);
			}

		}

	    if(!strcmp(aux->filhos[1]->nameNode,"VAL_DOUBLE")) { 

	    	if(!strcmp(tipo,"INT")){
				printf("\n\nO valor flutuante não pode ser adicionado a uma variável do tipo inteiro\n\n");
				exit(1);
			}
	    }

	    if(!strcmp(aux->filhos[1]->nameNode,"+") || !strcmp(aux->filhos[1]->nameNode,"-") || !strcmp(aux->filhos[1]->nameNode,"*") || !strcmp(aux->filhos[1]->nameNode,"/")){

			aritmetica(aux->filhos[1]);
			
		}

}

void trataListaDeclaracao(TreeNode* aux, char* tipo){

	if(!strcmp(aux->filhos[0]->nameNode,"ASSIGN")){

			trataAtribuicaoNaDeclaracao(aux->filhos[0], tipo);
		}else if(!strcmp(aux->filhos[0]->nameNode,"ID")){

			if(containsKey(aux->filhos[0]->valor, escopo)){
				printf("\n\nErro do tipo semantico. Variavel %s ja declarada...\n\n", aux->filhos[0]->valor);
		    	exit(1);
			}else{

				putVariable(aux->filhos[0]->valor, tipo, escopo);
			}
		}

		if(!strcmp(aux->filhos[1]->nameNode,"ASSIGN")){

			trataAtribuicaoNaDeclaracao(aux->filhos[1], tipo);

		}else if(!strcmp(aux->filhos[1]->nameNode,"ID")){
			

			if(containsKey(aux->filhos[1]->valor, escopo)){

				printf("\n\nErro do tipo semantico. Variavel %s ja declarada...\n\n", aux->filhos[1]->valor);
		    	exit(1);
			}else{

				putVariable(aux->filhos[1]->valor, tipo, escopo);
			}

		}else if(!strcmp(aux->filhos[1]->nameNode,"LISTA_DECLARACAO")){

			trataListaDeclaracao(aux->filhos[1], tipo);
		}

}

void trataDeclaracao(TreeNode* treeNode){
	TreeNode* aux;
	char tipo[7];

	if(!strcmp(treeNode->filhos[0]->nameNode,"INT"))
		strcpy(tipo,"INT");
	else
		strcpy(tipo,"FLOAT");


	if(!strcmp(treeNode->filhos[1]->nameNode,"ASSIGN")){
		aux = treeNode->filhos[1];

		trataAtribuicaoNaDeclaracao(aux,tipo);

			
	}else if(!strcmp(treeNode->filhos[1]->nameNode,"LISTA_DECLARACAO")){

		aux = treeNode->filhos[1];

		trataListaDeclaracao(aux, tipo);

	}else if(!strcmp(treeNode->filhos[1]->nameNode,"ID")){
		if(containsKey(treeNode->filhos[1]->valor, escopo)){
			printf("\n\nErro do tipo semantico. Variavel %s ja declarada...\n\n", treeNode->filhos[1]->valor);
		    exit(1);
		}else{
			putVariable(treeNode->filhos[1]->valor, tipo, escopo);
		}
	}
	
}

void trataParametrosDeclaracaoDeFuncao(TreeNode* tree, char* id){

	adicionaParametro(id, tree->filhos[0]->nameNode);
	adicionaId(id, tree->filhos[1]->nameNode);

	putVariable(tree->filhos[1]->valor, tree->filhos[0]->nameNode, id);

	if(tree->filhos[2] != NULL)
		trataParametrosDeclaracaoDeFuncao(tree->filhos[2],id);	

}

void trataDeclaracaoFuncao(TreeNode* tree){

	char tipo[10];
	char id[100];

	strcpy(tipo,tree->filhos[0]->nameNode);

	strcpy(id,tree->filhos[1]->valor);

	strcpy(escopo,id);

	if(containsKey(id,"&")){
		printf("\n\nUma funcao/variavel ja foi declarada com o id %s\n\n",id);
		exit(1);
	}

	putFunction(id, tipo);


	if(!strcmp(tree->filhos[2]->nameNode,"PARAMETROS_DEC_FUNC")){

		trataParametrosDeclaracaoDeFuncao(tree->filhos[2], id);

		if(tree->filhos[3] != NULL){
			analiseSemantica(tree->filhos[3]);
		}

	}else if(tree->filhos[2] != NULL){
		analiseSemantica(tree->filhos[2]);
	}

	strcpy(escopo,"&");
}

void trataExpressao(TreeNode* tree){

	aritmetica(tree->filhos[0]);

	if(tree->filhos[1] != NULL)
		aritmetica(tree->filhos[2]);
}

void trataIF(TreeNode* tree){

	trataExpressao(tree->filhos[0]);

	analiseSemantica(tree->filhos[1]);

	if(tree->filhos[2] != NULL)
		analiseSemantica(tree->filhos[2]);
}

void trataRepeticao(TreeNode* tree){

	analiseSemantica(tree->filhos[0]);

	trataExpressao(tree->filhos[1]);
}

void trataRetorno(TreeNode* tree){
    
	if(!strcmp(tree->filhos[0]->nameNode,"ID")){
		if(!containsKey(tree->filhos[0]->valor,escopo)){
			if(!containsKey(tree->filhos[0]->valor,"&") || isFunction(tree->filhos[0]->valor))
				printf("\n\nA variavel %s nao foi declarada nesse escopo\n\n",tree->filhos[0]->valor);
		}
	}

	else if(!strcmp(tree->filhos[0]->nameNode,"+") || !strcmp(tree->filhos[0]->nameNode,"-") || !strcmp(tree->filhos[0]->nameNode,"*") || !strcmp(tree->filhos[0]->nameNode,"/")){
		aritmetica(tree->filhos[0]);
	}
}

void trataAtribuicao(TreeNode* tree){

	char* tipoId1;
	char* tipoId2;

	if(strcmp(escopo,"&") && !containsKey(tree->filhos[0]->valor, escopo)){
		if(!containsKey(tree->filhos[0]->valor, "&")){
			printf("\n\nErro do tipo semantico. Variavel %s nao foi declarada...\n\n", tree->filhos[0]->valor);
			exit(1);
		}else{
			tipoId1 = getType(tree->filhos[0]->valor, "&");
		}
	}else if(!containsKey(tree->filhos[0]->valor, escopo)){
		printf("\n\nErro do tipo semantico. Variavel %s nao foi declarada...\n\n", tree->filhos[0]->valor);
		exit(1);
	}else{
		tipoId1 = getType(tree->filhos[0]->valor, escopo);
	}

	if(!strcmp(tree->filhos[1]->nameNode,"ID")){

		if(strcmp(escopo,"&") && !containsKey(tree->filhos[1]->valor, escopo)){
			if(!containsKey(tree->filhos[1]->valor, "&")){
				printf("\n\nErro do tipo semantico. Variavel %s nao foi declarada...\n\n", tree->filhos[1]->valor);
				exit(1);
			}else{
				tipoId2 = getType(tree->filhos[1]->valor, "&");
			}
		}else if(!containsKey(tree->filhos[1]->valor, escopo)){
			printf("\n\nErro do tipo semantico. Variavel %s nao foi declarada...\n\n", tree->filhos[1]->valor);
			exit(1);
		}else{
			tipoId2 = getType(tree->filhos[1]->valor, escopo);
		}

		if(!strcmp(tipoId1,"FLOAT") && !strcmp(tipoId2,"INT")){
			printf("\n\nO valor inteiro não pode ser adicionado a uma variável do tipo flutuante\n\n");
			exit(1);
		}

		if(!strcmp(tipoId1,"INT") && !strcmp(tipoId2,"FLOAT")){
			printf("\n\nO valor flutuante não pode ser adicionado a uma variável do tipo inteiro\n\n");
			exit(1);
		}

	}

	if(!strcmp(tree->filhos[1]->nameNode,"VAL_INT")) {

		if(!strcmp(tipoId1,"FLOAT")){
			printf("\n\nO valor inteiro não pode ser adicionado a uma variável do tipo flutuante\n\n");
			exit(1);
		}

	}

	if(!strcmp(tree->filhos[1]->nameNode,"VAL_DOUBLE")) { 

	   	if(!strcmp(tipoId1,"INT")){
			printf("\n\nO valor flutuante não pode ser adicionado a uma variável do tipo inteiro\n\n");
			exit(1);
		}
	}

	if(!strcmp(tree->filhos[1]->nameNode,"+") || !strcmp(tree->filhos[1]->nameNode,"-") || !strcmp(tree->filhos[1]->nameNode,"*") || !strcmp(tree->filhos[1]->nameNode,"/")){

		aritmetica(tree->filhos[1]);
			
	}
}

void analiseSemantica(TreeNode* tree){

	if(tree->irmao != NULL)
        analiseSemantica(tree->irmao);

    
    if(!strcmp(tree->nameNode,"DECLARACAO"))
    	trataDeclaracao(tree);
    else if(!strcmp(tree->nameNode,"DECL_FUNCAO"))
    	trataDeclaracaoFuncao(tree);
    else if(!strcmp(tree->nameNode,"IF"))
    	trataIF(tree);
    else if(!strcmp(tree->nameNode,"REPETICAO"))
    	trataRepeticao(tree);
    else if(!strcmp(tree->nameNode,"RETORNA"))
    	trataRetorno(tree);
    else if(!strcmp(tree->nameNode,"ASSIGN"))
    	trataAtribuicao(tree);



}

int main(int argc, char *argv[]){

	if(argc != 3){
	printf("\nParametros invalidos\n");
	return 0;
    }

    if(!initializeLex(argv[1], argv[2]))
		return 0;

	yyparse();

    strcpy(escopo,"&");

	analiseSemantica(tree);

	return 0;

}