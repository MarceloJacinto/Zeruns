/******************************************************************************
 * File Name: file.h
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements some functions related to the input and output files
 *****************************************************************************/
#ifndef _FILE_H
#define _FILE_H

#include <string.h>
#include "utils.h"

/*Functions to test arguments and make safety checks*/
int     testArg(char* arg);
void    getOutputName(char* nomeFicheiroIn, char ** nomeFicheiroOut);
FLAGGY  safetyChecks(int dim, int gamemode);

#endif
