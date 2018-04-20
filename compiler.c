#include "grammarRead.h"
#include "stack.h"
#include "ntree.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeChecker.h"

int error_flag2=0; // represents an error found in parsing
int error_flag3=0; // represents an error found after parsing while creating symbolTable, TypeChecking etc
TreeNode * parseTree=NULL;
TreeNode * ast=NULL;
TreeNode * st=NULL;

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
			if(error_flag2==1){
				printf("Cannot create AST because errors were found in creating the parse tree.\n");
			}
			else{
				printf("CREATING AST\n");
				ast=createAst(parseTree);
				printf("PRINTING AST........................\n");
				printSimpleParseTree(ast);
				printf("CREATING AST COMPLETED\n");	
			}
			
		}
		else if(k==6){
			if(error_flag2==1){
				printf("Cannot create Symbol Table because errors were found in creating the parse tree.\n");
			}
			else if(ast==NULL){
				printf("Please create AST first. Symbol table required the use of AST\n");
			}
			else{
				printf("CREATING SYMBOL TABLE\n");
				st=createNewTreeNode("MAIN",NULL);
				createSymbolTable(ast,st);
				printf("---------------------------------------------------------------------------\n");
				printSymbolTable(st);
				printf("CREATING SYMBOL TABLE COMPLETED\n");	
			}
			
		}
		else{
			printf("%d is not a valid option. Please Choose a valid option.\n", k);
		}

		display2();
		scanf("%d",&k);
	}
	return 0;
}