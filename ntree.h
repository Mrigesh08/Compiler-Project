// ID  =2015A7PS0146P
// Name=Mrigesh Madaan

#include <stdlib.h>
#include "lexer.h"


typedef struct _tn{
	char * str;
	struct _token * token;
	struct _tn * next;
	struct _tn * down;
	struct _tn * parent;
}TreeNode;


TreeNode * createNewTreeNode(char * str,Token * tk){
	TreeNode * n=(TreeNode *)malloc(sizeof(TreeNode));
	n->str=str;
	n->token=tk;
	n->next=NULL;
	n->down=NULL;
	n->parent=NULL;
	return n;
}