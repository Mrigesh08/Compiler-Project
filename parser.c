// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include <setjmp.h>
#include "grammarRead.h"
#include "stack.h"
#include "ntree.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeChecker.h"

// #include "ast.h" // this file includes lexer.h, ntree.h


Stack * stack;
Head * g;
TreeNode * parseTree=NULL;
int error_flag2=0;
jmp_buf place;
FILE * f;
TreeNode * ast;
TreeNode * st;

int atoiPlus(char * str){
	printf("VALUE RECEIVED BY AOITPLUS %s and size is %d\n", str,strlen(str));
	char * x=(char *)malloc(strlen(str)-1); // -2+1
	int i=1;
	while(str[i]!='>'){
		x[i-1]=str[i];
		i++;
	}
	x[i-1]='\0';
	return atoi(x);
}

void errorRecovery(Token * tk){
	// printf("ERROR RECOVERY : Looking for token %s\n",tk->tokenName );
	while(1){
		if(stack->length==0){
			longjmp(place,1);
		}
		char * n=top(stack);
		// printf("ERROR RECOVERY : Stack top found to be %s. ",n );
		if(n[0]=='<'){
			int k=atoiPlus(n);
			Node * x=g[k].followSets;
			int flag=0;
			while(x!=NULL){
				if(strcmp(x->str,tk->tokenName)==0){
					// printf("Matched!\n");
					flag=1;
					// pop(stack);
					break;
				}
				x=x->next;
			}
			if(flag==1){
				break;
			}
			pop(stack);
			// if(pop(stack) ==NULL){ printf("INSIDE");longjmp(place,1); break;}
			// printf("Popped\n");
		}
		else{
			if(strcmp(tk->tokenName,n)==0){
				// pop(stack);
				// printf("Matched!\n");
				break;
			}
			pop(stack);
			// if(pop(stack) ==NULL){printf("INSIDE 2");longjmp(place,1); break;}
			// printf("Popped\n");
		}
	}
}

/*
void strtok2(char * str,SNode * n, int k){	
	// in i is the starting point
	int i=0;
	while(str[i]!=' ' &&  i<k){
		i++;
	}
	// str[i]='\0';
	char * x=(char *)malloc(strlen(str)+1);
	
	int j=0;
	while(j<i){
		x[j]=str[j];
		j++;
	}
	x[j]='\0';

	char re[]="SEMICOL!";
	char re2[]="ASS!";
	char re3[]="SEMICOLON!";
	// printf("Generated new string - %s\n",x);
	
	
	SNode * n2;
	
	if(strcmp(x,"<1!")==0){
		n2=r3();
	}
	else if(strcmp(x,"!")==0){
		n2=newStackNode("CL");
	}
	else if(strcmp(x,"<!")==0){
		n2=newStackNode("<16>");
	}
	else if(strcmp(x,re)==0 || strcmp(x,re3)==0 || strcmp(x,"SEMI!")==0){
		n2=newStackNode("SEMICOLON");
	}

	else if(strcmp(x,re2)==0){
		n2=r1();
	}
	else{
		n2=newStackNode(x);
	}
	n->next=n2;
	if(i<k){
		strtok2(str+i+1,n2,k-i);
	}
	
}
/*
TreeNode * insertIntoTreeAndReversePush(char * str, TreeNode * tn){
	// printf("reversing and inserting\n");
	Stack * s=createStack();
	// printf("1. LOLOL \n");
	char * str2=(char *)malloc(strlen(str)+1);
	strcpy(str2,str);
	// printf("Length of string %ld\n",strlen(str2) );
	// printf("Copied string %s\n",str2 );
	SNode * n8=newStackNode(NULL);
	strtok2(str2,n8,strlen(str2));
	n8=n8->next;
	// printf("2. LOLOL \n");
	// char * x=(char *)malloc(sizeof(tk)+1);
	// strcpy(x,tk);
	
	push(s,n8->str);
	// printf("pushed into temporary stack %s\n",n8->str );
	
	TreeNode * tn2=createNewTreeNode(n8->str,NULL);
	// printf("pushed into tree %s\n",tn2->str );


	// printf("created\n"); 
	tn->down=tn2;
	// printf("step2\n");
	tn2->parent=tn;
	// printf("Inserted treenode downwards\n");
	// tk=strtok(NULL," ");
	// printf("Inserted %s into tree with parent=%s\n",tn->down->str,tn2->parent->str );
	n8=n8->next;
	while(n8!=NULL){
		// printf("Reversing Token =%s\n",n8->str );
		// x=(char *)malloc(sizeof(tk)+1);
		// strcpy(x,tk);
		
		push(s,n8->str);
		if(strcmp(n8->str,"$")==0){
			// printf("$ inserted\n");
		}
		TreeNode * tn3;
		if(strcmp(n8->str,"ASSIGNOP")==0){
			tn3=createNewTreeNode("ASSIGNOP",NULL);
		}
		else if(strcmp(n8->str,"FUNID")==0){
			tn3=createNewTreeNode("FUNID",NULL);

		}
		else if(strcmp(n8->str,"SEMICOLON")==0){
			tn3=createNewTreeNode("SEMICOLON",NULL);

		}
		else{
			tn3=createNewTreeNode(n8->str,NULL);

		}
		tn2->next=tn3;
		tn3->parent=tn2->parent;
		tn2=tn3;
		// printf("2, Inserted %s into tree with parent=%s\n",tn3->str,tn3->parent->str);
		// tk=strtok(NULL," ");
		n8=n8->next;
	}

	SNode * node=pop(s);
	// printf("popped node %s\n",node->str );
	while(node!=NULL){
		if((node->str)[0]!='$'){
			if(strcmp(node->str,"END")==0){
				push(stack,"END");
			}
			else push(stack,node->str);
			// printf("pushed node into actual stack %s\n",node->str );
			
		}
		node=pop(s);
		
	}
	// printf("returnning\n");
	return tn->down;

}
*/

