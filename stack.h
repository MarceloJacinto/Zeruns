/******************************************************************************
 * File Name: stack.h
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: implements abstract data type Stack and functions to operate it
 *****************************************************************************/
#ifndef _STACK_H_
#define _STACK_H_
#include "utils.h"

/*Abstract declaration of the data type*/
typedef struct _stack_node stack_node;

/*Functions to operate the stack*/
void         pushItem(stack_node ** stack, Item item);
Boolean      isEmpty(stack_node ** stack);
Item         popItem(stack_node ** stack);

#endif
