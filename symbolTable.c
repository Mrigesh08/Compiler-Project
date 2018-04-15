#include <stdio.h>
#include <string.h>
#include "ntree.h"
/*
#define numberOfVars 50
typedef struct _e{
	char * name;
	int type;
	int offset;
	int size;
	int width;
	struct _e * nextEntry;
}Entry;
*/


int getSize(int type){
	return (type==1)? 2 : (type==2)?4 :0;   // returning 0 for string and matrix 
}

int getType(char * x){
	if(strcmp(x,"INT")==0){
		return 1;
	}
	else if(strcmp(x,"REAL")==0){
		return 2;
	}
	else if(strcmp(x,"STRING")==0){
		return 3;
	}
	else if(strcmp(x,"MATRIX")==0){
		return 4;
	}
}

int lookupInList(char * name, Entry * list){
	Entry * e=list;
	while(e!=NULL){
		if(strcmp(e->name,name)==0){
			return 1;
		}
		e=e->nextEntry;
	}
	return 0;
}

Entry * createEntryNode(int type,char * s){
	Entry * e=(Entry *)malloc(sizeof(Entry));;
	e->name = s;
	e->type=type;
	e->offset=0;
	e->size=getSize(type);
	e->width=e->size;
	e->nextEntry=NULL;
	return e;
}

Entry * processVariableDeclaration(TreeNode * t, Entry * aie){
	Entry * e=NULL;
	Entry * e2, * e3;
	int type=getType(t->down->str);
	TreeNode * list=t->down->next->down;
	while(list!=NULL){
		char * x=list->token->c;
		if(e==NULL){

			if(lookupInList(x,aie)==0){
				e =createEntryNode(type,x);
				printf("SYMBOL TABLE : added variable %s \n",x );
				e2=e;	
			}
			else{
				printf("ERROR.... Redeclaration of variable %s\n",x);
			}
			
		}
		else{
			if(lookupInList(x,aie)==0){
				e3=createEntryNode(type,x);
				e2->nextEntry=e3;
				e2=e3;	
				printf("SYMBOL TABLE : added variable %s \n",x );
			}
			else{
				printf("ERROR.... Redeclaration of variable %s\n",x );				
			}
			
		}
		
		list=list->next;
	}
	return e; // NULL will never be returned at the very first declaration
}

Entry * generateEntryFromPlist(TreeNode * list1, Entry * list2){
	TreeNode * plist=list1->down;

	Entry * entryList=list2;
	Entry * temp,*x;
	if(entryList!=NULL){
		temp=entryList;
		while(temp->nextEntry!=NULL){
			temp=temp->nextEntry;
		}
	}
	
	int k,size;
	while(plist!=NULL){
		char * type=plist->str;
		char * name=plist->next->token->c;

		if(lookupInList(name,entryList)==0){
			k=getType(type);
			x=createEntryNode(k,name);
			printf("SYMBOL TABLE: added function variable %s\n",name );
			if(entryList==NULL){
				entryList=x;
				temp=entryList;
			}
			else{
				temp->nextEntry=x;
				temp=temp->nextEntry;
			}
		}
		else{
			printf("ERROR.... Redeclaration of variable %s\n",name );							
		}
		plist=plist->next->next;
	}
	return entryList;
}

void processFunctionDeclaration(TreeNode * tn1, TreeNode * tn2){
	// tn1 is the treenode where you will insert new declaration
	// tn2 is the definition of function in the ast
	// tn2-down is the first plist. tn2-down-next-next if the second plist. 
	// tn2-down-next-next-next-down is the statements that need to be parsed
	TreeNode * plist1=tn2->down;
	Entry * e=generateEntryFromPlist(plist1,NULL);

	TreeNode * plist2=tn2->down->next->next;
	Entry * e2=generateEntryFromPlist(plist2,e);

	printf("E1 and E2 generated\n");
	// Entry * temp=e;
	// while(temp->nextEntry!=NULL){
	// 	printf(" Traverserd %s",temp->name);
	// 	temp=temp->nextEntry;
	// }
	// temp->nextEntry=e2;

	tn1->nextEntry=e;

}

int lookupInFunList(char * name, TreeNode * tree){
	TreeNode * temp=tree;
	while(temp!=NULL){
		if(strcmp(name,temp->str)==0){
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

void createSymbolTable(TreeNode * t,  TreeNode * tn){
	// t is the abstract syntax treenode
	// tn is the SymbolTableTree node in which we have to insert the symbols. 
	// each tn represent a function starting with main
	TreeNode * t2=t->down;
	TreeNode * tempTreeNode=NULL;
	// you are looking for DECL nodes and FDEF nodes
	while(t2!=NULL){
		if(strcmp(t2->str,"DECL")==0){
			// add it to the current node
			Entry * e=processVariableDeclaration(t2,tn->nextEntry);
			if(tn->nextEntry==NULL){
				tn->nextEntry=e;
			}
			else{
				// traverse till the end and attach
				if(e!=NULL){
					Entry * temp=e;
					while(temp->nextEntry!=NULL){
						temp=temp->nextEntry;
					}
					temp->nextEntry=tn->nextEntry;
					tn->nextEntry=e;
				}
			}
		}
		else if(strcmp(t2->str,"FDEF")==0){
			// create a new node. 
			// set parent. 
			// recursively call createSymbolTable on the newNode
			char * thing=t2->down->next->token->c;
			printf("HELLO  %s\n",thing );
			if(lookupInFunList(thing,tn->down)==1){
				printf("ERROR.... Function '%s' redeclared.\n",thing );
			}
			else{


				TreeNode * t3=createNewTreeNode(thing,NULL);
				if(tempTreeNode==NULL){
					tn->down=t3;
					t3->parent=tn;
					tempTreeNode=t3;
				}
				else{
					t3->parent=tn;
					tempTreeNode->next=t3;
					tempTreeNode=t3;
				}
				processFunctionDeclaration(t3,t2);
				TreeNode * main2=t2->down->next->next->next;
				createSymbolTable(main2,t3); 
			}
		}
		t2=t2->next;
	}
}	

void printSymbolTable(TreeNode * t){
	printf("SCOPING FUNCTION = %s\n\t",t->str );
	Entry * list=t->nextEntry;
	while(list!=NULL){
		printf("%s,%d ",list->name,list->type );
		list=list->nextEntry;
	}
	printf("\n");
	TreeNode * temp=t->down;
	while(temp!=NULL){
		printSymbolTable(temp);
		temp=temp->next;
	}
}

/*
int main(int argc, char const *argv[])
{
	root=createNewTreeNode("ROOT",NULL);
	t=createSymbolTable(t,root);
	return 0;
}
*/