// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include <stdio.h>
#include <stdlib.h>
#define alphabetSize 27 
// insert one for the end marker of the string


typedef struct _node{
	struct _node * arr[alphabetSize];
}Node;

Node * newTrieNode(){
	Node * n=(Node *)malloc(sizeof(Node));
	for(int i=0;i<alphabetSize;i++){
		n->arr[i]=NULL;
	}
	return n;
}

void insertIntoTrie(char * str,Node * n){
	int x=(int)(str[0]-'a');
	if(str[0]=='\0'){
		// printf("WRITING1\n");
		n->arr[alphabetSize-1]=newTrieNode();
	}
	else if(n->arr[x]!=NULL){
		// printf("WRITING2\n");
		insertIntoTrie(str+1,n->arr[x]);
	}
	else if(n->arr[x]==NULL){
		// printf("WRITING3\n");
		n->arr[x]=newTrieNode();
		insertIntoTrie(str+1,n->arr[x]);
	}
}

int checkPresenceInTrie(char * str, Node *n){
	// returns 1 if present, 0 otherwise.
	int x=(int)(str[0]-'a');
	if(str[0]=='\0'){
		if(n->arr[alphabetSize-1]!=NULL) return 1;
		else return 0;
	}
	else if(n->arr[x]!=NULL){
		checkPresenceInTrie(str+1,n->arr[x]);
	}
	else if(n->arr[x]==NULL){
		return 0;
	}
}

void populateTrie(Node * n){
	
	char str1[]="end";
	char str2[]="int";
	char str3[]="real";
	char str4[]="string";
	char str5[]="matrix";
	char str6[]="main";
	char str7[]="if";
	char str8[]="else";
	char str9[]="endif";
	char str10[]="print";
	char str11[]="function";
	char str12[]="read";

	insertIntoTrie(str1,n);
	insertIntoTrie(str2,n);
	insertIntoTrie(str3,n);
	insertIntoTrie(str4,n);
	insertIntoTrie(str5,n);
	insertIntoTrie(str6,n);
	insertIntoTrie(str7,n);
	insertIntoTrie(str8,n);
	insertIntoTrie(str9,n);
	insertIntoTrie(str10,n);
	insertIntoTrie(str11,n);
	insertIntoTrie(str12,n);

}

/*
int main(int argc, char const *argv[])
{
	
	Node * n=newTrieNode();
	char str[]="friend";
	char str2[]="fried";
	char str3[]="from";
	insertIntoTrie(str,n);
	insertIntoTrie(str2,n);
	// insertIntoTrie(str3,n);

	printf("%d\n", checkPresenceInTrie(str,n));
	printf("%d\n", checkPresenceInTrie(str2,n));
	printf("%d\n", checkPresenceInTrie(str3,n));
	return 0;
}
*/