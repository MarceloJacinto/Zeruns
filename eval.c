/******************************************************************************
 * File Name: eval.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.2
 * COMMENTS: Implements the functions that evaluate whether a certain position
 * of the matriz should be filled with 1 or 0, or if we don't have enough
 * information to decide it
 *****************************************************************************/
#include "eval.h"

/*****************************************************************************
* checkConsecutive
* Arguments: t_puzzle* game, int binary - number we want to fill the position
* with
* int line, col - coordinates of the position we are trying to fill
* Returns: NOT_ANSWER if we can't fill these coordinates with num
* ANSWER if we must fille these coordinates with num
* INVALID if there are discordant evaluations
* MAYBE if we can't get any conclusions
* Side-Effects: none
*****************************************************************************/
ANSWER checkConsecutive(t_puzzle* game, int binary, int line, int col)
{
    int size = getDim(game);
    int** table = getTable(game);
    int the_answer_sum = 0; /*ANSWER counter*/
    int not_answer_sum = 0; /*NOT_ANSWER counter*/

    /*For all the following ifs, the outer if, certifies that we aren't going*/
    /*out of the matrix. The second outer if verifies if the adjacent positions*/
    /*are filled with 0s or 1s, as adjacent 9s won't let us draw any conclusions.*/
    /*The third outer if checks if the adjacent numbers are equal to each other*/
    /*because if they are different we won't be able to conclude anything.*/
    /*The inner if decides if our number is the correct or the wrong answer*/
    /*to fill these coordinates with.*/
    /*the_answer_sum and not_answer_sum are counters that will save the number*/
    /*of times we get and affirmative or negative answer, respectively.*/

    if(line != 0 && line != size-1)
    {
        if(table[line+1][col] != 9 && table[line-1][col] != 9)
        {
            if(table[line+1][col] == table[line-1][col])
            {
                if(table[line+1][col] == table[line][col]) not_answer_sum++;
                else the_answer_sum++;
            }
        }
    }

    if(col != 0 && col != size-1)
    {
        if(table[line][col-1] != 9 && table[line][col+1] != 9)
        {
            if(table[line][col+1] == table[line][col-1])
            {
                if(table[line][col+1] == table[line][col]) not_answer_sum++;
                else the_answer_sum++;
            }
        }
    }

    if(col > 1)
    {
        if(table[line][col-1] != 9 && table[line][col-2] != 9)
        {
            if(table[line][col-1] == table[line][col-2])
            {
                if(table[line][col-1] == table[line][col]) not_answer_sum++;
                else the_answer_sum++;
            }
        }
    }

    if(col < size-2)
    {
        if(table[line][col+1] != 9 && table[line][col+2] != 9)
        {
            if(table[line][col+1] == table[line][col+2])
            {
                if(table[line][col+1] == table[line][col]) not_answer_sum++;
                else the_answer_sum++;
            }
        }
    }

    if(line > 1)
    {
        if(table[line-2][col] != 9 && table[line-1][col] != 9)
        {
            if(table[line-2][col] == table[line-1][col])
            {
                if(table[line-1][col] == table[line][col]) not_answer_sum++;
                else the_answer_sum++;
            }
        }
    }

    if(line < size-2)
    {
        if(table[line+1][col] != 9 && table[line+2][col] != 9)
        {
            if(table[line+1][col] == table[line+2][col])
            {
                if(table[line+1][col] == table[line][col]) not_answer_sum++;
                else the_answer_sum++;
            }
        }
    }

    /*If we got discordant answers, the puzzle is INVALID*/
    if(the_answer_sum != 0 && not_answer_sum != 0) return INVALID;
    /*If we only got positive or negative answers, we return THE_ANSWER*/
    /*or NOT_ANSWER accordingly.*/
    else if(the_answer_sum != 0) return THE_ANSWER;
    else if(not_answer_sum != 0) return NOT_ANSWER;
    /*If we didn't get any answers, we return MAYBE*/
    return MAYBE;
}
/*****************************************************************************
* checkLineParity
* Arguments: t_puzzle* game
* int binary - number we want to fill the position with
* int line - line in which we are trying to fill a position
* Returns: NOT_ANSWER if we can't fill these coordinates with num
* ANSWER if we must fille these coordinates with num
* MAYBE if we can't get any conclusions
* Side-Effects: none
*****************************************************************************/
ANSWER checkLineParity(t_puzzle* game, int binary, int line)
{
    int dim = getDim(game);
    int* zeros_lines = getZerosLines(game);
    int* vetor_linhas = getVetorLinhas(game);
    int zeros = zeros_lines[line];
    int ones = (dim - zeros - vetor_linhas[line]);

    /*Based on the auxiliar vectors we know the exact number of 0s, 1s and 9s*/
    /*in each line*/

    /*If our binary is 1 and the number of 0s is half of the size of the line*/
    /*then we return THE_ANSWER. Else if the number of 1s is half of the size*/
    /*of the line, we return NOT_ANSWER*/
    /*The same applies for 0*/
    if(binary == 1)
    {
        if(zeros == (dim/2)) return THE_ANSWER;
        else if(ones == (dim/2)) return NOT_ANSWER;
    }
    if(binary == 0)
    {
        if(ones == (dim/2)) return THE_ANSWER;
        else if(zeros == (dim/2)) return NOT_ANSWER;
    }
    return MAYBE;
}

