// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include "grammarRead.h"


int numberOfTerminas=0;

Node * newNode(char * str){
	Node * n=(Node *)malloc(sizeof(Node));
	n->str=str;
	n->next=NULL;
	return n;
}

Head * createGrammarStruct(){
	Head * g=(Head *)malloc(sizeof(Head)*numberOfRules);
	for(int i=0;i<numberOfRules;i++){
		g[i].first=NULL;
		g[i].length=0;
		g[i].last=NULL;
		g[i].firstSets=NULL;
		g[i].followSets=NULL;
	}
	return g;
}


void readGrammar(Head * g){
	FILE * f=fopen("g2.txt","r");
	int lineNumber =1;
	while(lineNumber<numberOfRules){
		// printf("linenumber =%d\n",lineNumber );
		char * str=(char *)malloc(sizeof(char )*80);
		int i=0;
		fread(str,1,1,f);
		while(str[i]!='\n' && str[i]!='|'){
			fread(str+ ++i,1,1,f);
		}
		if(str[i]=='\n'){
			str[i]='\0';
			// printf("String Found -%s()\n",str );
			if( g[lineNumber].first==NULL){
				g[lineNumber].first=newNode(str);
				g[lineNumber].length=g[lineNumber].length+1;
				g[lineNumber].last=g[lineNumber].first;
			}
			else{
				(g[lineNumber].last)->next=newNode(str);
				Node * temp=(g[lineNumber].last)->next;
				g[lineNumber].last=temp;
				g[lineNumber].length=g[lineNumber].length+1;
			}
			lineNumber++;
		}
		else if(str[i]=='|'){
			str[--i]='\0';
			char c;
			fread(&c,1,1,f); // to take out the space after '|'
			// printf("String Found -%s()",str );
			// create a node and place it in the line numbers head
			if( g[lineNumber].first==NULL){
				g[lineNumber].first=newNode(str);
				g[lineNumber].length=g[lineNumber].length+1;
				g[lineNumber].last=g[lineNumber].first;
			}
			else{
				(g[lineNumber].last)->next=newNode(str);
				Node * temp=(g[lineNumber].last)->next;
				g[lineNumber].last=temp;
				g[lineNumber].length=g[lineNumber].length+1;
			}
		}
		
	}
}
Node * copyAll(Node * c){
	Node * n;
	if(c==NULL){
		return NULL;
	}
	else{
		n=newNode(c->str);
		c=c->next;
		Node * n2=n;
		while(c!=NULL){
			Node * x=newNode(c->str);
			n2->next=x;
			n2=x;
			c=c->next;
		}
	}
	return n;
}
Node * copyAll2(Node * c){
	Node * n;
	if(c==NULL){
		return NULL;
	}
	else{
		if((c->str)[0]=='$'){
			c=c->next;
		}
		n=newNode(c->str);
		c=c->next;
		Node * n2=n;
		while(c!=NULL){
			Node * x=newNode(c->str);
			n2->next=x;
			n2=x;
			c=c->next;
		}
	}
	return n;
}
void sparseCopy(Node * a, Node * c){
	// copies unique content of Node *c to Node *a
	Node * x=c;
	while(x!=NULL){
		Node * n=a;
		Node * prev=a;
		if((x->str)[0]!='$'){
			while(n!=NULL){
				if(strcmp(n->str,x->str)==0){
					break;
				}
				prev=n;
				n=n->next;
			}
			if(n==NULL){
				Node * k=newNode(x->str);
				prev->next=k;
			}
		}
		x=x->next;
	}
}