TreeNode * insertIntoTreeAndReversePush(char * str, TreeNode * tn){
	// printf("reversing and inserting\n");
	Stack * s=createStack();
	// printf("1. LOLOL \n");
	char * str2=(char *)malloc(strlen(str)+1);
	strcpy(str2,str);
	// printf("Copied string %s\n",str2 );
	char * tk=strtok(str2," ");
	// printf("2. LOLOL \n");
	char * x=(char *)malloc(strlen(tk)+1);
	strcpy(x,tk);
	
	push(s,x);
	// printf("pushed into temporary stack %s\n",x );
	
	TreeNode * tn2=createNewTreeNode(x,NULL);
	// printf("created\n");
	tn->down=tn2;
	// printf("step2\n");
	tn2->parent=tn;
	// printf("Inserted treenode downwards\n");
	tk=strtok(NULL," ");

	while(tk!=NULL){
		// printf("Reversing Token =%s\n",tk );
		x=(char *)malloc(strlen(tk)+1);
		strcpy(x,tk);
		
		push(s,x);

		TreeNode * tn3=createNewTreeNode(x,NULL);
		tn2->next=tn3;
		tn3->parent=tn2->parent;
		tn2=tn3;

		tk=strtok(NULL," ");
	}

	SNode * node=pop(s);
	// printf("popped node %s\n",node->str );
	while(node!=NULL){
		if((node->str)[0]!='$'){
			push(stack,node->str);
			// printf("pushed node into actual stack %s\n",node->str );
			
		}
		node=pop(s);
		
	}
	// printf("returnning\n");
	return tn->down;

}


