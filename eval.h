/******************************************************************************
 * File Name: eval.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.2
 * COMMENTS: Implements the functions that evaluate whether a certain position
 * of the matriz should be filled with 1 or 0, or if we don't have enough
 * information to decide it
 *****************************************************************************/
#ifndef _EVAL_H
#define _EVAL_H
#include "utils.h"
#include "puzzle.h"

/*Functions related to the evaluation of the line parity, column parity*/
/*and adjacency rules*/
int checkConsecutive(t_puzzle* game, int binary, int line, int col);
int checkLineParity(t_puzzle* game, int binary, int line);
int checkColumnParity(t_puzzle* game, int binary, int col);

/*Function that makes a decision based on the results from the other*/
/*tree functions*/
int checkSolution(t_puzzle* game, int line, int col, int binary);

#endif
