/******************************************************************************
 * File Name: puzzle.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-11-29
 * Revision:  v1.0
 * COMMENTS: Implements the type t_puzzle and the functions to operate it
 *****************************************************************************/
#include "puzzle.h"

/*Declaration of the datatype*/
struct _t_puzzle {
    int **table;                  /*puzzle matrix*/
    int dim;                      /*Dimension of the puzzle matrix*/
    FLAGGY flag;                  /*Flag to describe the state of the puzzle:*/
                                  /*SOLVED, UNSOLVED or IMPOSSIBLE*/
    int gamemode;                 /*puzzle mode of the current puzzle*/
    int hash_size;                /*Size of the hash tables*/
    t_hash_list ** hash_colunas;  /*Hash table for the columns*/
    t_hash_list ** hash_linhas;   /*Hash table for the lines*/
    int* vetor_linhas;            /*Vector that keeps track of the number of nines*/
                                  /*in each line*/
    int* vetor_colunas;           /*Same as vetor_linhas, but for the columns*/
    int nines;                    /*Saves the number of nines in the board*/
    int* zeros_lines;             /*Saves the number of zeros in each line*/
    int* zeros_cols;              /*Same as zeros_lines, but for the columns*/
};

/*****************************************************************************
* createPuzzle
* Arguments: FILE* fp, int size - size of the puzzle matrix
* Returns: (t_puzzle *) puzzle
* Side-Effects: Allocates memory for an element of the type t_puzzle*
*****************************************************************************/
t_puzzle * createPuzzle(FILE * fp) {
    t_puzzle * newPuzzle = NULL;
    int dim = 0;

    newPuzzle = (t_puzzle*) malloc(sizeof(t_puzzle));
    if(newPuzzle == NULL) exit(0);

    dim =  getMaxSize(fp);
    /*If the matriz size if valid(equal or bigger than 2), we allocate*/
    /*everything we need. Otherwise, we initialize all the pointers with NULL*/
    if(dim >= 2)
    {
		    newPuzzle->table = alocaMatriz(dim);
        newPuzzle->flag = UNSOLVED;
        newPuzzle->hash_size = chooseHashSize(dim);
        newPuzzle->hash_linhas = createHashTable(newPuzzle->hash_size);
        newPuzzle->hash_colunas = createHashTable(newPuzzle->hash_size);
        newPuzzle->vetor_colunas = allocVector(dim);
        newPuzzle->vetor_linhas = allocVector(dim);
        newPuzzle->zeros_cols = allocVector(dim);
        newPuzzle->zeros_lines = allocVector(dim);
        newPuzzle->dim = dim;
        newPuzzle->nines = -1;
	  }
    else
    {
        newPuzzle->table = NULL;
        newPuzzle->hash_linhas = NULL;
        newPuzzle->hash_colunas = NULL;
        newPuzzle->vetor_linhas = NULL;
        newPuzzle->vetor_colunas = NULL;
        newPuzzle->zeros_lines = NULL;
        newPuzzle->zeros_cols = NULL;
    }
    return newPuzzle;
}

/*****************************************************************************
* freePuzzle
* Arguments: t_puzzle* puzzle, int matriz_size
* Returns: void
* Side-Effects: Fress a previously allocated puzzle
*****************************************************************************/
void freePuzzle(t_puzzle *puzzle, int matriz_size) {
    if (puzzle == NULL) return;
    if (puzzle->table != NULL) freeMatriz(puzzle->table, matriz_size);
    if(puzzle->hash_linhas != NULL) free(puzzle->hash_linhas);
    if(puzzle->hash_colunas != NULL) free(puzzle->hash_colunas);
    if(puzzle->vetor_colunas != NULL) free(puzzle->vetor_colunas);
    if(puzzle->vetor_linhas != NULL) free(puzzle->vetor_linhas);
    if(puzzle->zeros_cols != NULL) free(puzzle->zeros_cols);
    if(puzzle->zeros_lines != NULL) free(puzzle->zeros_lines);
    free(puzzle);
}

/*****************************************************************************
* getZerosLine
* Arguments: t_puzzle* puzzle
* Returns: puzzle->zeros_lines
* Side-Effects: none
*****************************************************************************/
int* getZerosLines(t_puzzle* puzzle)
{
    return puzzle->zeros_lines;
}

/*****************************************************************************
* getZerosCols
* Arguments: t_puzzle* puzzle
* Returns: puzzle->zeros_cols
* Side-Effects: none
*****************************************************************************/
int* getZerosCols(t_puzzle* puzzle)
{
    return puzzle->zeros_cols;
}

/*****************************************************************************
* getNines
* Arguments: t_puzzle* puzzle
* Returns: puzzle->nines
* Side-Effects: none
*****************************************************************************/
int getNines(t_puzzle* puzzle)
{
    return puzzle->nines;
}

/*****************************************************************************
* getDim
* Arguments: t_puzzle* puzzle
* Returns: puzzle->dim
* Side-Effects: none
*****************************************************************************/
int getDim(t_puzzle *puzzle)
{
    return puzzle->dim;
}

/*****************************************************************************
* getFlag
* Arguments: t_puzzle* puzzle
* Returns: puzzle->flag
* Side-Effects: none
*****************************************************************************/
FLAGGY getFlag(t_puzzle *puzzle)
{
    return puzzle->flag;
}

/*****************************************************************************
* getTable
* Arguments: t_puzzle* puzzle
* Returns: puzzle->table
* Side-Effects: none
*****************************************************************************/
int ** getTable(t_puzzle *puzzle) {
    return puzzle->table;
}

/*****************************************************************************
* getGamemode
* Arguments: t_puzzle* puzzle
* Returns: gamemode of a puzzle
* Side-Effects: none
*****************************************************************************/
int getGamemode (t_puzzle * puzzle)
{
    return puzzle->gamemode;
}

