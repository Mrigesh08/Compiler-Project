// ID  =2015A7PS0146P
// Name=Mrigesh Madaan


#include <stdio.h>
#include <string.h>
#include "ntree.h"

int atoiPlus2(char * str){
	char * x=(char *)malloc(strlen(str)-1);
	int i=1;
	while(str[i]!='>'){
		x[i-1]=str[i];
		i++;
	}
	x[i-1]='\0';
	return atoi(x);
}

void printList(TreeNode * t){
	TreeNode * t2=t;
	while(t2!=NULL){
		printf("%s ", t2->str);
		t2=t2->next;
	}
	printf("\n");
}

TreeNode * combine(TreeNode * t, TreeNode * list){
	int x=atoiPlus2(t->str);
	TreeNode * tn, * tn1, * tn2, * tn3, * prev, *tn4;
	int k;
	switch(x){
		case 1: 
			tn2=list->next->next->next;
			free(list->next->next->next->next);
			list->next->next->next->next=NULL;
			free(list->next->next);
			free(list->next);
			free(list);
			return tn2;
			break;

		case 2: 
			tn = createNewTreeNode("MAIN",NULL);
			tn->down=list;
			return tn;
			break;

		case 3: 
			if((t->down->str)[0]=='$'){ // <3> => $
				printf("DOLLAR FOUND\n");
				return NULL; // this assumes that '$' will always come in the last
			}
			else{ // <3> => <4> <3>
				return list;
			}

			break;

		case 4: 
			// if(atoiPlus2(t->down->str)==5){ // <4> => <5>
			// 	return list;
			// }
			// else{ // <4> => <6>
			// 	return list;
			// }
			return list;
			break;

		case 5:
			return list;
			break;

		case 6: // FUNCTION SQO <7> SQC ASSIGNOP FUNID SQO <7> SQC <2> END SEMICOLON
			tn=createNewTreeNode("FDEF",NULL);
			// printf("000000000000000000000000000000000000000\n");
			// printList(list);
			tn2=list;
			tn->down = tn2->next->next; // <parameterList1>
			free(list->next);
			free(list);
			tn2=tn->down;
			tn3=tn->down;
			// printList(tn->down);

			tn2=tn3->next;
			tn3->next= tn3->next->next->next; // FUNID
			// printf("%s\n",tn3->next->str );
			free(tn2->next);
			free(tn2);
			tn3=tn3->next;
			tn2=tn3;
			// printList(tn->down);

			tn2=tn3->next;
			tn3->next= tn3->next->next; // <parameterList2>
			// printf("%s\n",tn3->next->str );
			free(tn2);
			tn3=tn3->next;
			tn2=tn3;
			// printList(tn->down);

			tn2=tn3->next;
			tn3->next= tn3->next->next;
			free(tn2);
			tn3=tn3->next;
			tn2=tn3;
			free(tn2->next->next);
			free(tn2->next);
			tn3->next=NULL;
			// printf("PORITINTINIJLDSJKLAJDKLAJDKLSJKLDALDKSADJKLSJDKLSJAKLDJSLk\n");
			// printList(tn->down);
			return tn;
			break;

		case 7: 
			if(atoiPlus2(t->parent->str) == 6){
				TreeNode * tn=createNewTreeNode("PLIST",NULL);
				tn->down=list;
				return tn;
			}
			else{
				return list;
			}
			break;

		case 8: 
			return list;
			break;

		case 9: 
			if((t->down->str)[0]=='$'){
				return NULL;
			}
			else{
				tn2=list;
				list=list->next; // skip the comma
				free(tn2);
				return list; 
			}
			break;

		case 10: 
			tn=createNewTreeNode("DECL",NULL);
			tn->down=list;
			free(list->next->next);
			list->next->next=NULL;
			return tn;
			break;

		case 11: 
			return list;
			break;

		case 12: 
			tn=createNewTreeNode("VARASSIGN",NULL);
			tn->down=list;
			tn2=list->next;
			list->next=tn2->next;
			free(tn2);
			free(list->next->next);
			list->next->next=NULL;
			return tn;
			break;
		
		case 13: 
			tn=createNewTreeNode("FUNASSIGN",NULL);
			
			tn2=list;
			list = list->next;
			free(tn2);
			tn2=list->next->next->next;
			free(list->next->next);
			free(list->next);
			list->next=tn2;
			free(tn2->next);
			tn2->next=NULL;
			tn->down=list;
			return tn;

			break;

		case 14: 
			if(strcmp(t->down->str,"SIZE")==0){
				tn=createNewTreeNode("SIZE",NULL);
				tn->down=list->next;
				free(list);
				return tn;
			}
			else{
				return list;
			}
			break;

		case 15: // IF OP <27> CL <5> <18> <16>
			tn=createNewTreeNode("IFSTMT",NULL);
			tn2=list;
			list=list->next->next;
			free(tn2->next);
			free(tn2);
			tn->down=list; // boolexp
			tn2=list;
			tn3=tn2->next->next; // stmt
			free(tn2->next);
			tn2=tn3->next;
			tn3->next=tn2->down;
			tn2->down=tn3;
			tn->down->next=tn2;
			return tn;
			break;

		case 16: 
			if(strcmp(t->down->str,"ELSE")==0){ // ELSE <5> <18> ENDIF SEMICOLON
				tn=createNewTreeNode("ELSESTMT",NULL);
				tn2=list;
				list=list->next;
				free(tn2);
				free(list->next->next->next);
				free(list->next->next);
				list->next->next=NULL;
				tn2=list;
				tn3=list->next;
				tn2->next=tn3->down;
				tn3->down=tn2;
				tn->down=tn3->down;
				return tn;
			}
			else{
				free(list->next);
				free(list);
				return NULL;
			}
			break;

		case 17: 
			if(strcmp(t->down->str,"READ")==0){
				tn=createNewTreeNode("READ",NULL);
				tn->down=list->next->next;
				free(list->next->next->next->next);
				free(list->next->next->next);
				list->next->next->next=NULL;
				free(list->next);
				free(list);
				return tn;
			}
			else{
				tn=createNewTreeNode("PRINT",NULL);
				tn->down=list->next->next;
				free(list->next->next->next->next);
				free(list->next->next->next);
				list->next->next->next=NULL;
				free(list->next);
				free(list);
				return tn;
			}
			break;

		case 18: 
			k=atoiPlus2(t->parent->str);
			tn=createNewTreeNode("STMTS",NULL);
			if((t->down->str)[0]=='$'){
				if(k==15 || k==16){
					return tn;
				}
				else{
					return NULL;
				}
				return tn;
			}
			else if(k==15 || k==16){
				tn->down=list;
				return tn;
			}
			else{
				return list;
			}
			break;
		
		case 19:
			if(list->next==NULL){
				return list;
			}
			else{
				tn1=list;
				tn2=list->next;
				tn=tn2;
				while(tn2->down->down!=NULL && (strcmp(tn2->down->str,"PLUS")==0 || strcmp(tn2->down->str,"MINUS")==0)){
					tn2=tn2->down;
				}
				tn1->next=tn2->down;
				tn2->down=tn1;
				return tn;
			}
			// if(list->next==NULL){
			// 	return list;
			// }
			// tn=createNewTreeNode("AEXP",NULL);
			// tn->down = list;
			// return tn;
			break;

		case 20: 
			if((t->down->str)[0]=='$'){
				return NULL;
			}
			else{
				if(list->next->next==NULL){
					tn=createNewTreeNode(list->str,NULL);
					tn->down=list->next;
					tn2=list;
					free(tn2);
					return tn;
				}
				else{
					tn1=list;
					tn2=list->next;
					tn3=list->next->next;
					tn=tn3;
					while(tn3->down->down!=NULL && (strcmp(tn3->down->str,"PLUS")==0 || strcmp(tn3->down->str,"MINUS")==0)){
						tn3=tn3->down;
					}
					// traverse till tn->down == NULL
					tn1->down=tn2;
					tn2->next=NULL;
					tn1->next=tn3->down;
					tn3->down=tn1;
					return tn;
				}
			}
			break;
		case 21: 
			if(list->next==NULL){
				return list;
			}
			else{
				// else{
				tn1=list;
				tn2=list->next;
				tn=tn2;
				while(tn2->down->down!=NULL){
					// printf("WENT DOWN @\n");
					tn2=tn2->down;
				}
				tn1->next=tn2->down;
				tn2->down=tn1;
				return tn;
				// }
			}
			// tn= createNewTreeNode("AEXP",NULL);
			// tn->down=list;
			// return tn;
			break;
		
		case 22: 
			if((t->down->str)[0]=='$'){
				return NULL;
			}
			else{
				if(list->next->next==NULL){
					// printf("-----------------------------------\n");
					tn=createNewTreeNode(list->str,NULL);
					tn->down=list->next;
					tn2=list;
					free(tn2);
					return tn;
				}
				else{
					// printf("=================================\n");
					tn1=list;
					tn2=list->next;
					tn3=list->next->next;
					tn=tn3;
					while(tn3->down->down!=NULL){
						// printf("went down\n");
						tn3=tn3->down;
					}
					// traverse till tn->down == NULL
					tn1->down=tn2;
					tn2->next=NULL;
					tn1->next=tn3->down;
					tn3->down=tn1;
					return tn;
				}
			}
			break;
		
		case 23: 
			if(strcmp(t->down->str,"OP")==0){
				free(list->next->next);
				tn=list->next; // Arithmetic Expression
				free(list); 
				tn->next=NULL;
				return tn;
			}
			else{
				return list; // var
			}
			break;
		
		case 24: 
			return list;
			break;
		
		case 25: 
			return list;
			break;
		
		case 26: 
			// tn=createNewTreeNode("VAR",NULL);
			if(strcmp(t->down->str,"ID")==0){
				// create matrixElemet
				if(list->next==NULL){
					// there is no matrix elem
					// tn->down=list;
					return list;
				}
				tn2=createNewTreeNode("MATRIXELEM",NULL);
				tn2->down=list;
				// tn->down=tn2;
				return tn2;
			}
			else if(strcmp(t->down->str,"SQO")==0){
				// create matrix // str="MATRIX"
				// tn->down=list;
				return list;
			}
			else if(strcmp(t->down->str,"FUNID")==0){
				// create funCall stmt // str="FUNCALL"
				// tn->down=list;
				return list;
			}
			else if(strcmp(t->down->str,"SIZE")==0){
				tn2=createNewTreeNode("SIZE",NULL);
				tn2->down=list->next;
				free(list);
				// tn->down=tn2;
				return tn2;
			}
			else{
				// tn->down=list;
				return list;
			}
			break;
		
		case 27: //OP <27> CL <29> OP <27> CL | NOT OP <27> CL | <40> <30> <40>
			if(strcmp(t->down->str,"OP")==0){
				
				tn2=list;
				list=list->next;
				free(tn2); // freeing OP

				tn2=list->next;
				list->next=list->next->next;
				free(tn2); // freeing CL

				tn=createNewTreeNode(list->next->str,NULL);

				tn2=list->next->next;
				list->next=list->next->next->next;
				free(tn2); // freeing OP
				free(list->next->next); // freeing CL
				list->next->next=NULL;
				tn->down=list;
				return tn;
			}
			else if(strcmp(t->down->str,"NOT")==0){
				tn=createNewTreeNode("NOT",NULL);
				tn->down=list->next->next;
				free(list->next->next->next);
				list->next->next->next=NULL;
				free(list->next);
				free(list);
				return tn;
			}
			else{
				tn=createNewTreeNode(list->next->str,NULL);
				tn2=list->next;
				list->next=list->next->next;
				free(tn2); // freeing relational op
				tn->down=list;
				return tn;
			}
			break;
		
		case 28: 
			if((t->down->str)[0]=='$'){
				return NULL;
			}
			else{
				tn2=list;
				list=list->next;
				free(tn2);
				tn2=list->next;
				list->next=list->next->next;
				free(tn2);
				free(list->next->next);
				list->next->next=NULL;
				return list;	
			}
			
			break;
		
		case 29: 
			return list;
			break;
		
		case 30: 
			return list;
			break;
		
		case 31: 
			k=atoiPlus2(t->parent->str);
			if(k==10 || k==13){
				tn=createNewTreeNode("VARLIST",NULL);
				tn->down=list;
				return tn;
			}
			else{
				return list;
			}		
			break;
		
		case 32: 
			if((t->down->str)[0]=='$'){
				return NULL;
			}
			else{
				tn2=list;
				list=list->next;
				free(tn2);
				return list;
			}
			break;
		
		case 33: 
			tn2=list;
			list=list->next;
			free(tn2);
			free(list->next);
			list->next=NULL;
			return list;
			break;
		
		case 34: 
			if(atoiPlus2(t->parent->str)==33){
				tn=createNewTreeNode("MATRIX",NULL);
				tn3=list;
				while(tn3->next!=NULL && strcmp(tn3->next->str,"INTEGERLIST")!=0){
					tn3=tn3->next;
				}
				tn2=createNewTreeNode("INTEGERLIST",NULL);
				tn2->down=list;
				tn2->next=tn3->next;
				tn3->next=NULL;
				tn->down=tn2;
				return tn;
			}
			else{
				// traverse till you find NULL or integerlist;
				tn3=list;
				while(tn3->next!=NULL && strcmp(tn3->next->str,"INTEGERLIST")!=0){
					tn3=tn3->next;
				}
				tn=createNewTreeNode("INTEGERLIST",NULL);
				tn->down=list;
				tn->next=tn3->next;
				tn3->next=NULL;
				return tn;
			}
			break;
		
		case 35: 
			if(strcmp(t->down->str,"COMMA")==0){
				tn2=list;
				list=list->next;
				free(tn2);
				return list;
			}
			else if(strcmp(t->down->str,"SEMICOLON")==0){
				tn=list->next;
				free(list);
				return tn;
			}
			else{
				return NULL;
			}
			break;
		
		case 36: 
			free(list->next);
			list->next=NULL;
			return list; 
			break;
		
		case 37: 
			tn=createNewTreeNode("FUNCALL",NULL);
			free(list->next->next->next);
			list->next->next->next=NULL;
			tn2=list->next;
			list->next=list->next->next;
			free(tn2);
			tn->down=list;
			return tn;
			break;
		
		case 38: 
			k=atoiPlus2(t->parent->str);
			if(k==37){
				tn=createNewTreeNode("ARGLIST",NULL);
				tn->down=list;
				return tn;
			}
			else{
				return list;
			}
			break;
		
		case 39: 
			if((t->down->str)[0]=='$'){
				return NULL;
			}
			else{
				tn2=list;
				list=list->next;
				free(tn2);
				return list;
			}
			break;
		
		case 40: 
			// tn=createNewTreeNode("VAR",NULL);
			// tn->down=list;
			return list;
			break;
		
		case 41:
			tn=createNewTreeNode("STMTS",NULL);
			tn->down=list;
			return tn;
			break;	
	}
}

