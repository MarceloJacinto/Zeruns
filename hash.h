/******************************************************************************
 * File Name: hash.h
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements hash tables and functions to operate them
 *****************************************************************************/
#ifndef _HASH_H
#define  _HASH_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "utils.h"

/*Functions to create and initialize the table*/
t_hash_list** createHashTable(int dim);
void initializeHashTable(t_hash_list** hashTable, int hash_size);

/*Function to choose and adequate prime numnber for the size of the table*/
int chooseHashSize(int dim);

/*Hashing function*/
int getIdx(unsigned int n, int hash_size);

/*Functions to insert elements in the table or check if they are there*/
int checkRepetition(t_hash_list** hashTable, unsigned int num, int hash_size, int index, ROW row, int** matriz, int matriz_size);
int findInsertNum(t_hash_list* head, int ** matriz, unsigned int num, int matriz_size , int index1, ROW row);

/*Functions to remove elements from the table*/
void removeNum(t_hash_list** hash, unsigned int num, int hash_size, int col_line, int dim);
void findRemoveNum(t_hash_list** hashTable, t_hash_list* head, unsigned int num, int idx, int col_line, int dim);

/*Function to free the table elements*/
void freeHashTableElements(t_hash_list** hashTable, int hash_size);

#endif
