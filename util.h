#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "tree.h"

char* getTokenString(TokenType tokenType);

TreeNode* newNode(char* val);

void refreshColumn(int *column, char* token);

void copy(char** target, char* source);

void printTree(TreeNode *tree);

TreeNode* addIrmao(TreeNode* a, TreeNode* b);
