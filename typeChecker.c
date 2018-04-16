#include <stdio.h>
#include <stdlib.h>
#include "ntree.h"
#include "symbolTable.h"
#include "typeChecker.h"

/*
	Global types:
	INT : 1
	REAL : 2
	STRING : 3
	MATRIX : 4
*/

int checkType(TreeNode * t, TreeNode * st,char * scopingFunction){
	// t is treenode of type "var"
	// st is the symbol table
	// scopingFunction is name of function in which this assigment statement written
	if(strcmp(t->str,"MATRIXELEM")==0){
		return 1;
	}
	else if(strcmp(t->str,"MATRIX")==0){
		return 4;
	}
	else if(strcmp(t->str,"FUNCALL")==0){
		TreeNode * temp=t->down->down;
		if(temp->next->next==NULL){
			if(strcmp(temp->str,"INT")==0){
				return 1;
			}
			else if(strcmp(temp->str,"REAL")==0){
				return 2;
			}
			else if(strcmp(temp->str,"STRING")==0){
				return 3;
			}
			else if(strcmp(temp->str,"MATRIX")==0){
				return 4;
			}
			else{
				// highly unlikely code will end up here
				return -1;
			}
		}
		else{
			return -1;
		}
	}
	else if(strcmp(t->str,"SIZE")==0){
		char * name=t->down->token->c;
		// printf("FOUND FOR SIZE %s\n",name );
		int k=getTypeFromSymbolTable(name,st,scopingFunction);
		if(k==4){
			return -1;
		}
		else if(k==3){
			return 1;
		}
		else if(k==-1){
			printf("TYPECHECKER : ERROR... Variable undeclared %s at line number %d\n",name,t->down->token->lineNumber );
			return -1;
		}
		else{
			printf("TYPECHECKER : ERROR...  cannot calculate size of %s\n",name );
		}
	}
	else if(strcmp(t->str,"ID")==0){
		// printf("TYPECHECKER : checking for %s\n",t->token->c );
		int k=getTypeFromSymbolTable(t->token->c,st,scopingFunction);
		if(k==-1){
			printf("TYPECHECKER : ERROR1... Variable undeclared %s at line number %d\n",t->token->c,t->token->lineNumber );			
			return -1;
		}
		// printf("return value %d\n",k );
		return k;

	}
	else if(strcmp(t->str,"STR")==0){
		return 3;
	}
	else if(strcmp(t->str,"NUM")==0){
		return 1;
	}
	else if(strcmp(t->str,"RNUM")==0){
		return 2;
	}

}
void printTree(TreeNode * tree){
	TreeNode * temp=tree;
	if(temp->down==NULL){
		printf("Reached %s\n",temp->token->c );
	}
	else{
		printf("%s\n", tree->str);
		temp=temp->down;
		while(temp!=NULL){
			printTree(temp);
			temp=temp->next;
		}
	}
}
int validateArithmeticExpression(TreeNode * t, TreeNode * st, char * scopingFunction){
	// t is a RHS of VARASSGN
	// st is the symbol table
	// scopingFunction is the name of the scoping function in which the varible is present
	// call recursively for statements defined inside a function.
	// printTree(t);
	int k1,k2;
	if(strcmp(t->str,"PLUS")==0){
		k1=validateArithmeticExpression(t->down,st,scopingFunction);
		k2=validateArithmeticExpression(t->down->next,st,scopingFunction);
		if(k1==1 && k2==1){
			return 1;
		}
		else if(k1==2 && k2==2){
			return 2;
		}
		else if(k1==3 && k2==3){
			return 3;
		}
		else if(k1==4 && k2==4){
			return 4;
		}
		else{
			return -1;
		}
	}
	else if(strcmp(t->str,"MINUS")==0){
		k1=validateArithmeticExpression(t->down,st,scopingFunction);
		k2=validateArithmeticExpression(t->down->next,st,scopingFunction);

		if(k1==1 && k2==1){
			return 1;
		}
		else if(k1==2 && k2==2){
			return 2;
		}
		else if(k1==4 && k2==4){
			return 4;
		}
		else{
			return -1;
		}
	}
	else if(strcmp(t->str,"MUL")==0){
		k1=validateArithmeticExpression(t->down,st,scopingFunction);
		k2=validateArithmeticExpression(t->down->next,st,scopingFunction);
		if(k1==1 && k2==1){
			return 1;
		}
		else if(k1==2 && k2==2){
			return 2;
		}
		else{
			return -1;
		}
	}
	else if(strcmp(t->str,"DIV")==0){
		k1=validateArithmeticExpression(t->down,st,scopingFunction);
		k2=validateArithmeticExpression(t->down->next,st,scopingFunction);
		if((k1==1 && k2==1 )||(k1==2 && k2==2)){
			return 2;
		}
		else{
			return -1;
		}
	}
	else{
		return checkType(t,st,scopingFunction);
	}
}
int checkBooleanExpression(TreeNode * t, TreeNode * st, char * scopingFunction){
	char * x=t->str;
	int k1, k2;
	// if(strcmp(x,"OR")==0){
	// 	// k1=checkType(t->down);
	// 	// k2=checkType(t->down->next);
	// }
	if(strcmp(x,"AND")==0 || strcmp(x,"OR")==0){
		k1=checkBooleanExpression(t->down,st,scopingFunction);
		k2=checkBooleanExpression(t->down->next,st,scopingFunction);	
		if(k1==-1 || k2==-1){
			return -1;
		}
		else{
			return 1;
		}
	}
	else if(strcmp(x,"NOT")==0){
		return checkBooleanExpression(t->down,st,scopingFunction);
	}
	else if(strcmp(x,"LT")==0 ||strcmp(x,"LE")==0 ||strcmp(x,"EQ")==0 ||strcmp(x,"GT")==0 ||strcmp(x,"GE")==0 ||strcmp(x,"NE")==0){
		char * a=t->down->str;
		char * b=t->down->next->str;
		k1=checkType(t->down,st,scopingFunction);
		k2=checkType(t->down->next,st,scopingFunction);

		if(k1==3 || k2==3 || k1==4 || k2==4 ){
			return -1;
		}
		else if(k1==-1 || k2==-1){
			return -1;
		}
		else{
			return 1;
		}
	}
	else{
		return -1;
	}

}
/*
int main(int argc, char const *argv[])
{
	return 0;
}
*/