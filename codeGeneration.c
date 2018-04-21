#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntree.h"
#include "symbolTable.h"
#include "quad.h" // required for list of quads
#include "intermediateCode.h"
// remember to open file "f" for appending

void writeFileForOp(FILE * f, char * op, char * result){
	if(strcmp(op,"PLUS")==0){
		printf("HERE 5\n");
		fprintf(f, "\tadd ax,bx\n" );
	}
	else if(strcmp(op,"MINUS")==0){
		printf("HERE 6\n");
		fprintf(f, "\tsub ax,bx\n" );
	}
	else if(strcmp(op,"MUL")==0){
		printf("HERE 7\n");
		fprintf(f, "\timul bx\n");
	}
	else if(strcmp(op,"DIV")==0){
		// don't do anything.
		// only integer arithmetic supported as of now.
	}
}
void processQuads(FILE * f,Quad * q,TreeNode * st){
	Quad * temp=q;
	while(temp!=NULL){
		
		if(strcmp(temp->op,"ASSIGNOP")==0){

			// printf("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
			Entry * e=temp->arg1;
			if(e->type==14){
				char * arg3=temp->arg3->name;
				fprintf(f, "\tmov ebx,%s\n",arg3 );
				fprintf(f, "\tmov ax,%d\n",e->astNode->token->i );
				fprintf(f, "\tmov [ebx],ax\n\n");
			}
			else{
				char * arg3=temp->arg3->name;
				fprintf(f, "\tpop ax\n" );
				fprintf(f, "\tmov ebx, %s\n",arg3 );
				fprintf(f, "\tmov [ebx],ax\n" );
			}
			fprintf(f, "\n" );
		}
		else{
			char * arg1=temp->arg1->name;
			char * arg2=temp->arg2->name;
			char * arg3=temp->arg3->name;
			Entry * e1=getEntryFromSymbolTable(arg1,st);
			if(e1==NULL){
				printf("HERE 1\n");
				fprintf(f, "\tpop ax\n");
				// TreeNode * astNode=temp->arg1->astNode;
				// if(strcmp(astNode->str,"NUM")==0){
				// 	fprintf(f, "\tpop eax\n");	
				// }
				// more statements to follow - for size, matrix etc
			}
			else{
				printf("HERE 2\n");
				// assuming that value would have been assigned to this already.
				TreeNode * astNode=e1->astNode;
				fprintf(f, "\tmov ebx, %s\n",astNode->down->token->c );
				fprintf(f, "\tmov ax, [ebx]\n");
			}

			Entry * e2=getEntryFromSymbolTable(arg2,st);
			if(e2==NULL){
				printf("HERE 3\n");
				fprintf(f, "\tpop bx\n");
				// TreeNode * astNode=temp->arg2->astNode;
				// if(strcmp(astNode->str,"NUM")==0){
				// 		fprintf(f, "\tpop ebx\n");
				// }
				// more statements to follow - for size, matrix etc
			}
			else{
				printf("HERE 4\n");
				TreeNode * astNode=e2->astNode;
				fprintf(f, "\tmov ebx, %s\n",astNode->down->token->c );
				fprintf(f, "\tmov bx, [ebx]\n");
			}
			writeFileForOp(f,temp->op,arg3);	

			Entry * e3=getEntryFromSymbolTable(arg3,st);
			if(e3==NULL){
				printf("HERE 11\n");
				fprintf(f, "\tpush ax\n" );
			}
			else{
				printf("Can't be happening\n");	
			}

			fprintf(f, "\n" );
		}
		
		temp=temp->next;
	}
}

void generateCode(FILE * f, TreeNode * ast, TreeNode * st){
	// Remember, first check to see if all the values have been initialized properly
	TreeNode * temp=ast->down;
	while(temp!=NULL){
		if(strcmp(temp->str,"VARASSIGN")==0){
			Quad * q=listOfQuads->first;
			listOfQuads=listOfQuads->next;
			processQuads(f,q,st);
		}
		else if(strcmp(temp->str,"READ")==0){
			fprintf(f, "\tmov eax, msg111\n");
			fprintf(f, "\tcall fun_printLineFeed\n\n");
			fprintf(f, "\tmov eax, %s\n",temp->down->token->c );
			fprintf(f, "\tcall fun_readString\n\n");

		}
		else if(strcmp(temp->str,"PRINT")==0){
			char * id=temp->down->token->c;
			int k=getTypeFromSymbolTable(id,st);
			if(k==1){
				// for ints
				fprintf(f, "\tmov eax,0\n");
				fprintf(f, "\tmov ebx,%s\n",id );
				fprintf(f, "\tmov ax,[ebx]\n");
				fprintf(f, "\tcall fun_iPrintLF\n\n" );
			}
			else if(k==3){
				// for strings
				fprintf(f, "\tmov eax,%s\n",id );
				fprintf(f, "\tcall fun_sprint\n\n");

			}
			
		}
		temp=temp->next;
	}
}

void allocateSpaceForDeclarations(FILE * f, TreeNode * ast, TreeNode * st){
	// fprintf(f, "SECTION .bss\n\n");
	TreeNode * temp=ast->down;
	while(temp!=NULL){
		if(strcmp(temp->str,"DECL")==0){
			printf("FOUND DECLARATION\n");
			TreeNode * t=temp->down->next->down; 
			while(t!=NULL){
				int k=getTypeFromSymbolTable(t->token->c, st);
				if(k==1){
					fprintf(f, "%s: RESW 1\n",t->token->c );
				}
				else if(k==2){
					fprintf(f, "%s: RESW 2\n",t->token->c );
				}
				else if(k==3){
					fprintf(f, "%s: RESB 20\n",t->token->c );
				}
				else if(k==4){
					fprintf(f, "%s: RESW 100\n",t->token->c );
				}
				t=t->next;
			}
		}
		else if(strcmp(temp->str,"IFSTMT")==0){
			allocateSpaceForDeclarations(f,temp->down->next,st);
		}
		temp=temp->next;
	} 
}

void writeDataSection(FILE * f){
	fprintf(f, "%cinclude 'functions.asm'\n\nSECTION .data\n",'%' );
	fprintf(f, "msg111 db 'Waiting For input: ', 0h\n");
	fprintf(f, "\n" );
}

void writeStartSection(FILE * f){
	fprintf(f, "SECTION .text\n");
	fprintf(f, "global _start\n\n" );
	fprintf(f, "_start:\n");
}
void writeQuit(FILE * f){
	fprintf(f, "\tcall fun_exit\n" );
}

/*
int main(int argc, char const *argv[])
{
	return 0;
}
*/