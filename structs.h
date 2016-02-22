
typedef enum {
     DOUBLE, INTEGER, ID, IF, THEN, ELSE, END, REPEAT, FLOAT, UNTIL, READ, WRITE, RETURN, INT, EXP, PLUS, MINUS, TIMES, OVER, EQUAL, 
     COMMA, LT, LPARENT, RPARENT, SEMICOLON, ASSIGN, ERROR, FINISH

}TokenType;

typedef struct {
    TokenType tokenVal;
    union {
	char *stringVal;
	int numVal;
    }attribute;
}Token;
