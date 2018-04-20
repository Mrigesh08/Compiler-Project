struct _tn;



Stack * stack;
Head * g;
extern TreeNode * parseTree;
extern int error_flag2;
FILE * f;


struct _tn * parse();
void printSimpleParseTree(struct _tn * t);
void printParseTree(struct _tn * t);