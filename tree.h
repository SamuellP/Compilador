#define maxchildren 6


typedef enum {
    SpecialWord, Assign, Declara_Var, Declara_Func, id, Condicao, Op_Cond, Op_Ari
}NodeKind;

typedef enum {
    If, Repeat, Read, Write
}Kind;

typedef enum {
    Integer, Double
}Type;

typedef struct treeNode {
    char* nameNode;
    struct treeNode *filhos[maxchildren];
    struct treeNode *irmao;
    char* valor;
    int linha;
    NodeKind nodeKind;
    Kind kind; // Só analisa este atributo se "nodeKind" for igual à "SpecialWord" //
    //TokenType tokenType;
    Type type; // Só analisa este atributo se "nodeKind" for igual à "ID" //
}TreeNode;