void findFirstSetRecursively(Head * g,int p,int * currentStack){
	Node * n=g[p].first;

	if(currentStack[p]==0){
		currentStack[p]=1;
	
		while(n!=NULL){
			char * str=n->str;
			int flag=0;

			for(int i=0;i<strlen(str) && str[i]!='\0' ;i++){
				if(str[i]=='<'){
					char * x=(char *)malloc(sizeof(char)*3);
					int j=0;
					++i;
					while(str[i]!='>'){
						x[j]=str[i];
						i++; j++;
					}
					x[j]='\0';
					int k=atoi(x);
					if(g[k].firstSets==NULL){
						findFirstSetRecursively(g,k,currentStack);
					}
					if(g[k].isNullable==0){
						Node * n2=g[p].firstSets;
						if(n2==NULL){
							g[p].firstSets=copyAll(g[k].firstSets);
						}
						else{
							while(n2->next!=NULL){
								n2=n2->next;
							}
							n2->next=copyAll(g[k].firstSets);						
						}
						break;
						// return g[k].firstSets;
					}
					i++;
					i++; // to take the char ' ' out
					if(i<strlen(str)){
						if(str[i]!='<'){
							if(g[k].isNullable==1){
								i--; // to bring pointed back to '<' coz i will be incremented in the for loop
								continue;

							}
							else{
								break;
							}
						}
						else{
							i--; // to bring pointed back to '<' coz i will be incremented in the for loop
						}
					}
					else{
						g[p].isNullable=1;
						break;
					}

				}
				else{
					// program reaches here only on two conditions.
					// 1. The very first word of str was a terminal
					// 2, Last non-terminal was Nullable
					char * x=(char *)malloc(sizeof(char)*20);
					int j=0;
					while(i<strlen(str) && ((str[i]>='A' && str[i]<='Z') || str[i]=='$')){
						x[j]=str[i];
						i++;
						j++;
					}
					x[j]='\0';
					Node * n2=newNode(x);
					n2->next=g[p].firstSets;
					g[p].firstSets=n2;
					if(str[0]=='$'){
						g[p].isNullable=1;
					}
					break;
				}

			}
			n=n->next;
		}
		currentStack[p]=0;
	}
}

void findFollowSetRecursively(Head * g,int p, int * cs){
	if(cs[p]==0){
		cs[p]=1;
	
	for(int i=1;i<numberOfRules;i++){
		Node * n=g[i].first;
		while(n!=NULL){
			char * str=n->str;
			for(int j=0;j<strlen(str);j++){
				if(str[j]=='<'){
					j++;
					char * x=(char *)malloc(sizeof(char)*3);
					int k=0;
					while(str[j]!='>'){
						x[k]=str[j];
						j++;
						k++;
					}
					x[k]='\0';
					int h=atoi(x);
					if(h==p){
						// determine if h is the end. If it is, then follow(p) == follow(i)
						//  if is not the end:
						// next element is a termianl - add it in its follow set.
						// next element is a non-terminal - follo(p)==follow(atoi(nextelem)) 
						// as of now - str[j]=='>'
						j++;
						if(strlen(str)==j){
							// this mean that end of str has been reached
							findFollowSetRecursively(g,i,cs);
							if(g[p].followSets==NULL){
								g[p].followSets=copyAll(g[i].followSets);
							}
							else{
								/*
								Node * n2=g[p].followSets;
								while(n2->next!=NULL){
									n2=n2->next;
								}
								n2->next=copyAll(g[i].followSets);
								*/
								sparseCopy(g[p].followSets,g[i].followSets);
							}
						}
						else{
							j++; // to take out the space
							if(str[j]=='<'){
								// this a non-terminal
								j++;
								char * y=(char *)malloc(sizeof(char)*3);
								int q=0;
								while(str[j]!='>'){
									y[q]=str[j];
									j++;
									q++;
								}
								y[q]='\0';
								int z=atoi(y);
								// findFollowSetRecursively(g,z,cs);
								if(g[p].followSets==NULL){
									g[p].followSets=copyAll2(g[z].firstSets);
								}
								else{
									sparseCopy(g[p].followSets,g[z].firstSets);
								}
								
								if(g[z].isNullable==1){
									// findFollowSetRecursively(g,i,cs);
									/*
									int u=x;
									u++; // to take out the >
									u++; // to take out the space if any
									// loop till a terminal or str end is reached or non terminal is not nullable
									*/
									if(g[i].followSets==NULL){
										findFollowSetRecursively(g,i,cs);
									}
									if(g[p].followSets==NULL){
										g[p].followSets=copyAll(g[i].followSets);
									}
									else{	
										sparseCopy(g[p].followSets,g[i].followSets);
									}	
								}
								

							}	
							else if(str[j]>='A' && str[j]<='Z'){
								// this a terminal
								char * y=(char *)malloc(sizeof(char)*20);
								int q=0;
								while( j<strlen(str) && str[j]>='A' && str[j]<='Z'){
									y[q]=str[j];
									j++; q++;
								}
								y[q]='\0';
								Node * a=newNode(y);
								if(g[p].followSets==NULL){
									g[p].followSets=a;
								}
								else{
									/*
									a->next=g[p].followSets;
									g[p].followSets=a;
									
									*/
									if(y[0]!='$'){

									}
									sparseCopy(g[p].followSets,a);
								}
								break;
							}
						}


					}
				}
			}
			n=n->next;
		}
	}
	}
}
void sparseCopy2(char ** arr, Node * c){
	Node * x=c;
	while(x!=NULL){
		int flag_found=0;
		int i;
		for(i=0;i<50;i++){
			if(arr[i]==NULL){
				break;
			}
			else if(strcmp(arr[i],x->str)==0){
				flag_found=1;
				break;
			}
		}
		if(flag_found==0){
			arr[i]=(char *)malloc(sizeof(char)*20);
			strcpy(arr[i],x->str);
			numberOfTerminas++;
		}
		x=x->next;
	}
}
void computeFirstSets(Head * g){
	terminalIndex=(char **)malloc(sizeof(char *)*50);
	for(int i=0;i<50;i++){
		terminalIndex[i]=NULL;
	}
	int * cs=(int *)malloc(sizeof(int)*numberOfRules);
	for(int i=1;i<numberOfRules;i++){
		if(g[i].firstSets==NULL){
			for(int i=0;i<numberOfRules;i++){
				cs[i]=0;
			}
			findFirstSetRecursively(g,i,cs);
		}
		sparseCopy2(terminalIndex,g[i].firstSets);
		/*
		printf("First Sets for %d=\n",i );
		Node *n =g[i].firstSets;
		while(n!=NULL){
			printf("%s |",n->str );
			n=n->next;
		}
		if(g[i].isNullable==1){
			printf(" NULL");
		}
		printf("\n");
		*/
	}
	
	Node * n=newNode("ASSIGNOP");
	sparseCopy2(terminalIndex,n);
	
}

