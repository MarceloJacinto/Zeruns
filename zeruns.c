/******************************************************************************
 * File Name: zeruns.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements the main function
 *****************************************************************************/
/*CENAS A FAZER*/
/*Voltar a implementar aquela cena para os padrões que tinha no bolt2*/
/*varrimento hibrido - dar fix*/
/*fillLine e fillCol*/

#include "puzzle.h"
#include "solve.h"
#include "file.h"
#include "utils.h"

int main (int argc, char* argv[])
{
    /*Variable declaration*/
    FILE * fpIn = NULL;
    FILE * fpOut = NULL;
    t_puzzle * game = NULL;
    int biggestSize = -1, dim = -1, gamemode = -1;
    FLAGGY flag = UNSOLVED;
    char * InputFileName = NULL;
    char * OutputFileName = NULL;
    int first_line = 0, first_col = 0;

    /*Checks if the input file name is valid*/
    if (argc != 2) exit(0);
    if(0 == testArg(argv[1])) exit(0);
    InputFileName = argv[1];

    /*Opens the input file, reads it and creates a puzzle whose matrix has the*/
    /*size of the biggest sized valid matrix in the file. Then, closes the file*/
    fpIn  = fopen(InputFileName,"r");
    if(fpIn == NULL) exit(0);
    game = createPuzzle(fpIn);
    biggestSize = getDim(game);
    fclose(fpIn);

    /*Reopens the file. This time, to read and process all of the puzzles*/
    fpIn  = fopen(InputFileName,"r");
    if(fpIn == NULL) exit(0);

    /*Opens the output file, into which the results will be printed*/
    getOutputName(InputFileName, &OutputFileName);
    fpOut = fopen(OutputFileName, "w");

    /*Processes the input file and writes the output file*/
    while(fscanf(fpIn, "%d %d", &dim, &gamemode) == 2)
    {
        flag = UNSOLVED; /*Restores the flag to a neutral value*/
        /*Checks if everyhting is ok with the data present in the file*/
        flag = safetyChecks(dim, gamemode);
        /*Updates the puzzle structure with the data from the current game*/
        game = updatePuzzle(fpIn, game, dim, flag, gamemode, biggestSize);
        flag = getFlag(game);

        /*If by this point flag == IMPOSSIBLE, something is wrong with the*/
        /*definition of the game*/
        if (flag == IMPOSSIBLE)
        {
            /*If the game mode is wrong, we print the size of the matrix*/
            /*and the gamemode*/
            if (gamemode != 1 && gamemode != 2)
            {
                fprintf(fpOut, "%d %d \n", dim, gamemode);
                fprintf(fpOut, "\n");
            }
            else
            /*If the problem is in the size of the matrix, we print the first*/
            /*line and -1 at the end of it*/
            {
                printFirstLine(fpOut, game);
                fprintf(fpOut, "\n");
            }
        }
        else if(flag == UNSOLVED)
        {
            /*Calls the function responsible for solving the puzzle, or detecting*/
            /*if it is impossible*/
            recursiveSolver(game, first_line, first_col);
            flag = getFlag(game);
            /*If the game was solved, we print the first line and the solved puzzle*/
            /*Else, we only print the first line*/
            printFirstLine(fpOut, game);
            if(flag == IMPOSSIBLE) fprintf(fpOut, "\n");
            else if (flag == SOLVED) printMatriz(fpOut, getTable(game), dim);
        }

        /*If the current game was in mode 2, fress all of the elements from the*/
        /*hash tables, except from the tables themselves*/
        if(gamemode == 2)
        {
            freeHashTableElements(getHashLinhas(game), getHashSize(game));
            freeHashTableElements(getHashColunas(game), getHashSize(game));
        }
    }
    /*Closes both the input and the output files*/
    if(fpIn != NULL) fclose(fpIn);
    if(fpOut != NULL) fclose(fpOut);

    /*Frees all the memory allocated*/
    if (OutputFileName != NULL) free(OutputFileName);
    freePuzzle(game, biggestSize);
    return 0;
}
