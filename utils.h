/******************************************************************************
 * File Name: utils.h
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: implements some generic functions
 *****************************************************************************/
#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 32
#define Item void*

typedef enum {COL, LINE} ROW;

/*Some useful enums, to make the code easier to read*/
typedef enum {FALSE = 0, TRUE = 1} Boolean;
typedef enum {INVALID = -2, NOT_ANSWER = -1, MAYBE = 0, THE_ANSWER = 1} ANSWER;
typedef enum {IMPOSSIBLE = -1, UNSOLVED = 0, SOLVED = 1} FLAGGY;

/* Reads dim*dim numbers from the file but doesn't save them*/
void    readToDummy(FILE* fp, int dim);

/*General matrix operations*/
int     getMaxSize(FILE *fp);
int **  alocaMatriz(int dim);
void    freeMatriz(int** matriz, int dim);
void    printMatriz(FILE * fpOut, int ** matriz, int dim);

/*Power of 2 funcion using shifts*/
unsigned int powerOf2(int expoente);

/*General vector operations*/
int*    allocVector(int dim);
void    initializeVector(int* vector, int dim, int value);

/*Functions to get codes for completely fulfilled lines and columns*/
unsigned int  convertColToDecimal(int** matriz, int col, int dim);
unsigned int  convertLineToDecimal(int** matriz, int line, int dim);

/*Function to compare lines and columns*/
ANSWER    compareLineCol(int index1, int index2, ROW row, int dim, int**matriz);

/*Functions related to prime numbers*/
int       getFirstPrime(int n);
Boolean   isPrime(int n);

#endif
