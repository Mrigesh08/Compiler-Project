#include "quad.h"
#include "symbolTable.h"
#include "ntree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int variableName=11;


Quad * newQuad(char * op,struct _e * arg1, struct _e * arg2, struct _e * arg3){
	Quad * q=(Quad * )malloc(sizeof(Quad));
	q->op=op;
	q->arg1=arg1;
	q->arg2=arg2;
	q->arg3=arg3;
	q->next=NULL;
	return q;
}

QuadList * newQuadListElement(Quad * q){
	QuadList * ql=(QuadList *)malloc(sizeof(QuadList));
	ql->first=q;
	ql->next=NULL;
	return ql;
}


char * generateVariableName(){
	char * x=(char *)malloc(sizeof(char)* 10);
	sprintf(x,"%d",variableName);
	variableName++;
	char * y=(char *)malloc(strlen(x)+2);
	y[0]='t';
	strcpy(y+1,x);
	return y;
}

void appendToQuads(Quad * ql, Quad * q){
	Quad * temp=ql;
	while(temp->next!=NULL){
		temp=temp->next;
	}
	temp->next=q;
}
Entry * generateQuads(TreeNode * ae,TreeNode * st,Quad * ql){
	// ae is the starting node of arithmetic expression
	// st is the symbolTable Tree node;
	// generated quads are appended to ql which acts as a list of quads
	if(strcmp(ae->str,"PLUS")==0 || strcmp(ae->str,"MINUS")==0 || strcmp(ae->str,"MUL")==0 || strcmp(ae->str,"DIV")==0){
		Entry * e1=generateQuads(ae->down,st,ql);
		Entry * e2=generateQuads(ae->down->next,st,ql);
		Entry * e3=createEntryNode(e1->type,generateVariableName());  // QUESTION- should these entries be added in the symbol table;
		Quad * q=newQuad(ae->str,e1, e2, e3);
		appendToQuads(ql,q);
		return e3;
	}
	else if(strcmp(ae->str,"ID")==0){
		return getEntryFromSymbolTable(ae->token->c,st);
	}
	else if(strcmp(ae->str,"MATRIX")==0){
		Entry * e1=createEntryNode(11,generateVariableName()); // type specially decided to be allowing lone matrices having no ID
		e1->astNode=ae;
		e1->nextEntry=st->nextEntry;
		st->nextEntry=e1;
		return e1;
	}
	else if(strcmp(ae->str,"MATRIXELEM")==0){
		Entry * e1=createEntryNode(12,generateVariableName());
		e1->astNode=ae;
		return e1;
	}
	else if(strcmp(ae->str,"SIZE")==0){
		Entry * e1=createEntryNode(13,generateVariableName());
		e1->astNode=ae;
		return e1;
	}
	else if(strcmp(ae->str,"NUM")==0){
		Entry * e1=createEntryNode(14,generateVariableName()); // type specially decided to be allowing lone matrices having no ID
		e1->astNode=ae;
		return e1;
	}
	else if(strcmp(ae->str,"RNUM")==0){
		Entry * e1=createEntryNode(15,generateVariableName()); // type specially decided to be allowing lone matrices having no ID
		e1->astNode=ae;
		return e1;
	}
	else if(strcmp(ae->str,"STR")==0){
		printf("STR found\n");
		Entry * e1=createEntryNode(16,generateVariableName()); // type specially decided to be allowing lone matrices having no ID
		e1->astNode=ae;
		e1->nextEntry=st->nextEntry;
		st->nextEntry=e1;
		return e1;
	}
}



Entry * generateQuadsForBoolean(TreeNode * be, TreeNode * st, Quad * ql){
	// be is the ast node containing the boolean expression
	// st is the symbol table node
	// append the quads to the list ql
	if(strcmp(be->str,"AND")==0 || strcmp(be->str,"OR")==0){
		Entry * e1=generateQuadsForBoolean(be->down,st,ql);
		Entry * e2=generateQuadsForBoolean(be->down->next,st,ql);
		Entry * e3=createEntryNode(17,generateVariableName());
		Quad * q=newQuad(be->str,e1,e2,e3);
		appendToQuads(ql,q);
		return e3;
	}
	else if(strcmp(be->str,"NOT")==0){
		Entry * e1=generateQuadsForBoolean(be->down,st,ql);
		Entry * e3=createEntryNode(17,generateVariableName());
		Quad * q=newQuad(be->str,e1,NULL,e3);
		appendToQuads(ql,q);
		return e3;
	}
	else if(strcmp(be->str,"LT")==0 ||strcmp(be->str,"LE")==0 ||strcmp(be->str,"EQ")==0 ||strcmp(be->str,"GT")==0 ||strcmp(be->str,"GE")==0 ||strcmp(be->str,"NE")==0){
		Entry * e1=generateQuadsForBoolean(be->down,st,ql);
		Entry * e2=generateQuadsForBoolean(be->down->next,st,ql);
		Entry * e3=createEntryNode(17,generateVariableName());
		Quad * q=newQuad(be->str,e1,e2,e3);
		appendToQuads(ql,q);
		return e3;
	}
	else if(strcmp(be->str,"ID")==0){
		return getEntryFromSymbolTable(be->token->c,st);
	}
	else if(strcmp(be->str,"NUM")==0){
		Entry * e1=createEntryNode(14,generateVariableName());
		e1->astNode=be;
		return e1;
	}
	else if(strcmp(be->str,"RNUM")==0){
		Entry * e1=createEntryNode(15,generateVariableName());
		e1->astNode=be;
		return e1;
	}
}

void printQuad(Quad * q){
	// q=q->next; // first quad is dummy
	while(q!=NULL){
		printf("%s = %s %s %s\n",q->arg3->name, q->arg1->name, q->op,q->arg2 == NULL ? NULL : q->arg2->name );
		q=q->next;
	}
}
QuadList * appendToListOfQuads(QuadList * ql, Quad * q){
	// printf("INSIDE APPENDING====================\n");
	if(ql==NULL){
	// printf("INSIDE APPENDING 22====================\n");
		QuadList * newq=(QuadList *)malloc(sizeof(QuadList));
		newq->first=q;
		newq->next=NULL;
		ql=newq;
		return ql;
	}
	else{
			// printf("INSIDE APPENDING 44 	====================\n");

		QuadList * temp=ql;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		QuadList * newq=(QuadList *)malloc(sizeof(QuadList));
		newq->first=q;
		newq->next=NULL;
		temp->next=newq;
		return ql;
	}
}

void printQuadList(QuadList * ql){
	QuadList * temp=ql;
	while(temp!=NULL){
		printf("PRINTING QUAD ================= \n");
		printQuad(temp->first);
		temp=temp->next;
	}
}

/*
int main(int argc, char const *argv[])
{
	return 0;
}
*/