/******************************************************************************
 * File Name: puzzle.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-11-29
 * Revision:  v1.0
 * COMMENTS: Implements the type t_puzzle and the functions to operate it
 *****************************************************************************/

#ifndef _PUZZLE_H
#define _PUZZLE_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "hash.h"

/*Type definition for a structure to hold the puzzle*/
typedef struct _t_puzzle t_puzzle;

/*Create and free elements of type t_puzzle*/
t_puzzle * createPuzzle(FILE * fp);
void       freePuzzle(t_puzzle *puzzle, int matriz_size);

/*Get elements from the puzzle struct*/
int*        getZerosCols(t_puzzle* game);
int*        getZerosLines(t_puzzle* game);
int         getNines(t_puzzle* game);
int        getDim(t_puzzle *puzzle);
FLAGGY     getFlag(t_puzzle *puzzle);
int **     getTable(t_puzzle *puzzle);
int        getGamemode(t_puzzle * puzzle);
int        getHashSize(t_puzzle * puzzle);
int *      getVetorLinhas (t_puzzle * puzzle);
int *      getVetorColunas (t_puzzle * puzzle);
t_hash_list ** getHashColunas (t_puzzle * puzzle);
t_hash_list ** getHashLinhas (t_puzzle * puzzle);



/*Change elements of the puzzle*/
t_puzzle * updatePuzzle(FILE * fpIn, t_puzzle * game, int dim, FLAGGY flag, int gamemode, int biggestSize);
void       changeFlag (t_puzzle * game, FLAGGY flag);
void        decNines(t_puzzle* game);
void        incNines(t_puzzle* game);

/*Funciton to print the first line into a file*/
void       printFirstLine(FILE * fpOut, t_puzzle * puzzle);


#endif
