struct _tn;

void writeFileForOp(FILE * f, char * op, char * result);
void processQuads(FILE * f,Quad * q,struct _tn * st);

void generateCode(FILE * f, struct _tn * ast, struct _tn * st);
void allocateSpaceForDeclarations(FILE * f, struct _tn * ast, struct _tn * st);

void writeDataSection(FILE * f);
void writeStartSection(FILE * f);
void writeQuit(FILE * f);