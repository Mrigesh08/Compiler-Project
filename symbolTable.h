struct _tn;

typedef struct _l{
	struct _tn * inputList;
	struct _tn * outputList;
}List;

void createSymbolTable(struct _tn * t,  struct _tn * tn);
void printSymbolTable(struct _tn *t);
struct _tn * lookupFunctionName(struct _tn * t, char * x);
int findSymbolType(struct _tn * t,char *x);
int getTypeFromSymbolTable(char * id, struct _tn * st);
struct _tn * getAstNodeFromSymbolTable(char * id, struct _tn * st);
int getType(char * x);