TreeNode * fillStackTillTerminal(int ruleNumber, int tIndex, char * str, TreeNode * tn, Token * tk){
	// stack has been popped in the calling function
	// printf("Filling Stack\n");
	char * x=ppt[ruleNumber][tIndex];
	if(x==NULL){
		// printf("Predictive parising table value was found NULL\n");
		printf("ERROR at line no. %d : Syntax Analyser : Unexpected symbol %s\n",tk->lineNumber,str );
		error_flag2=1;
		tn->token=tk;
		errorRecovery(tk);	
		x=tk->tokenName;
	}
	else{
		// printf("stack to be filled with %s\n",x);
	}
	tn = insertIntoTreeAndReversePush(x,tn); // go down one level and insert
	if(x[0]=='$'){
		// printf("adjusting dollar.\n");
		if(tn!=NULL){
			// printf("TREE PRINTING %s\n",tn->str );
			while(tn!=NULL && tn->next==NULL ){
				tn=tn->parent;
			}
			if(tn!=NULL) tn=tn->next;
			// else printf("TN WAS NULL\n");
		}
		// printf("LANDED ON %s\n", tn->str);
	}
	char * y=top(stack);
	// printf("Top of stack 2= %s and str=%s and x=%s\n",y,str,x );
	if(strcmp(y,str)!=0){
		// Either there is terminal at the top or a nonterminal;
		// if its a terminal, then its an error,
		// if its a non-terminal, then get the ruleNumber and place a recursive call
		if(y[0]=='<'){
			int k=atoiPlus(y);
			pop(stack);
			tn=fillStackTillTerminal(k,tIndex,str,tn,tk);
		}
		else{
			printf("ERROR at line no. %d : Syntax Analyser : Token expected was %s while it was found to be %s\n",tk->lineNumber,y,str );
			error_flag2=1;
			errorRecovery(tk);
			

		}
	}
	else{
		// here it depends on the tk->type.
		// convert it to str and put it here.
		SNode * snode=pop(stack);
		// printf("Terminal was same i.e. %s. Popped stack.\n",str );
		tn->str=tk->tokenName;
		tn->token=tk;

		// insertIntoTree(tk->,tn)
	}
	return tn; // terminal leaf is returnned
}


TreeNode * parse(){
	Token * tk=getNextToken();
	TreeNode * tn=createNewTreeNode("<1>",NULL);
	// printf("Inserted <1> into tree\n");
	// skipping checking for _main error;

	// printf("created tree node %s\n",tk->tokenName);
	TreeNode * tn2=tn;
	if(setjmp(place)!=0){
		// printf("returned using longjmp\n");
	}
	else{
		while(tk!=NULL){
			// printf("NEW TOKEN => %s.\n",tk->tokenName);
			// tk ==NULL signals the end has been reached
			int tIndex=getIndexOfTerminal(tk->tokenName);
			char * str=top(stack);
			// printf("Top os the stack =%s\n",str );
			if(strcmp(str,"%")==0){
				return tn2;
			}
			// printf(	"Top of the stack %s\n", str);
			if(str[0]=='<'){
				// some nonterminal is on top of the stack 
				int k=atoiPlus(str);
				// printf("atoi value %d\n",k );
				// printf("Terminal index value %d\n",tIndex );
				pop(stack);
				// printf("stack popped\n");
				tn=fillStackTillTerminal(k,tIndex,tk->tokenName,tn,tk);
				// terminal has been popped; // token for the terminal set.// terminal leaf was returned
				// printf("stack filled till terminal\n");
				// tn=tn->next;
				if(tn->next==NULL){
					tn=tn->parent;
					// printf("reached parent. down that was %s\n",tn->down->str);
					if(tn!=NULL){
						// printf("TREE PRINTING %s\n",tn->str );
						while(tn!=NULL && tn->next==NULL ){
							tn=tn->parent;
						}
						if(tn!=NULL) tn=tn->next;
					}
				}
				else{
					tn=tn->next;
				}
			}
			else{
				// some terminal is on top of the stack 
				if(strcmp(str,tk->tokenName)!=0){
					printf("ERROR at line no. %d : Syntax Analyser : Token expected was %s while it was found to be %s\n",tk->lineNumber,str,tk->tokenName );
					error_flag2=1;
					errorRecovery(tk);
					
					// break;
				}
				else{
					// terminal was found.
					SNode * yono=pop(stack);
					// printf("2. Terminal was same i.e. %s. Popped stack.\n",str );
					tn->token=tk;
					if(tn->next==NULL){
						tn=tn->parent;
						// printf("reached parent. down that was %s\n",tn->down->str);
						if(tn!=NULL){
							// printf("TREE PRINTING %s\n",tn->str );
							while(tn!=NULL && tn->next==NULL ){
								tn=tn->parent;
							}
							if(tn!=NULL) tn=tn->next;
						}
					}
					else{
						tn=tn->next;
					}
				}
			}

			tk=getNextToken();
		}
	}
	return tn2;
}
void printSimpleParseTree(TreeNode * t){
	if(t->down==NULL){
		printf("%s\n",t->str );
	}
	else{
		printSimpleParseTree(t->down);
		printf("%s\n",t->str );
		TreeNode * x=t->down;
		x=x->next;
		while(x!=NULL){
			printSimpleParseTree(x);
			x=x->next;
		}
	}
}