void computeFollowSets(Head * g){
	int * cs=(int *)malloc(sizeof(int)*numberOfRules);
	for(int i=1;i<numberOfRules;i++){
		
		if(g[i].isNullable==1 || g[i].isNullable==0){
			for(int i=0;i<numberOfRules;i++){
				cs[i]=0;
			}
			findFollowSetRecursively(g,i,cs);
			sparseCopy2(terminalIndex,g[i].followSets);
		}
	}
	g[1].followSets=newNode("END");
}

int getIndexOfTerminal(char * str){
	for(int i=0;i<50 && terminalIndex[i]!=NULL;i++){
		if(strcmp(str,terminalIndex[i])==0){
			return i;
		}
	}

	return -1;
}

Node * getFirstSet(Head * g,int p,Node * n){
	char * str=n->str;
	int flag=0;
	Node * answer=NULL;
	for(int i=0;i<strlen(str) && str[i]!='\0' ;i++){
		if(str[i]=='<'){
			char * x=(char *)malloc(sizeof(char)*3);
			int j=0;
			++i;
			while(str[i]!='>'){
				x[j]=str[i];
				i++; j++;
			}
			x[j]='\0';
			int k=atoi(x);

			if(g[k].isNullable==0){
				Node * n2=answer;
				if(n2==NULL){
					answer=copyAll2(g[k].firstSets);
				}
				else{
					while(n2->next!=NULL){
						n2=n2->next;
					}
					n2->next=copyAll2(g[k].firstSets);						
				}
				break;
				// return g[k].firstSets;
			}
			i++;
			i++; // to take the char ' ' out
			if(i<strlen(str)){
				if(str[i]!='<'){
					if(g[k].isNullable==1){
						i--; // to bring pointed back to '<' coz i will be incremented in the for loop
						continue;
					}
					else{
						break;
					}
				}
				else{
					i--; // to bring pointed back to '<' coz i will be incremented in the for loop
				}
			}
			else{
				// g[p].isNullable=1;
				// do something more here
				break;
			}

		}
		else{
			// program reaches here only on two conditions.
			// 1. The very first word of str was a terminal
			// 2, Last non-terminal was Nullable
			char * x=(char *)malloc(sizeof(char)*20);
			int j=0;
			while(i<strlen(str) && ((str[i]>='A' && str[i]<='Z') || str[i]=='$')){
				x[j]=str[i];
				i++;
				j++;
			}
			x[j]='\0';
			// if(x[0]!='$'){
				Node * n2=newNode(x);
				n2->next=answer;
				answer=n2;	
			// }
			break;
		}

	}
	Node * x=answer;
	while(x!=NULL){
		if((x->str)[0]=='$'){
			sparseCopy(answer,g[p].followSets);
		}
		x=x->next;
	}
	return answer;
}

