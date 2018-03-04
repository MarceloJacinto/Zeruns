/******************************************************************************
 * File Name: solve.h
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-11-16
 * Revision:  v1.0
 * COMMENTS: Implements the most important functions needed to solve the puzzle
 *****************************************************************************/
#ifndef _SOLVE_H
#define _SOLVE_H
#include "puzzle.h"
#include "utils.h"
#include "eval.h"
#include "coord.h"

/*Main solving function, that will call all of the other functions*/
/*and decide the values to try when we can't know for sure what*/
/*value to put in a specific coordinate*/
void    recursiveSolver(t_puzzle* game, int line, int col);

/*Functions that fill the most they can using the rules*/
void    varrimento(t_puzzle* game, int line, int col, int ramo);
void    auxVarr(t_puzzle* game, int line_test, int col_test);
void    varrimento9(t_puzzle* game, int line, int col, int flag2);
void    auxVarr9(t_puzzle* game, int line, int col);

/*Functions used in mode 2 that certify that there aren't any*/
/*equal lines or columns, and that are responsible for the insertions*/
/*in the hash tables*/
void    mode2Checks(int line, int col, t_puzzle* game);
ANSWER  checkRepetitionWithBackup(ROW row, t_hash_list** hashTable, unsigned int num, int hash_size, int index, int **matriz, int size);

/*Function that fills the coordinates [line][col] with number*/
void    tryNumber(t_puzzle* game, int line, int col, int number);

/*Functions to get coordinates of specific things in the matrix*/
void    getFirstPositionVarrimento(t_puzzle* game, int* line, int* col);
FLAGGY  searchNextNine(t_puzzle* game, int* line, int* col);
int     searchNotNine(t_puzzle* game, int* line, int* col);
int     getNextPosition(t_puzzle* game, int* line, int* col);

/*Stack operations that make our backtrack system work*/
void    saveCoordinates (int line, int col, int num);
void    restoreCoordinates(t_puzzle *game);
void    backupHash(ROW row,unsigned int num, int col_line);
void    restoreHash(ROW row, t_hash_list** hash_table, int hash_table_size, int matrix_dim);
void    emptyStacks(void);

#endif
