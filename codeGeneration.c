#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// remember to open file "f" for appending


void generateCode(File * f, TreeNode * ast){
	// Remember, first check to see if all the values have been initialized properly
	TreeNode * temp=ast->down;
	while(temp!=NULL){
		if(strcmp(temp->str,"READ")==0){

		}
		else if(strcmp(temp->str,"PRINT")==0){

		}
		temp=temp->next;
	}
}

void writeDataSection(File * f){
	fprintf(f, "%cinclude 'functions.asm'\nSECTION .data\n",'%' );
}
void writeStartSection(File * f){
	fprintf(f, "global _start\n\n" );
	fprintf(f, "_start:\n");
}

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}