void printParseTree(TreeNode * t){
	
	if(t->down==NULL){
		if(t->token==NULL){
			fprintf(f,"----- \t ------ \t ---- \t ---- \t %s \t False \t %s\n",t->parent->str,t->str ); 
			// printf("----- \t ----- \t ---- \t ---- \t %s \t False \t %s\n",t->parent->str,t->str ); 

		}
		else if(t->token->type==2){
			fprintf(f,"%s \t %d \t %s \t %s \t %s \t %s \t %s\n",t->token->c,t->token->lineNumber,t->token->tokenName, t->token->c ,t->parent->str,"True",t->str );
			// printf("%s \t %d \t %s \t %s \t %s \t %s \t %s\n",t->token->c,t->token->lineNumber,t->token->tokenName, t->token->c ,t->parent->str,"True",t->str );
			
		}
		else if(t->token->type==3){
			fprintf(f,"--- \t %d \t %s \t %d \t %s \t %s \t %s\n",t->token->lineNumber,t->token->tokenName, t->token->i ,t->parent->str,"True",t->str );
			// printf("--- \t %d \t %s \t %d \t %s \t %s \t %s\n",t->token->lineNumber,t->token->tokenName, t->token->i ,t->parent->str,"True",t->str );
			
		}
		else if(t->token->type==4){
			fprintf(f,"--- \t %d \t %s \t %.2f \t %s \t %s \t %s\n",t->token->lineNumber,t->token->tokenName, t->token->f ,t->parent->str,"True",t->str );
			// printf("--- \t %d \t %s \t %.2f \t %s \t %s \t %s\n",t->token->lineNumber,t->token->tokenName, t->token->f ,t->parent->str,"True",t->str );
			
		}
		
	}
	else{
		printParseTree(t->down);
	
		if(t->parent!=NULL){	
			fprintf(f,"----- \t ------ \t ---- \t ---- \t %s \t False \t %s\n",t->parent->str,t->str ); 
			// printf("----- \t ----- \t ---- \t ---- \t %s \t False \t %s\n",t->parent->str,t->str ); 
		}
		else{ 
			fprintf(f,"----- \t ------ \t ---- \t ---- \t NULL \t False \t %s\n",t->str ); 
			// printf("----- \t ----- \t ---- \t ---- \t NULL \t False \t %s\n",t->str ); 

		}
		TreeNode * x=t->down;
		x=x->next;
		while(x!=NULL){
			printParseTree(x);
			x=x->next;
		}
	}

}

void display(){
	printf("GENERAL INFO:\n");
	printf("0. Both Lexical Analyzer and Syntax Analyzer developed.\n");
	printf("1. Grammar is read from the file 'g2.txt'. Each non-terminal is written as a number. Each line corresponds to non-terminal.\n");
	printf("   For example, line 1 in 'g2.txt' is 'MAIN SQO SQC <2> END'\n");
	printf("   It is to interpreted as '<1> -> MAIN SQO SQC <2> END'\n");
	printf("2. Grammar stored in an Array of linked lists (file= 'grammarRead.c').\n");
	printf("3. First and Follow sets have been automated (file= 'grammarRead.c').\n");
	printf("4. Predictive Parsing Table has been automated (file= 'grammarRead.c').\n");
	printf("5. Error recovery implemented in Lexer.\n");
	printf("6. Error recovery (panic mode) implemented in parser.\n");
}

void display2(){
	printf("\nPROGRAM OPTIONS:\n");
	printf("Press 0 to Exit.\n");
	printf("Press 1 to print the source code without comments.\n");
	printf("Press 2 to print the tokens generated.\n");
	printf("Press 3 to start parsing the source code.\n");
	printf("Press 4 to print the parse tree.\n");
	printf("\n");
}	

