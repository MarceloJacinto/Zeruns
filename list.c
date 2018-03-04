/******************************************************************************
 * File Name: list.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements the data type List and functions to operate it
 *****************************************************************************/
#include "list.h"

/*Declaration of the data type*/
struct _t_hash_list
{
    unsigned int num;
    int col_line;
    t_hash_list* next;
    t_hash_list* prev;
};

/*****************************************************************************
* createListNode
* Arguments: int num
* Returns: Pointer to a listNode
* Side-Effects: Allocates memory for a new list node, and initializes
* newNode->num with num and newNode->col_line = col_line
*****************************************************************************/
t_hash_list* createListNode(unsigned int num, int col_line)
{
    t_hash_list* newNode = NULL;

    newNode = (t_hash_list*)malloc(sizeof(t_hash_list));
    if(newNode == NULL) exit(0);

    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->num = num;
    newNode->col_line = col_line;

    return newNode;
}

/*****************************************************************************
* insertListNode
* Arguments: int num, t_hash_list* prev
* Returns: void
* Side-Effects: Inserts a node after the prev element
*****************************************************************************/
t_hash_list* insertListNode(unsigned int num, int col_line, t_hash_list* prev)
{
    t_hash_list* newNode;

    newNode = createListNode(num, col_line);
    newNode->prev = prev;
    if(prev != NULL) prev->next = newNode;
    return newNode;
}

/*****************************************************************************
* removeListNode
* Arguments: t_hash_list* curr
* Returns: void
* Side-Effects: Frees the element pointed by curr
*****************************************************************************/
void removeListNode(t_hash_list** curr)
{
    t_hash_list *aux = *curr, *temp = NULL;
    if (aux == NULL) return;
    if(aux->prev != NULL) aux->prev->next = aux->next;
    if(aux->next != NULL) aux->next->prev = aux->prev;
    temp = aux;
    if(aux->prev == NULL) aux = aux->next;
    free(temp);
    *curr = aux;
}

/*****************************************************************************
* getListNum
* Arguments: t_hash_list* node
* Returns: node->num
* Side-Effects: none
*****************************************************************************/
unsigned int getListNum(t_hash_list* node)
{
    return node->num;
}

/*****************************************************************************
* getListColLine
* Arguments: t_hash_list* node
* Returns: node->col_line
* Side-Effects: none
*****************************************************************************/
int getListColLine(t_hash_list* node)
{
    return node->col_line;
}

/*****************************************************************************
* getListNext
* Arguments: t_hash_list* curr
* Returns: curr->next
* Side-Effects: none
*****************************************************************************/
t_hash_list* getListNext(t_hash_list* curr)
{
    return curr->next;
}

/*****************************************************************************
* getListPrev
* Arguments: t_hash_list* curr
* Returns: curr->prev
* Side-Effects: none
*****************************************************************************/
t_hash_list* getListPrev(t_hash_list* curr)
{
    return curr->prev;
}

/*****************************************************************************
* freeList
* Arguments: t_hash_list* head
* Returns: void
* Side-Effects: Frees a list whose head is 'head'
*****************************************************************************/
void freeList(t_hash_list* head)
{
    t_hash_list* aux = head, *temp = NULL;

    while(aux != NULL)
    {
        temp = aux;
        aux = aux->next;
        free(temp);
    }
}
