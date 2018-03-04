/******************************************************************************
 * File Name: file.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements some functions related to the input and output files
 *****************************************************************************/
#include "file.h"

/*****************************************************************************
* testArg
* Arguments: char* arg
* Returns: 1 if the string ends in .puz, 0 otherwise
* Side-Effects: none
*****************************************************************************/
int testArg(char* arg) {
    char str[5] = ".puz";
    int i = 0, n;

    n = strlen(arg);
    for(i = 0; i<4; i++)
    {
        if(arg[n-i] != str[4-i])
        {
            return 0;
        }
    }
    return 1;
}

/*****************************************************************************
* getOutputName
* Arguments: char* nomeFicheiroIn, nomeFicheiroOut
* Returns: void
* Side-Effects: Allocates memory for the output file's name and writes
* the output name in the string nomeFicheiroOut
*****************************************************************************/
void getOutputName(char* nomeFicheiroIn, char ** nomeFicheiroOut)
{
    char* aux = NULL;
    int n = 0;
    int i = 0;

    /*Allocates memory with enought size for our ouput file's name*/
    aux = (char *)malloc((strlen(nomeFicheiroIn) - 4 + strlen(".sol") + 1) * sizeof(char));
    if(aux == NULL) exit(0);

    for (i = 0; i < (strlen(nomeFicheiroIn) - 4 + strlen(".sol") + 1); i++) aux[i] = 0;

    n = strlen(nomeFicheiroIn);
    for(i = 0; i < n-4; i++) aux[i]=nomeFicheiroIn[i];

    strcat(aux, ".sol");
    *nomeFicheiroOut = aux;
    return;
}

/*****************************************************************************
* safetyChecks
* Arguments: int dim, int gamemode
* Returns: IMPOSSIBLE if either dim or gamemode is invalid, UNSOLVED otherwise
* Side-Effects: none
*****************************************************************************/
FLAGGY safetyChecks(int dim, int gamemode)
{
    if (dim < 2 || dim % 2 != 0) return IMPOSSIBLE;
    if (gamemode != 1 && gamemode != 2) return IMPOSSIBLE;
    return UNSOLVED;
}
