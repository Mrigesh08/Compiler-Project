typedef struct _m{
	int rows;
	int columns;
}Msize;

struct _tn;
int checkType(struct _tn * t,struct _tn * symbolTable);

int validateArithmeticExpression(struct _tn * t, struct _tn * st);
int checkBooleanExpression(struct _tn * t, struct _tn * st);
void semanticsChecker(struct _tn * ast, struct _tn * symbolTableNode);
Msize * ensureMatrixSize(struct _tn * t, struct _tn * st, int line);