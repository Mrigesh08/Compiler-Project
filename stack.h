// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include <stdlib.h>

typedef struct _s{
	struct _snode * first;
	int length;
}Stack;


typedef struct _snode{
	char * str;
	struct _snode *next;
}SNode;


Stack * createStack(){
	Stack * s=(Stack *)malloc(sizeof(Stack));
	s->first=NULL;
	s->length=0;
	return s;
}

SNode * newStackNode(char * str){
	SNode * n=(SNode *)malloc(sizeof(SNode));
	n->next=NULL;
	n->str=str;
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


SNode * r1(){

	char str1[]="ASSIGNOP";
	char str2[]="FUNID";
	char str3[]="SQO";
	char str4[]="<7>";
	char str5[]="SQC";
	char str6[]="<2>";
	char str7[]="END";
	char str8[]="SEMICOLON";
	
	char * x=(char *)malloc(sizeof(str8)+1);
	strcpy(x,str8);
	SNode * n1=newStackNode(str1);
	SNode * n2=newStackNode(str2);
	SNode * n3=newStackNode(str3);
	SNode * n4=newStackNode(str4);
	SNode * n5=newStackNode(str5);
	SNode * n6=newStackNode(str6);
	SNode * n7=newStackNode(str7);
	SNode * n8=newStackNode(x);
	
	n1->next=n2;
	n2->next=n3;
	n3->next=n4;
	n4->next=n5;
	n5->next=n6;
	n6->next=n7;
	n7->next=n8;
	
	return n1;
}

SNode * r2(){
	
	char str1[]="OP";
	char str2[]="<27>";
	char str3[]="CL";
	char str4[]="<29>";
	char str5[]="OP";
	char str6[]="<27>";
	char str7[]="CL";

	SNode * n1=newStackNode(str1);
	SNode * n2=newStackNode(str2);
	SNode * n3=newStackNode(str3);
	SNode * n4=newStackNode(str4);
	SNode * n5=newStackNode(str5);
	SNode * n6=newStackNode(str6);
	SNode * n7=newStackNode(str7);
	
	n1->next=n2;
	n2->next=n3;
	n3->next=n4;
	n4->next=n5;
	n5->next=n6;
	n6->next=n7;
	
	return n1;

}

SNode * r3(){
	char str1[]="<14>";
	char str2[]="SEMICOLON";

	SNode * n1=newStackNode(str1);
	SNode * n2=newStackNode(str2);
	
	n1->next=n2;
	
	return n1;

}

