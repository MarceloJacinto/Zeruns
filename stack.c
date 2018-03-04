/******************************************************************************
 * File Name: stack.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: implements abstract data type Stack and functions to operate it
 *****************************************************************************/
#include "stack.h"

/*Declaration of the datatype*/
struct _stack_node {
    Item item;
    struct _stack_node *next;
};

/*****************************************************************************
* createStackNode
* Arguments: Item item
* Returns: Pointer to a stackNode
* Side-Effects: Allocates memory for a newNode, with newNode->item = item
*****************************************************************************/
stack_node * createStackNode(Item item)
{
    stack_node * newNode;

    newNode = (stack_node*)malloc(sizeof(stack_node));
    if (newNode == NULL) exit(0);

    newNode->item = item;
    newNode->next = NULL;

    return newNode;
}

/*****************************************************************************
* pushItem
* Arguments: stack_node** stack, Item item
* Returns: void
* Side-Effects: Allocates memory for a newNode and puts it on top of the stack
*****************************************************************************/
void pushItem(stack_node ** stack, Item item)
{
    stack_node * node = createStackNode(item);
    node->next = *stack;
    *stack = node;
}

/*****************************************************************************
* isEmpty
* Arguments: stack_node** stack
* Returns: TRUE OR FALSE, if the stack is empty or not, respectively
*****************************************************************************/
Boolean isEmpty(stack_node** stack)
{
    if (*stack != NULL) return FALSE;
    return TRUE;
}

/*****************************************************************************
* popItem
* Arguments: stack_node** stack
* Returns: Item stored on the top of the stack
* Side-Effects: Removes an element from the top of the stack
*****************************************************************************/
Item popItem(stack_node ** stack)
{
    stack_node * our_node;
    Item our_item;

    if (isEmpty(stack) == TRUE) return NULL;

    our_node = *stack;
    *stack = our_node->next;
    our_item = our_node->item;

    free (our_node);
    return our_item;
}
