#include <stdio.h>
#include <string.h>
#include "ntree.h"
#include "symbolTable.h"
#include "typeChecker.h"
#include "intermediateCode.h"
#include "quad.h"
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
				printf("TYPECHECKER ERROR: Redeclaration of variable %s\n",x);
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
				printf("TYPECHECKER ERROR: Redeclaration of variable %s\n",x );				
			}
			
		}
		
		list=list->next;
	}
	return e;
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
			printf("TYPECHECKER ERROR: Redeclaration of variable %s\n",name );							
		}
		plist=plist->next->next;
	}
	return entryList;
}

void processFunctionDeclaration(TreeNode * tn1, TreeNode * tn2){
	// tn1 is the treenode of symbolTable where you will insert new declaration
	// tn2 is the definition of function in the ast
	// tn2-down is the first plist. tn2-down-next-next if the second plist. 
	// tn2-down-next-next-next-down is the statements that need to be parsed
	TreeNode * plist1=tn2->down;
	Entry * e=generateEntryFromPlist(plist1,NULL);

	// check if all the output variables have been assigned a value
	TreeNode * plist2=tn2->down->next->next;
	Entry * e2=generateEntryFromPlist(plist2,e);

	// printf("E1 and E2 generated\n");
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
TreeNode * lookupFunctionName(TreeNode * t, char * name){
	if(strcmp(t->str,name)==0){
		return t;
	}
	else{
		TreeNode * temp=t->down;
		while(temp!=NULL){
			return lookupFunctionName(temp,name);
			temp=temp->next;
		}
		return NULL;

	}
}
int findSymbolType(TreeNode * t,char *x){
	// t is the symbolTable node in which this symbol was operated upon.
	// x is the name of symbol
	Entry * temp=t->nextEntry;
	while(temp!=NULL){
		if(strcmp(temp->name,x)==0){
			return temp->type;
		}
		temp=temp->nextEntry;
	}
	if(t->parent==NULL){
		return -1;
	}
	else{
		return findSymbolType(t->parent,x);
	}

}

int getTypeFromSymbolTable(char * id, TreeNode * st){
	Entry * temp=st->nextEntry;
	while(temp!=NULL){
		if(strcmp(id,temp->name)==0){
			return temp->type;
		}
		temp=temp->nextEntry;
	}
	if(st->parent==NULL){
		// printf("TYPECHECKER ERROR : couldn't find a symbol with name %s\n",id );
		return -1;
	}
	else{
		return getTypeFromSymbolTable(id,st->parent);
	}
}

Entry * getEntryFromSymbolTable(char * id, TreeNode * st){
	Entry * temp=st->nextEntry;	 
	while(temp!=NULL){
		if(strcmp(id,temp->name)==0){
			return temp;
		}
		temp=temp->nextEntry;
	}
	return NULL;
	// if(st->parent==NULL){
	// 	printf("TYPECHECKER ERROR : Cannot assign a value to an undeclared variable %s\n",id );
	// 	return NULL;
	// }
	// else{
	// 	return getEntryFromSymbolTable(id,st->parent);
	// }
}

TreeNode * getAstNodeFromSymbolTable(char * id, TreeNode * st){
	// printf("Looking for node %s\n",id );
	Entry * temp=st->nextEntry;
	while(temp!=NULL){
		if(strcmp(id,temp->name)==0){
			return temp->astNode;
		}
		temp=temp->nextEntry;
	}
	if(st->parent==NULL){
		printf("TYPECHECKER ERROR : couldn't find a symbol with name %s\n",id );
		return NULL;
	}
	else{
		return getAstNodeFromSymbolTable(id,st->parent);
	}
}

List * getInputAndOutputListOfFunction(TreeNode * t){
	// t is node of type "FDEF"
	List * l1=(List *)malloc(sizeof(List));
	l1->outputList=t->down->down;
	l1->inputList=t->down->next->next->down;
	return l1;
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
			 // map the symbol table node for all the decl stmts
			// add it to the current node
			Entry * e=processVariableDeclaration(t2,tn->nextEntry);
			if(e!=NULL){
				t2->symbolTableNode=tn;
				e->astNode=t2;
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
			
		}
		else if(strcmp(t2->str,"VARASSIGN")==0){
			// get the symbol table entry where the LHS of this has the symbol
			// map the assignment stmt to the astnode entry of the entry of that symbol table
			char * x=t2->down->token->c;
			Entry * e=getEntryFromSymbolTable(x,tn);
			if(e!=NULL){
				e->astNode=t2;
			}
			TreeNode * temp=t2;
			int k=validateArithmeticExpression(temp->down->next,tn);
			if(k==-1){
				printf("TYPECECKER ERROR : operation upon incompatible types at lineNumber %d\n",temp->down->token->lineNumber);
			}
			else{
				int a=checkType(temp->down,tn);
				if(a!=k){
					printf("TYPECHECKER ERROR : Assigning incompatible types at lineNumber %d\n",temp->down->token->lineNumber );
				}
				else if(k==4){
					ensureMatrixSize(temp->down->next,tn,temp->down->token->lineNumber);
				}
				if(a==k){
					Quad * q=newQuad(NULL,NULL,NULL,NULL);
					generateQuads(temp->down->next,tn,q);
					printQuad(q);
					// remove the first q after printing.
					// add the final q by mappping an entry to id of lhs
				}

			}
			printf("Type checkig result line number %d = %d\n",temp->down->token->lineNumber,k );
		}
		else if(strcmp(t2->str,"FDEF")==0){
			// create a new node. 
			// set parent. 
			// recursively call createSymbolTable on the newNode
			char * thing=t2->down->next->token->c;
			// printf("HELLO  %s\n",thing );
			if(lookupInFunList(thing,tn->down)==1){
				printf("TYPECHECKER ERROR.... Function '%s' redeclared at line number %d\n",thing,t2->down->down->token->lineNumber );
			}
			else{


				TreeNode * t3=createNewTreeNode(thing,NULL);
				t2->symbolTableNode=t3;
				t3->astNode=t2;
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
				ensureValuesAssigned(t2,t3);
			}
		}
		else if(strcmp(t2->str,"IFSTMT")==0){
			TreeNode * stmts=t2->down->next;
			Quad * q=newQuad(NULL,NULL,NULL,NULL);
			generateQuadsForBoolean(t2->down,tn,q);
			printQuad(q);
			createSymbolTable(stmts,tn);
			TreeNode * elsePart=t2->down->next->next;

			if(elsePart!=NULL){
				createSymbolTable(elsePart,tn);
			}

		}
		else if(strcmp(t2->str,"READ")==0){
			TreeNode * temp=t2->down;
			int k=checkType(temp,tn);
			if(k==3 || k==4){
				printf("TYPECHECKER ERROR: Cannot read matrix or string at lineNumber %d\n",temp->token->lineNumber );
			}
		}
		else if(strcmp(t2->str,"FUNASSIGN")==0){
			if(strcmp(t2->down->next->str,"FUNCALL")==0){
				TreeNode * temp = t2->down->next; // this node has the function call
				if(strcmp(tn->str,temp->down->token->c)==0){
					// recursive call check
					printf("TYPECHECKER ERROR: recursive calls are not allowed. Recursive call found at line number %d\n",temp->down->token->lineNumber );
				}
				TreeNode * fun=ensureFunctionDeclared(temp->down->token->c,tn);
				// fun is a symbol table node
				if(fun==NULL){
					printf("TYPECHECKER ERROR: Undeclared function used at line number %d\n",temp->down->token->lineNumber );
				}
				else{
					// check if the output parameters are declared and exact number is there and they are of same type. 
					// check if the input parameter are declared and exact number is there and they are of same type.
					// printf("FOUND function %s\n",fun->astNode->down->next->token->c);
					List * l=getInputAndOutputListOfFunction(fun->astNode);
					ensureCorrectFunctionCall(t2,l,tn);
				}	
			}
			else if(strcmp(t2->down->next->str,"SIZE")==0){
				checkType(t2->down->next,tn);
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