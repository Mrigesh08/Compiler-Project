// ID  =2015A7PS0146P
// Name=Mrigesh Madaan


#include "lexer.h"
#include "trie.h"

int lexemeStart=0;
int forward=0;
int lineNumber=1;
Node * trie;
int error_flag=0;

int forwardCheck1(){
	if(forward==lex_bufSize){
		return 1;
	}
	return 0;
}

void emptyBuffer(){
	lexemeStart=0;
	forward=0;
	lineNumber=1;
	error_flag=0;

	for(int i=0;i<lex_bufSize;i++){
		lex_buffer[i]='\0';
	}
}

void reloadBuffer(){
	int x=fread(lex_buffer,(size_t)1,(size_t)lex_bufSize,sourceCode);
	if(x<lex_bufSize){
		lex_buffer[x]='\0';
		// printf("2 value of buffer after reloading %s\n",lex_buffer );
	}		
}
void realoadBuffer2(int index){
	int x=fread(lex_buffer+index,(size_t)1,(size_t)(lex_bufSize-index),sourceCode);
	if(x<lex_bufSize-index){
		lex_buffer[x+index]='\0';
		// printf("1 value of buffer after reloading %s\n",lex_buffer );
	}
}
int copyCharsToBufferStart(int index){
	int i=0;
	while(index<lex_bufSize){	
		lex_buffer[i]=lex_buffer[index];
		index++;
		i++;
	}
	return i;
}
void retract(){
	forward--;
	lexemeStart=forward;
}
char * createString(char * str, int start, int end){
	// start is included, end is not included
	char * c=(char *)malloc(sizeof(char)*(end-start+1));
	int i=start;
	while(i<end){
		c[i-start]=str[i];
		i++;
	}
	c[i]='\0';
	return c;
}

void forwardCheck(){
	if(forwardCheck1()==1){
		int x=copyCharsToBufferStart(lexemeStart);
		realoadBuffer2(x);
		lexemeStart=0;
		forward=x;
	}
}

Token * generateToken(char * name,char * str, int type){
	// TODO:
	// handle name as ID to create a symbol table entry
	// handle NUM to convert it to a number and RNUM to real number
	// handle STRING
	Token * tk=(Token *)malloc(sizeof(Token));
	tk->tokenName=(char *)malloc(sizeof(name));
	strcpy(tk->tokenName,name);
	tk->lineNumber=lineNumber;
	if(type!=-1){
		if(type==1){
			// its a keyword
			// TODO : handle this case
			// it will be different for main and different for other id names.
			tk->type=1;
		}
		else if(type==2){
			tk->type=2;
			tk->c=(char *)malloc(sizeof(str)+1);
			strcpy(tk->c,str);
		}
		else if(type==3){
			tk->type=3;
			tk->i=atoi(str);
		}
		else if(type==4){
			tk->type=4;
			tk->f=atof(str);
		}
	}
	else{
		tk->type=-1;
	}
	return tk;
}
void constructAndPopulateTrie(){
	trie=newTrieNode();
	populateTrie(trie);
	// printf("Trie populated\n");
}
void checkPresenceInTrieAndConvert(char * str){
	if(checkPresenceInTrie(str,trie)==1){
		// printf("Trie => presence of %s confirmed.\n",str );
		for(int i=0;i<strlen(str);i++){
			str[i]=str[i]-'a'+'A';	
		}
	}
}