void computePredictiveParsingTable(Head * g){
	ppt=(char ***)malloc(sizeof(char **)*numberOfRules);
	for(int i=1;i<numberOfRules;i++){
		ppt[i]=(char **)malloc(sizeof(char *)*numberOfTerminas);
		for(int j=0;j<numberOfTerminas;j++){
			ppt[i][j]=NULL;
		}
	}
	for(int i=1;i<numberOfRules;i++){
		Node * n=g[i].first;
		while(n!=NULL){	
			Node * answer=getFirstSet(g,i,n);
			// printf("Printing answer for %d\n",i);
			Node * no=answer;
			while(no!=NULL){
				// printf("%s\t", no->str);
				int index=getIndexOfTerminal(no->str);
				ppt[i][index]=(char *)malloc(sizeof(char)*60);
				strcpy(ppt[i][index],n->str);
				no=no->next;
			}
			// printf("\n");
			n=n->next;
		}
	}

}

void buildSynchronizingSet(Head * g){
	for(int i=1;i<numberOfRules;i++){
		Node * n=g[i].followSets;
		while(n!=NULL){
			int k=getIndexOfTerminal(n->str);
			if(ppt[i][k]==NULL){
				ppt[i][k]=(char *)malloc(sizeof(char )*10);
				strcpy(ppt[i][k],"synch");
				printf("INSERTED INSERTED INSERTED \n");
			}
			else{
				printf("ERROR\nERROR\nERROR\nERROR\n");
			}
			n=n->next;
		}
	}
}

void printAll(Head * g){
	for(int i=1;i<numberOfRules;i++){
		Node * n=g[i].first;
		while(n!=NULL){
			printf("%s |",n->str );
			n=n->next;
		}
		printf("\n");
	}
}

void printAllFirstSets(Head * g){
	for(int i=1;i<numberOfRules;i++){
		Node * n=g[i].firstSets;
		while(n!=NULL){
			printf("%s |",n->str );
			n=n->next;
		}
		printf("\n");
	}
}

void printAllFollowSets(Head * g){
	for(int i=1;i<numberOfRules;i++){
		Node * n=g[i].followSets;
		printf("%d. ", i);
		while(n!=NULL){
			printf("%s |",n->str );
			n=n->next;
		}
		printf("\n");
	}
}

void printTerminalIndexList(){
	for(int i=0;i<50;i++){
		printf("%d. %s\n",i,terminalIndex[i] );
	}
}

void printPredictiveParsingTable(){
	for(int i=1;i<numberOfRules;i++){
		printf("%d=> ",i );
		for(int j=0;j<numberOfTerminas;j++){
			if(ppt[i][j]!=NULL){
				printf("%s => %s | ",terminalIndex[j],ppt[i][j] );

			}
		}
		printf("\n");
	}
}
/*
int main(int argc, char const *argv[])
{
	Head * g=createGrammarStruct();
	readGrammar(g);
	// printAll(g);
	computeFirstSets(g);
	// printf("\nFIRST SETS \n");
	// printAllFirstSets(g);
	computeFollowSets(g);
	// printf("\nFOLLOW SETS \n");
	// printAllFollowSets(g);
	// printTerminalIndexList();
	computePredictiveParsingTable(g);
	// printPredictiveParsingTable();
	buildSynchronizingSet(g);

	return 0;
}
*/