/*****************************************************************************
* checkColParity
* Arguments: t_puzzle* game
* int binary - number we want to fill the position with
* int columns - columns in which we are trying to fill a position
* Returns: NOT_ANSWER if we can't fill these coordinates with num
* ANSWER if we must fille these coordinates with num
* MAYBE if we can't get any conclusions
* Side-Effects: none
*****************************************************************************/
ANSWER checkColumnParity(t_puzzle* game, int binary, int col)
{
    int dim = getDim(game);
    int* zeros_cols = getZerosCols(game);
    int* vetor_colunas = getVetorColunas(game);
    int zeros = zeros_cols[col];
    int ones = (dim - zeros - vetor_colunas[col]);

    /*The same as for checkLineParity, but applied to a column*/
    if(binary == 1)
    {
        if(zeros == (dim/2)) return THE_ANSWER;
        else if(ones == (dim/2)) return NOT_ANSWER;
    }
    if(binary == 0)
    {
        if(ones == (dim/2)) return THE_ANSWER;
        else if(zeros == (dim/2)) return NOT_ANSWER;
    }
    return MAYBE;
}

/*****************************************************************************
* checkSolution
* Arguments: t_puzzle* game
* int line, col - coordinates we are testing
* int binary - number we want to test
* Returns: NOT_ANSWER if we can't fill these coordinates with num
* ANSWER if we must fille these coordinates with num
* INVALID if there are discordant evaluations
* MAYBE if we can't get any conclusions
* Side-Effects: none
*****************************************************************************/
ANSWER checkSolution (t_puzzle* game, int line, int col, int binary)
{
    ANSWER adjacent = MAYBE;
    ANSWER colPar = MAYBE;
    ANSWER linePar = MAYBE;
    int not_answer_sum = 0, the_answer_sum = 0;
    int** matriz = getTable(game);

    /*Makes sure that we aren't testing a position that is already filled*/
    if(matriz[line][col] != 9) return MAYBE;
    matriz[line][col] = binary;

    /*Gets the evaluation from all the rules*/
    adjacent = checkConsecutive(game, binary, line, col);
    if(adjacent == INVALID) return INVALID;
    else if(adjacent == THE_ANSWER) the_answer_sum++;
    else if(adjacent == NOT_ANSWER) not_answer_sum++;

    linePar = checkLineParity(game, binary, line);
    if(linePar == THE_ANSWER) the_answer_sum++;
    else if(linePar == NOT_ANSWER) not_answer_sum++;

    colPar = checkColumnParity(game, binary, col);
    if(colPar == THE_ANSWER) the_answer_sum++;
    else if(colPar == NOT_ANSWER) not_answer_sum++;

    /*Restores the original number, that will always be 9 to its position*/
    matriz[line][col] = 9;

    /*If there are discordant evaluations, then the current puzzle is INVALID*/
    if(the_answer_sum != 0 && not_answer_sum != 0) return INVALID;
    /*If we only got positive or negative answers, we return THE_ANSWER*/
    /*or NOT_ANSWER accordingly.*/
    else if(the_answer_sum != 0) return THE_ANSWER;
    else if(not_answer_sum != 0) return NOT_ANSWER;
    /*If we didn't get any answers, we return MAYBE*/
    return MAYBE;
}
