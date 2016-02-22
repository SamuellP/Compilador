#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

char* getTokenString(TokenType tokenType){

    char *s = (char*)calloc(10,sizeof(char));

    if(tokenType == DOUBLE){
	strcpy(s,"DOUBLE");
	return s;
    }
    else if(tokenType == INTEGER){
	strcpy(s,"INTEGER");
	return s;
    }
    else if(tokenType == ID){
	strcpy(s,"ID");
	return s;
    }
    else if(tokenType == IF){
	strcpy(s,"IF");
	return s;
    }
    else if(tokenType == THEN){
	strcpy(s,"THEN");
	return s;
    }
    else if(tokenType == ELSE){
	strcpy(s,"ELSE");
	return s;
    }
    else if(tokenType == END){
	strcpy(s,"END");
	return s;
    }
    else if(tokenType == REPEAT){
	strcpy(s,"REPEAT");
	return s;
    }
    else if(tokenType == FLOAT){
	strcpy(s,"FLOAT");
	return s;
    }
    else if(tokenType == UNTIL){
	strcpy(s,"UNTIL");
	return s;
    }
    else if(tokenType == READ){
	strcpy(s,"READ");
	return s;
    }
    else if(tokenType == WRITE){
	strcpy(s,"WRITE");
	return s;
    }
    /*else if(tokenType == PROCEDURE){
	strcpy(s,"PROCEDURE");
	return s;
    }*/
    else if(tokenType == INT){
	strcpy(s,"INT");
	return s;
    }
    else if(tokenType == EXP){
	strcpy(s,"EXP");
	return s;
    }
    else if(tokenType == PLUS){
	strcpy(s,"PLUS");
	return s;
    }
    else if(tokenType == MINUS){
	strcpy(s,"MINUS");
	return s;
    }
    else if(tokenType == TIMES){
	strcpy(s,"TIMES");
	return s;
    }
    else if(tokenType == OVER){
	strcpy(s,"OVER");
	return s;
    }
    else if(tokenType == EQUAL){
	strcpy(s,"EQUAL");
	return s;
    }
    else if(tokenType == COMMA){
	strcpy(s,"COMMA");
	return s;
    }
    else if(tokenType == LT){
	strcpy(s,"LT");
	return s;
    }
    else if(tokenType == LPARENT){
	strcpy(s,"LPARENT");
	return s;
    }
    else if(tokenType == RPARENT){
	strcpy(s,"RPARENT");
	return s;
    }
    else if(tokenType == SEMICOLON){
	strcpy(s,"SEMICOLON");
	return s;
    }
    else if(tokenType == ASSIGN){
	strcpy(s,"ASSIGN");
	return s;
    }
    else if(tokenType == ERROR){
	strcpy(s,"ERROR");
	return s;
    }

    return NULL;
}

TreeNode* newNode(char* val){
    TreeNode *treeNode = (TreeNode*)calloc(1,sizeof(TreeNode));
    int i;

    for(i = 0; i < maxchildren; i++)
	treeNode->filhos[i] = NULL;

    treeNode->irmao = NULL;
    //treeNode->linha = linha;

    //treeNode->tokenType = tokenType;

    treeNode->nameNode = (char*)calloc(strlen(val)+1,sizeof(char));
    strcpy(treeNode->nameNode,val);

    return treeNode;
}

void refreshColumn(int *column, char* token){
    if(token == NULL) return;

    *column += strlen(token);
}

void copy(char** target, char* source){
    target[0] = (char*)calloc(strlen(source)+1,sizeof(char));
    strcpy(target[0],source);
}

TreeNode* addIrmao(TreeNode* a, TreeNode* b){

    /*if(a != NULL){
        TreeNode* aux = a;
        while(aux->irmao != NULL)
            aux = aux->irmao;
        aux->irmao = b;
        return a;

    }else
        return b;*/
    b->irmao = a;
    return b;
}

/*void printTree(TreeNode *tree){

    int i = 0;

    while(tree->filhos[i] != NULL){

        printTree(tree->filhos[i]);
        i++;

    }

    TreeNode* aux = tree->irmao;

    while(aux != NULL){
        printTree(aux);
        aux = aux->irmao;
    }

    printf("%s\n", tree->nameNode);

}*/

void printTree(TreeNode *tree){

    if(tree->irmao != NULL)
        printTree(tree->irmao);

    int i = 0;
    while(tree->filhos[i] != NULL){

            printTree(tree->filhos[i]);
            i++;
        }
    printf("%s\n", tree->nameNode);

    /*if(!strcmp(tree->nameNode,"RETORNA")){

        printf("--- %s\n", tree->filhos[0]->nameNode);
        printf("e - %s\n", tree->filhos[0]->filhos[0]->nameNode);
        printf("d - %s\n", tree->filhos[0]->filhos[1]->nameNode);
    }*/
}