Token * getNextToken(){
	int state=0;
	char c;
	char * ch;
	int len=0;
	while(1){

		switch(state){
			case 0:
				if(forwardCheck1()==1){
					reloadBuffer();
					lexemeStart=0;
					forward=0;
				}
				c=lex_buffer[forward];
				forward++;
				if(c=='=') state=1;
				else if(c=='#') state=5;
				else if(c=='_') state=6;
				else if((c>='a' && c <='z') || (c>='A' && c<='Z') ) state=8;
				else if(c>='0' && c<='9' ) state=10;
				else if(c=='"') state=14;
				else if(c=='[') state=17;
				else if(c==']') state=18;
				else if(c=='(') state=19;
				else if(c==')') state=20;
				else if(c==';') state=21;
				else if(c==',') state=22;
				else if(c=='+') state=23;
				else if(c=='-') state=24;
				else if(c=='*') state=25;
				else if(c=='/') state=26;
				else if(c=='@') state=27;
				else if(c=='.') state=28;
				else if(c=='<') state=40;
				else if(c=='>') state=42;
				else if(c=='\n') state=44;
				else if(c=='\0') state=45;
				else lexemeStart++;
				break;
			
			case 1:
				if(forwardCheck1()==1){
					reloadBuffer();
					lexemeStart=0;
					forward=0;
				}
				c=lex_buffer[forward];
				forward++;
				if(c=='=') state=2;
				else if(c=='/') state=3;
				else{ 
					retract();
					return generateToken("ASSIGNOP","=",2);
				}
				break;
			
			case 2:
				lexemeStart=forward;
				state=0;
				return generateToken("EQ","==",2);
				break;

			case 3:
				if(forwardCheck1()==1){
					reloadBuffer();
					lexemeStart=0;
					forward=0;
				}
				c=lex_buffer[forward];
				forward++;
				if(c=='=') state=4;
				else{
					// return some trap state or error
					// shall i retract or not?
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}

			case 4:
				lexemeStart=forward;
				return generateToken("NE","=!=",2);
				break;
			
			case 5:
				c=lex_buffer[forward];
				// printf("Printing Comment\n");
				// printf("%c",c);
				forward++;
				while(c!='\n'){
					c=lex_buffer[forward];
					// printf("%c",c);
					forward++;
					if(forwardCheck1()==1){
						// read chars 1 by 1 until '\n' is reached
						char * str=(char *)malloc(sizeof(char ));
						fread(str,(size_t)1,(size_t)1,sourceCode);
						// printf("%c",*str );
						while(*str!='\n'){
							fread(str,(size_t)1,(size_t)1,sourceCode);
							// printf("%c",*str );
						}

						reloadBuffer();
						lexemeStart=0;
						forward=0;

						break;
					}
				}
				lexemeStart=forward;
				state=0;
				break;
			
			case 6:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if((c>='a' && c <='z') || (c>='A' && c<='Z')) state=7;
				else{
					// return to some trap state or error
					// what now?
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'. Grammar rules disallow the use of this symbol in identifiers.\n",lineNumber,c );
					state=7;
				}
				break;

			case 7:
				forwardCheck();
				c=lex_buffer[forward];
				len=1;
				while((c>='a' && c <='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')){
					forward++;
					len++;
					forwardCheck();
					c=lex_buffer[forward];
				}
				ch=createString(lex_buffer,lexemeStart,forward);
				lexemeStart=forward;
				char yoyo[]="_main";
				if(strcmp(ch,yoyo)==0){
					return generateToken("MAIN",ch,2);
				}
				return generateToken("FUNID",ch,2);
				break;

			case 8:
				forwardCheck();
				c=lex_buffer[forward];
				len=1;
				while((c>='a' && c <='z') || (c>='A' && c<='Z')){
					forward++;
					len++;
					forwardCheck();
					c=lex_buffer[forward];
				}
				if(len>20){
					// return error here
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Length of the identifier is greater than the prescribed length.\n",lineNumber );
			
				}
				if(c>='0' && c<='9'){state=9; break;}
				else{
					ch=createString(lex_buffer,lexemeStart,forward);
					lexemeStart=forward;
					if(checkPresenceInTrie(ch,trie)==1){
						char * yoyoo=(char *)malloc(sizeof(char )*(strlen(ch)+1));
						int i;
						for(i=0;i<strlen(ch);i++){
							yoyoo[i]=ch[i]-'a'+'A';	
						}
						yoyoo[i]='\0';
						return generateToken(yoyoo,ch,2);	
					}
					return generateToken("ID",ch,2);
					// TODO:
					// how will you add the symbol table entry for this?				
				}
				break;

			case 9:
				forward++;
				ch=createString(lex_buffer,lexemeStart,forward);
				if(checkPresenceInTrie(ch,trie)==1){
					char * yoyoo=(char *)malloc(sizeof(char )*(strlen(ch)+1));
					int i;
					for(i=0;i<strlen(ch);i++){
						yoyoo[i]=ch[i]-'a'+'A';	
					}
					yoyoo[i]='\0';
					return generateToken(yoyoo,ch,1);	
				}
				lexemeStart=forward;
				return generateToken("ID",ch,2);
				break;
			
			case 10:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c>='0' && c<='9') state=10;
				else if(c=='.') state=11;
				else{
					ch=createString(lex_buffer,lexemeStart,forward);
					retract();
					return generateToken("NUM",ch,3);
				}
				break;

			case 11:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='a') state=29;
				else if(c=='o') state=33;
				else if(c=='n') state=36;
				else if(c>='0' && c<='9') state=12;
				else{
					// some error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c' after the dot.\n",lineNumber,c );
					state=0;
					// retract or not? basically where should i start if this wrong token is seen
				}
				break;

			case 12:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c>='0' && c<='9') state=13;
				else{
					// some error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'. A real number must have two digits at the after the decimal.\n",lineNumber,c );
					state=0;
					// retract or not?
				}
				break;

			case 13:
				ch=createString(lex_buffer,lexemeStart,forward);
				return generateToken("RNUM",ch,4);
				break;

			case 14:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if((c>='a' && c<='z') || c==' ') state=15;
				else{
					// produce some error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Strings cannot include the symbol '%c'.\n",lineNumber,c );
					state=15;
					// where to go
				}
				break;

			case 15:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if((c>='a' && c<='z') || c==' ') state=15;
				else if(c=='"') state=16;
				else{
					// produce some error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Strings cannot include the symbol '%c'.\n",lineNumber,c );
					state=15;
				}
				break;

			case 16:
				ch=createString(lex_buffer,lexemeStart,forward);
				if(strlen(ch)>20){
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Length of string should be less than 20.\n",lineNumber);				
				}
				lexemeStart=forward;
				return generateToken("STR",ch,2);
				break;

			case 17:
				lexemeStart=forward;
				return generateToken("SQO","[",2);
				break;

			case 18:
				lexemeStart=forward;
				return generateToken("SQC","]",2);
				break;

			case 19:
				lexemeStart=forward;
				return generateToken("OP","(",2);
				break;

			case 20:
				lexemeStart=forward;
				return generateToken("CL",")",2);
				break;

			case 21:
				lexemeStart=forward;
				return generateToken("SEMICOLON",";",2);
				break;

			case 22:
				lexemeStart=forward;
				return generateToken("COMMA",",",2);
				break;

			case 23:
				lexemeStart=forward;
				return generateToken("PLUS","+",2);
				break;

			
			case 24:
				lexemeStart=forward;
				return generateToken("MINUS","-",2);
				break;

			case 25:
				lexemeStart=forward;
				return generateToken("MUL","*",2);
				break;

			case 26:
				lexemeStart=forward;
				return generateToken("DIV","/",2);
				break;

			case 27:
				lexemeStart=forward;
				return generateToken("SIZE","@",2);
				break;

			case 28:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='a') state=29;
				else if(c=='o') state=33;
				else if(c=='n') state=36;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				
				}
				break;
			
			case 29:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='n') state=30;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 30:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='d') state=31;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 31:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='.') state=32;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 32:
				lexemeStart=forward;
				return generateToken("AND",".and.",2);
				break;

			case 33:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='r') state=34;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 34:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='.') state=35;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 35:
				lexemeStart=forward;
				return generateToken("OR",".or.",2);
				break;

			case 36:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='o') state=37;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 37:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='t') state=38;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 38:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='.') state=39;
				else{
					// produce error
					error_flag=1;
					printf("ERROR at line no. %d: Lexical Error : Unexpected symbol '%c'.\n",lineNumber,c );
					state=0;
				}
				break;

			case 39:
				lexemeStart=forward;
				return generateToken("NOT",".not.",2);
				break;

			case 40:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='=') state=41;
				else{
					retract();
					return generateToken("LT","<",2);
				}
				break;

			case 41:
				lexemeStart=forward;
				return generateToken("LE","<=",2); 
				break;

			case 42:
				forwardCheck();
				c=lex_buffer[forward];
				forward++;
				if(c=='=') state=43;
				else{
					retract();
					return generateToken("GT",">",2);
				}
				break;

			case 43:
				lexemeStart=forward;
				return generateToken("GE",">=",2);
				break;

			case 44:
				lexemeStart=forward;
				lineNumber++;
				state=0;
				break;

			case 45:
				return NULL;
				// Terminate token stream somehow
				break;
		}	
	}

		
	


}