/*****************************************************************************
* getHashSize
* Arguments: t_puzzle* puzzle
* Returns: Hashsize of a puzzle
* Side-Effects: none
*****************************************************************************/
int getHashSize(t_puzzle * puzzle) {
    return puzzle->hash_size;
}

/*****************************************************************************
* getVetorLinhas
* Arguments: t_puzzle* puzzle
* Returns: vetor_linhas of a puzzle
* Side-Effects: none
*****************************************************************************/
int* getVetorLinhas (t_puzzle * puzzle) {
    return puzzle->vetor_linhas;
}

/*****************************************************************************
* getVetorColunas
* Arguments: t_puzzle* puzzle
* Returns: vetor_colunas of a puzzle
* Side-Effects: none
*****************************************************************************/
int* getVetorColunas (t_puzzle * puzzle) {
    return puzzle->vetor_colunas;
}

/*****************************************************************************
* getHashColunas
* Arguments: t_puzzle* puzzle
* Returns: hash_colunas of a puzzle
* Side-Effects: none
*****************************************************************************/
t_hash_list** getHashColunas (t_puzzle * puzzle) {
    return puzzle->hash_colunas;
}

/*****************************************************************************
* getHashLinhas
* Arguments: t_puzzle* puzzle
* Returns: hash_linhas of a puzzle
* Side-Effects: none
*****************************************************************************/
t_hash_list** getHashLinhas (t_puzzle * puzzle) {
    return puzzle->hash_linhas;
}

/*****************************************************************************
* updatePuzzle
* Arguments: FILE* fp, t_puzzle* puzzle, int dim - size of the matrix
* int flag , int gamemode
* int biggestSize
* Returns: t_puzzle* puzzle
* Side-Effects: Changes some info about the puzzle
*****************************************************************************/
t_puzzle * updatePuzzle(FILE * fpIn, t_puzzle * puzzle, int dim, FLAGGY flag, int gamemode, int biggestSize)
{
    int i, j, num;
    unsigned int lineDecimal;
    unsigned int colDecimal;

    if(dim > biggestSize)
    {
        puzzle->flag = IMPOSSIBLE;
        readToDummy(fpIn, dim);
        return puzzle;
    }

    /*Initializes the vectors and the hash tables*/
    initializeVector(puzzle->vetor_linhas, dim, dim);
    initializeVector(puzzle->vetor_colunas, dim, dim);
    initializeVector(puzzle->zeros_lines, dim, 0);
    initializeVector(puzzle->zeros_cols, dim, 0);
    if(gamemode == 2)
    {
        initializeHashTable(puzzle->hash_linhas, puzzle->hash_size);
        initializeHashTable(puzzle->hash_colunas, puzzle->hash_size);
    }
    puzzle->nines = dim*dim;

    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            if(fscanf(fpIn, "%d", &num) != 1) exit(0); /*READING ERROR*/
            if(num == 1 || num == 0)
            {
                if(num == 0)
                {
                    puzzle->zeros_cols[j]++;
                    puzzle->zeros_lines[i]++;
                }
                puzzle->nines--;
                puzzle->vetor_linhas[i]--;
                puzzle->vetor_colunas[j]--;
                if(gamemode == 2)
                {
                    if(puzzle->vetor_linhas[i] == 0)
                    {
                        lineDecimal = convertLineToDecimal(puzzle->table, i, dim);
                        if(checkRepetition(getHashLinhas(puzzle), lineDecimal, getHashSize(puzzle), i, LINE, puzzle->table,dim) == INVALID) flag = IMPOSSIBLE;
                    }
                    if(puzzle->vetor_colunas[j] == 0)
                    {
                        colDecimal = convertColToDecimal(puzzle->table, j, dim);
                        if(checkRepetition(getHashColunas(puzzle), colDecimal, getHashSize(puzzle), j, COL, puzzle->table,dim) == INVALID) flag = IMPOSSIBLE;
                    }
                }
            }
            else if(num != 9) flag = IMPOSSIBLE; /*THE PUZZLE IS IMPOSSIBLE*/
            puzzle->table[i][j] = num;
        }
    }

    puzzle->dim = dim;
    puzzle->flag = flag;
    puzzle->gamemode = gamemode;
    return puzzle;
}


/*****************************************************************************
* changeFlag
* Arguments: t_puzzle* puzzle, FLAGGY flag
* Returns: void
* Side-Effects: Changes the value of puzzle->flag to flag
*****************************************************************************/
void changeFlag (t_puzzle* puzzle, FLAGGY flag)
{
    puzzle->flag = flag;
}

/*****************************************************************************
* incNines
* Arguments: t_puzzle* puzzle
* Returns: void
* Side-Effects: Increments puzzle->nines
*****************************************************************************/
void incNines(t_puzzle* puzzle)
{
    puzzle->nines++;
}

/*****************************************************************************
* decNines
* Arguments: t_puzzle* puzzle
* Returns: void
* Side-Effects: Decrements puzzle->nines
*****************************************************************************/
void decNines(t_puzzle* puzzle)
{
    puzzle->nines--;
}

/*****************************************************************************
* printFirstLine
* Arguments: t_puzzle* puzzle
* Returns: void
* Side-Effects: Prints the first line to fpOut
*****************************************************************************/
void printFirstLine(FILE * fpOut, t_puzzle * puzzle) {
    FLAGGY flag = getFlag(puzzle);

    if (fpOut == NULL) return;

    fprintf(fpOut, "%d %d ", getDim(puzzle), getGamemode(puzzle));

    if (flag == IMPOSSIBLE) fprintf(fpOut, "-1 \n");
    else if (flag == SOLVED) fprintf(fpOut, "1 \n");

    return;
}