TreeNode * visit(TreeNode * t, TreeNode * list){
	if((t->str)[0]=='<'){
		// it is a non-terminal
		return combine(t,list);
	}
}



TreeNode * postOrderTraversal(TreeNode * t){
	if(t->down!=NULL){
		TreeNode * t2 = t->down;
		TreeNode * list=postOrderTraversal(t2);
		TreeNode * list2=list;
		t2=t2->next;
		while(t2!=NULL){
			TreeNode * x=postOrderTraversal(t2);
			list2->next=x;
			list2=list2->next;
			t2=t2->next;
		}
		printf("PRINTING List for %s\n",t->str );
		printList(list);
		return visit(t,list);
	}
	else{
		// TreeNode * t2= t;
		// while(t2!=NULL){
		// 	visit(t2);
		// 	t2=t2->next;
		// }
		if((t->str)[0]=='$'){
			return NULL;
		}
		return t;
	}
}

TreeNode * createAst(TreeNode * t){
	// traverse the tree in postorder traversal. 
	// create a linked list of "TreeNodes" that need to be made
	// join those tree nodes when necessary. 
	// return the created Tree
	return postOrderTraversal(t);
}

/*
int main(int argc, char const *argv[])
{
	TreeNode * t=parse();
	TreeNode * ast=createAst(t);	

	return 0;
}
*/