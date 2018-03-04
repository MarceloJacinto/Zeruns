/******************************************************************************
 * File Name: utils.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements some generic functions
 *****************************************************************************/
#include "utils.h"

/*****************************************************************************
* readToDummy
* Arguments: FILE* fp, int dim
* Side-Effects: Reads dim*dim numbers from the file but doesn't save them
*****************************************************************************/
void readToDummy(FILE* fp, int dim)
{
    int i = 0, j = 0, ret = 0, dummy = 0;

    for(i=0; i<dim; i++)
    {
        for(j=0; j<dim; j++)
        {
            ret = fscanf(fp, "%d", &dummy);
            if(ret != 1) exit(0);
        }
    }
}

/*****************************************************************************
* getMaxSize
* Arguments: FILE* fp
* Returns: int max - size of the biggest valid (correct dimension and mode) matrix
* Side-Effects: Scanf all the file, reading its elements, to discover the size
*               of the biggest valid matrix
*****************************************************************************/
int getMaxSize(FILE *fp)
{
    int dim, dummy, i, j, max=0, mode = 0, ret = 0;

    while(fscanf(fp, "%d %d", &dim, &mode) == 2)
    {
        for(i = 0; i < dim; i++)
        {
            for(j = 0; j < dim; j++)
            {
                ret = fscanf(fp, "%d", &dummy);
                if (ret == 0) exit(0);
                if(dim>max)
                {
                    if(mode == 1 || mode == 2)
                    {
                        max = dim;
                    }
                }
            }
        }

    }
    return max;
}

/*****************************************************************************
* powerOf2
* Argument: int expoente
* Returns: int 2^expoente
*****************************************************************************/
unsigned int powerOf2(int expoente)
{
    return 1 << expoente;
}

/*****************************************************************************
* alocaMatriz
* Arguments: int dim - size of the matrix
* Returns: int** matriz
* Side-Effects: Allocates memory for a size 'dim' matrix
*****************************************************************************/
int** alocaMatriz(int dim)
{
    int i=0;
    int** matriz = NULL;

    matriz = (int**)malloc(sizeof(int*)*dim);
    if(matriz == NULL) exit(0);

    for(i=0; i<dim; i++)
    {
        matriz[i] = (int*)malloc(sizeof(int)*dim);
        if(matriz[i] == NULL) exit(0);
    }

    return matriz;
}

/*****************************************************************************
* freeMatriz
* Arguments: int dim - size of the matrix
* Returns: void
* Side-Effects: Frees a size 'dim' matrix
*****************************************************************************/
void freeMatriz(int** matriz, int dim)
{
    int i = 0;
    if(matriz == NULL) return;
    for(i = 0; i < dim; i++) free(matriz[i]);
    free(matriz);
}

/*****************************************************************************
* printMatriz
* Arguments: FILE* fpOut, int** matriz, int dim - size of the matrix
* Returns: void
* Side-Effects: Prints the matrix 'matriz' into the file pointed by 'fpOut'
*****************************************************************************/
void printMatriz(FILE * fpOut, int ** matriz, int dim)
{
    int i, j;

    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            fprintf(fpOut, "%d ", matriz[i][j]);
        }
        fprintf(fpOut, "\n");
    }

    fprintf(fpOut, "\n");
    return;
}

/*****************************************************************************
* allocVector
* Arguments: dim - size of the vector
* Returns: int* vector
* Side-Effects: Allocates a size 'dim' vector
*****************************************************************************/
int* allocVector(int dim)
{
    int * newVector = NULL;

    newVector = (int*)malloc(sizeof(int)*dim);
    if(newVector == NULL) exit(0);

    return newVector;
}

/*****************************************************************************
* initializeVector
* Arguments: int* vector, int dim - size of the vector, int value
* Returns: void
* Side-Effects: Initializes all of the positions of a 'dim' size vector with the
* number 'value'
*****************************************************************************/
void initializeVector(int* vector, int dim, int value)
{
    int i = 0;
    for(i=0; i<dim; i++) vector[i] = value;
}

/*****************************************************************************
* convertLineToDecimal
* Arguments: int** matriz, int line - index of the line to be converted
* int dim - size of the matrix
* Returns: Convertion from binary to decimal of the value present in the
* first MAX_SIZE bits of the line 'line'of the matrix
* Side-Effects: none
*****************************************************************************/
unsigned int convertLineToDecimal(int** matriz, int line, int dim)
{
    int i = 0, sum = 0;
    int max = -1;

    if(dim>MAX_SIZE) max = MAX_SIZE;
    else max = dim;

    for(i=0; i<max; i++)
    {
        if(matriz[line][i] == 1)
        {
            sum += powerOf2(i);
        }
    }

    return sum;
}

/*****************************************************************************
* convertColToDecimal
* Arguments: int** matriz, int col - index of the column to be converted
* int dim - size of the matrix
* Returns: Convertion from binary to decimal of the value present in the
* first MAX_SIZE bits of the column 'col'of the matrix
* Side-Effects: none
*****************************************************************************/
unsigned int convertColToDecimal(int** matriz, int col, int dim)
{
    int i = 0, sum = 0;
    int max = -1;

    if(dim>MAX_SIZE) max = MAX_SIZE;
    else max = dim;

    for(i=0; i<max; i++)
    {
        if(matriz[i][col] == 1)
        {
            sum += powerOf2(i);
        }
    }

    return sum;
}

/*****************************************************************************
* getFirstPrime
* Arguments: int n
* Returns: First prime number bigger or equal than n
* Side-Effects: none
*****************************************************************************/
int getFirstPrime(int n)
{
    while(isPrime(n) != TRUE) n++;
    return n;
}

/*****************************************************************************
* isPrime
* Arguments: int n
* Returns: TRUE if n is prime, FALSE otherwise
*****************************************************************************/
Boolean isPrime(int n)
{
    int i = 0, w = 0;

    if(n == 2) return TRUE;
    if(n == 3) return TRUE;
    if(n%2 == 0) return FALSE;
    if(n%3 == 0) return FALSE;

    i = 5;
    w = 2;

    while(i*i <= n)
    {
        if(n%i == 0) return FALSE;
        i+=w;
        w = 6 -w;
    }

    return TRUE;
}

/*****************************************************************************
* compareLineCol
* Arguments: index1, index2, row - tells us if we are comparing lines
* or columns, dim - dimension and matrix
* Returns: INVALID if there are two lines or columns equal to each other,
* MAYBE otherwise
*****************************************************************************/
ANSWER compareLineCol(int index1, int index2, ROW row, int dim, int**matriz)
{
    int i = 0;

    for(i=MAX_SIZE-1; i<dim; i++)
    {
        if (row == LINE)
        {
            if (matriz[index1][i] != matriz[index2][i]) return MAYBE;
        }
        else if (row == COL)
        {
            if (matriz[i][index1] != matriz[i][index2]) return MAYBE;
        }
    }

    return INVALID;
}
