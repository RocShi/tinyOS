#ifndef TLIB_H
#define TLIB_H

#include <stdint.h>

#define firstnode headNode.nextNode									//The first node in the list
#define lastnode headNode.preNode										//The Last node in the list
#define tNodeParent(node, parent, name) (parent *)((uint32_t)node-(uint32_t)&((parent *)0)->name)

typedef struct																			//Bitmap data structure
{
	uint32_t bitmap;
}tBitmap;

typedef struct	_tNode															//Node data structure
{
	struct	_tNode *preNode;													//Previous node pointer
	struct	_tNode *nextNode;													//Next node pointer
}tNode;

typedef struct	_tList															//List data structure
{
	tNode headNode;																		//The head node of the list 
	uint32_t nodeCount;																//The numbers of nodes of the list
}tList;

void tBitmapInit(tBitmap *bitmap);									//Bitmap initiate
void tBitmapSet(tBitmap *bitmap, uint32_t pos);			//Set one bit of a bitmap 
void tBitmapClear(tBitmap *bitmap, uint32_t pos);		//Clear one bit of a bitmap
uint32_t tBitmapGetFirstSet(tBitmap *bitmap);				//Get the first setted bit of a bitmap
uint32_t tBitmapPosCount(void);											//Total setted bits of a bitmap

void tNodeInit(tNode *node);												//The node initial function
void tListInit(tList *list);												//The list initial function
uint32_t tListCount(tList *list);										//Return the numbers of nodes in the list
void tListAddFirst(tList *list, tNode *node);				//Add a node at the begin of the list
void tListAddLast(tList *list, tNode *node);				//Add a node at the end of the list
void tListInsertAfter(tList *list, tNode *nodeAfter, 
					  tNode *nodeToInsert);										//Add a node at the begin of the list
void tListRemoveNode(tList *list, tNode *node);			//Remove one given node in the list
void tListRemoveFirst(tList *list);									//Remove the first node of the list
void tListRemoveAll(tList *list);										//Remove all the nodes of the list
tNode * tListFirst(tList *list);										//Return the first node of the list
tNode * tListLast(tList *list);											//Return the last node of the list
tNode * tListPre(tList *list, tNode *node);					//Return the node in the list before the given node
tNode * tListNext(tList *list, tNode *node);				//Return the node in the list after the given node

#endif
