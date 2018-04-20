#include "stack.h"

Stack * createStack(){
	Stack * s=(Stack *)malloc(sizeof(Stack));
	s->first=NULL;
	s->length=0;
	return s;
}

SNode * newStackNode(char * str){
	SNode * n=(SNode *)malloc(sizeof(SNode));
	n->next=NULL;
	if(str==NULL){
		n->str=str;
	}
	else{
		char * c=(char *)malloc(strlen(str)+1);
		strcpy(c,str);
		n->str=c;	
	}
	
	return n;
}

void push(Stack * s,char * str){
	SNode * n=newStackNode(str);
	n->next=s->first;
	s->first=n;
	s->length=s->length+1;
}

SNode * pop(Stack * s){
	if(s->length==0) return NULL;
	SNode * n=s->first;
	s->first=n->next;
	s->length=s->length-1;
	return n;
}

char * top(Stack * s){
	return s->first->str;
}
