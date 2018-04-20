struct _e;
typedef struct _q{
	char * op;
	struct _e * arg1;
	struct _e * arg2;
	struct _e * arg3; // this is the result field;	
	struct _q * next;
}Quad;


typedef struct _ql{
	struct _q * first;
	struct _ql * next;

}QuadList;

extern QuadList * listOfQuads;
extern QuadList * listOfBooleanQuads;

Quad * newQuad(char * op,struct _e * arg1, struct _e * arg2, struct _e * arg3);

QuadList * newQuadListElement(Quad * q);