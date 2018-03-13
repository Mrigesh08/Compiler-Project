// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define numberOfRules 41 

char ** terminalIndex;
char *** ppt; // Predictive Parsing Table

// Keep it 1 extra than the actual number of rules

typedef struct _h{
	struct _n * first;
	int length;
	struct _n * last;
	struct _n * firstSets;
	struct _n * followSets;
	int isNullable; // 0=false,1=true	
}Head;

typedef struct _n{
	char * str;
	struct _n * next;

}Node;


Head * createGrammarStruct();

void readGrammar(Head * g);

void computeFirstSets(Head * g);

void computeFollowSets(Head * g);

void computePredictiveParsingTable(Head * g);

int getIndexOfTerminal(char * str);

void printAllFirstSets(Head * g);

void printAllFollowSets(Head * g);

void printTerminalIndexList();

void printPredictiveParsingTable();