int main(int argc, char const *argv[])
{
	/* code */
	if(argc<=1){
		printf("Please enter filename command line arguments.\n");
		return 0;
	}

	constructAndPopulateTrie();
	display();
	display2();
	int k;
	scanf("%d",&k);
	while(k!=0){
		if(k==1){
			emptyBuffer();
			sourceCode=fopen(argv[1],"r");
			fread(lex_buffer,(size_t)1,(size_t)lex_bufSize,sourceCode);
			
			Token * tk=getNextToken();
			int linenumber=1;
			while(tk!=NULL){
				if(tk->lineNumber>linenumber){
					printf("\n");
					linenumber++;
				}
				if(tk->type==2){
					printf("%s",tk->c );
				}
				else if(tk->type==3){
					printf("%d",tk->i);
				}
				else if(tk->type==4){
					printf("%.2f", tk->f);
				}
				tk=getNextToken();
			}
			fclose(sourceCode);
			printf("\n");
			sourceCode=NULL;
			return 0;
			// return 0;
		}
		else if(k==2){
			emptyBuffer();
			sourceCode=fopen(argv[1],"r");
			fread(lex_buffer,(size_t)1,(size_t)lex_bufSize,sourceCode);
			Token * tk=getNextToken();
			while(tk!=NULL){
				if(tk->type==2){
					printf("Line Number= %d, ",tk->lineNumber );
					printf("Token Name= %s, ",tk->tokenName);
					printf("Value= %s\n",tk->c );
				}
				else if(tk->type==3){
					printf("Line Number= %d, ",tk->lineNumber );
					printf("Token Name= %s, ",tk->tokenName);
					printf("Value= %d\n",tk->i );
				}
				else if(tk->type==4){
					printf("Line Number= %d, ",tk->lineNumber );
					printf("Token Name= %s, ",tk->tokenName);
					printf("Value= %.2f\n",tk->f );
				}
				tk=getNextToken();
			}
			fclose(sourceCode);
			printf("\n");
			sourceCode=NULL;
			return 0;
			// return 0;
		}
		else if(k==3){
			emptyBuffer();
			sourceCode=fopen(argv[1],"r");
			if(sourceCode==NULL){
				printf("File %s does not exist.\n",argv[1] );
				return 0;
			}

			fread(lex_buffer,(size_t)1,(size_t)lex_bufSize,sourceCode);
			
			// create stack
			stack=createStack();
			push(stack,"%"); // push the end marker symbol
			push(stack,"<1>");

			// read and parse grammar. compute first sets, follow sets and predictive parsing table
			g=createGrammarStruct();
			readGrammar(g);
			computeFirstSets(g);
			computeFollowSets(g);
			computePredictiveParsingTable(g);	
			// printAllFirstSets(g);
		 	// printAllFollowSets(g);
			// printTerminalIndexList();
			// printPredictiveParsingTable();
			
			printf("Starting Parser\n");
			
			// call parser to parse the source code
			
			parseTree=parse();

			if(error_flag2==0 && error_flag==0){
				printf("\nParsing Complete. Successful Compilation.\n");	
			}
			else if(error_flag2==1 || error_flag==1){
				printf("\nUnsuccessful compilation. Errors were found.\n");
			}
		}
		else if(k==4){
			if(argc<3){
				printf("Please enter filename for parse tree output\n");
				return 0;
			}
			f=fopen(argv[2],"w");
			fprintf(f,"LEXEME \t LINE NO. \t TOKEN \t VALUE \t PARENT \t isLeaf \t NodeSymbol\n");
			// printf("LEXEME \t LINE NO. \t TOKEN \t VALUE \t PARENT \t isLeaf \t NodeSymbol\n");
			printParseTree(parseTree);
			// fclose(f);
			return 0;
		}
		else if(k==5){
			printf("CREATING AST\n");
			ast=createAst(parseTree);
			printf("PRINTING AST........................\n");
			printSimpleParseTree(ast);
			printf("CREATING AST COMPLETED\n");
		}
		else if(k==6){
			printf("CREATING SYMBOL TABLE\n");
			st=createNewTreeNode("MAIN",NULL);
			createSymbolTable(ast,st);
			printf("---------------------------------------------------------------------------\n");
			printSymbolTable(st);
			printf("CREATING SYMBOL TABLE COMPLETED\n");
		}
		else if(k==7){
			printf("TPYE CHECKING STARTED\n");
			semanticsChecker(ast,st);
			printf("TPYE CHECKING ENDED\n");
		}
		else{
			printf("%d is not a valid option. Please Choose a valid option.\n", k);
		}

		display2();
		scanf("%d",&k);
	}
	return 0;
}