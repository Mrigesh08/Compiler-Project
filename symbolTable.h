struct _tn;
void createSymbolTable(struct _tn * t,  struct _tn * tn);
void printSymbolTable(struct _tn *t);
struct _tn * lookupFunctionName(struct _tn * t, char * x);
int findSymbolType(struct _tn * t,char *x);
int getTypeFromSymbolTable(char * id, struct _tn * st);