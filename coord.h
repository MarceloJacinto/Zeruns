/******************************************************************************
 * File Name: coord.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements the data types that will save the info related to
 * the bactrack system
 *****************************************************************************/
#ifndef _COORD_H
#define _COORD_H

#include <stdio.h>
#include <stdlib.h>

/*Type definition for a structure to hold coordinates and the number we put there*/
typedef struct _t_coord coord;

/*Function to allocate a coord* element and initialize it*/
coord *     newCoord(int line, int col, int num);

/*Functions to get items from a coord* */
int         getLineFromCoord(coord * coordinates);
int         getColFromCoord(coord * coordinates);
int         getNumFromCoord(coord * coordinates);

/*Function to free a coord* element*/
void        freeCoord(coord * coordinates);

/*Type definition for a structure to hold the unsigned ints that we insert in*/
/*the hash, and the index of the line/columns that it corresponds to*/
typedef struct  _t_hash_container hash_container;

/*Funcrion to allocate a hash_container* element*/
hash_container * allocHashContainer (unsigned int num, int col_line);

/*Functions to get items from a container* */
unsigned int     getNumFromHashContainer(hash_container* container);
unsigned int     getColLineFromHashContainer(hash_container* container);

/*Function to free a hash_container* element*/
void             freeHashContainer (hash_container* container);


#endif