void printToken(Token * tk){
	// char * tokenName;
	// 	int lineNumber;
	// 	union {
	// 		struct head * h; // for the entry in symbol table
	// 		char * c;
	// 		int i;
	// 		float f;
	// 	};
	printf("Printing Token\n");
	printf("Token Name=%s\n",tk->tokenName );
	printf("Line Number=%d\n", tk->lineNumber);
	if(tk->type!=-1){
		printf("Value= ");
		if(tk->type==1){
			// printf("%s",tk->k );
		}
		else if(tk->type==2){
			printf("%s",tk->c);
		}
		else if(tk->type==3){
			printf("%d",tk->i );
		}
		else if(tk->type==4){
			printf("%f",tk->f );
		}
		printf("\n");
	}

}
void printNextBuffer(){
	int ptr=forward;
	for(int i=forward;i<lex_bufSize;i++){
		printf("%c",*(lex_buffer+i) );
	}
	printf("\n");
	printf("\n");
}
/*
int main(int argc, char const *argv[])
{
	sourceCode=fopen("source2.c","r");
	fread(lex_buffer,(size_t)1,(size_t)lex_bufSize,sourceCode);
	constructAndPopulateTrie();
	printNextBuffer();
	Token* tk=getNextToken();
	printToken(tk);

	while(1){
		// printNextBuffer();
		tk=getNextToken();
		if(tk!=NULL){
			printToken(tk);	

		}
		else{
			break;
		}
	}

	return 0;

}
*/