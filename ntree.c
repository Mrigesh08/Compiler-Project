#include "ntree.h"

TreeNode * createNewTreeNode(char * str,Token * tk){
	TreeNode * n=(TreeNode *)malloc(sizeof(TreeNode));
	// n->str=( char *)malloc(sizeof(str)+1);
	// strcpy(n->str,str);
	n->str=str;
	n->token=tk;
	n->nextEntry=NULL;
	n->symbolTableNode=NULL;
	n->next=NULL;
	n->down=NULL;
	n->parent=NULL;
	return n;
}