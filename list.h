/******************************************************************************
 * File Name: list.h
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-11-29
 * Revision:  v1.0
 * COMMENTS: Implements the data type List and functions to operate it
 *****************************************************************************/
#ifndef _LIST_H
#define  _LIST_H

#include <stdio.h>
#include <stdlib.h>

/*Declaration of the data type*/
typedef struct _t_hash_list t_hash_list;

/*Functions to create the list and add elements to it*/
t_hash_list* createListNode(unsigned int num, int col_line);
t_hash_list* insertListNode(unsigned int num, int col_line, t_hash_list* prev);

/*Function to remove elements from the lis*/
void removeListNode(t_hash_list** curr);

/*Functions to get information from a list element*/
unsigned int getListNum(t_hash_list* node);
int getListColLine(t_hash_list* node);
t_hash_list* getListNext(t_hash_list* curr);
t_hash_list* getListPrev(t_hash_list* curr);

/*Function to free the list*/
void freeList(t_hash_list* head);

#endif
