struct _tn;
struct _l;
typedef struct _m{
	int rows;
	int columns;
}Msize;


int checkType(struct _tn * t,struct _tn * symbolTable);
int validateArithmeticExpression(struct _tn * t, struct _tn * st);
int checkBooleanExpression(struct _tn * t, struct _tn * st);
void semanticsChecker(struct _tn * ast, struct _tn * symbolTableNode);
Msize * ensureMatrixSize(struct _tn * t, struct _tn * st, int line);
struct _tn *  ensureFunctionDeclared(char * id, struct _tn * tn);
void ensureCorrectFunctionCall(struct _tn * t, struct _l * l,struct _tn * st);