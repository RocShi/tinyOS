#include "tLib.h"

void tNodeInit(tNode *node)													//The node initial function
{
	node->nextNode=node;
	node->preNode=node;
}

void tListInit(tList *list)													//The list initial function
{
	list->firstnode=&(list->headNode);
	list->lastnode=&(list->headNode);
	list->nodeCount=0;
}

uint32_t tListCount(tList *list)										//Return the numbers of nodes in the list
{
	return list->nodeCount;
}

void tListAddFirst(tList *list, tNode *node)					//Add a node at the begin of the list
{
	node->nextNode=list->firstnode;
	node->preNode=&(list->headNode);
	
	list->firstnode->preNode=node;
	list->firstnode=node;
	
	list->nodeCount++;
}

void tListAddLast(tList *list, tNode *node)					//Add a node at the end of the list
{
	node->nextNode=&(list->headNode);
	node->preNode=list->lastnode;
	
	list->lastnode->nextNode=node;
	list->lastnode=node;
	
	list->nodeCount++;
}

void tListInsertAfter(tList *list, tNode *nodeAfter, 
											tNode *nodeToInsert)					//Add a node at the begin of the list
{
	nodeToInsert->nextNode=nodeAfter->nextNode;
	nodeToInsert->preNode=nodeAfter;
	
	nodeAfter->nextNode->preNode=nodeToInsert;
	nodeAfter->nextNode=nodeToInsert;
	
	list->nodeCount++;
}

void tListRemoveNode(tList *list, tNode *node)			//Remove one given node in the list
{
	if(list->nodeCount>0)
	{
		node->preNode->nextNode=node->nextNode;
		node->nextNode->preNode=node->preNode;
		
		list->nodeCount--;
	}
	return;
}

void tListRemoveFirst(tList *list)									//Remove the first node of the list
{
	if(list->nodeCount>0)
	{
		list->firstnode->nextNode->preNode=&(list->headNode);
		list->firstnode=list->firstnode->nextNode;
		
		list->nodeCount--;
	}
	return;
}

void tListRemoveAll(tList *list)										//Remove all the nodes of the list
{
	tNode *nextNode=list->firstnode;
	tNode *currentNode;
	while(list->nodeCount>0)
	{		
		currentNode=nextNode;
		nextNode=nextNode->nextNode;
				
		currentNode->nextNode=currentNode;
		currentNode->preNode=currentNode;
		
		list->nodeCount--;
	}
	
	list->firstnode=&(list->headNode);
	list->lastnode=&(list->headNode);
}

tNode * tListFirst(tList *list)											//Return the first node of the list
{
	if(list->nodeCount>0)
		return list->firstnode;
	else
		return ((tNode *)0);
}

tNode * tListLast(tList *list)											//Return the last node of the list
{
	if(list->nodeCount>0)
		return list->lastnode;
	else
		return ((tNode *)0);
}

tNode * tListPre(tList *list, tNode *node)					//Return the node in the list before the given node
{
	if(node->preNode==node)
		return ((tNode *)0);
	else
		return node->preNode;
}

tNode * tListNext(tList *list, tNode *node)					//Return the node in the list after the given node
{
	if(node->nextNode==node)
		return ((tNode *)0);
	else
		return node->nextNode;
}
