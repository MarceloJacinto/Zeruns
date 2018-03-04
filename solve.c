/******************************************************************************
 * File Name: solve.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-11-16
 * Revision:  v1.0
 * COMMENTS: Implements the most important functions needed to solve the puzzle
 *****************************************************************************/
#include "solve.h"
#include "stack.h"

/*Stacks used to backtrack the changes made to the matrix*/
stack_node * stack;

/*Stacks used for the backtrack of the hash tables*/
stack_node * stack_hash_colunas;
stack_node * stack_hash_linhas;

/*****************************************************************************
 * recursiveSolver
 * Arguments: t_puzzle* game
 * int line, col - coordinates
 * dim - size of the matrix
 * Returns: t_puzzle* game
 * Side-Effects: Solves the puzzle, or determines that it is impossible
 *****************************************************************************/
void recursiveSolver(t_puzzle* game, int line, int col)
{
    /*Gets the table, its size and the gamemode from the struct*/
    int** matriz = getTable(game);
    int dim = getDim(game);
    int mode = getGamemode(game);

    /*Gets the hash tables from the struct*/
    t_hash_list ** hash_linhas = getHashLinhas(game);
    t_hash_list ** hash_colunas = getHashColunas(game);

    /*Initialization of some variables*/
    int line_varr = line, col_varr = col;
    FLAGGY game_state = UNSOLVED;
    int test1 = 0, test2 = 1;
    int line_aux = line, col_aux = col;

    /*Initialization of the pointers that will allow us to backtrack*/
    /*later, in case we need it*/
    stack_node * stack_time_machine = NULL;
    stack_node * hash_col_time_machine = NULL;
    stack_node * hash_line_time_machine = NULL;

    /*Saves the pointers to the top of the stack before sweeping the board*/
    /*to fill it using the rules*/
    stack_time_machine = stack;
    hash_col_time_machine = stack_hash_colunas;
    hash_line_time_machine = stack_hash_linhas;

    /*Based on the number of nines in the board, calls a sweeping fucntion*/
    /*that searches directly for nines, or for 1s or 0s with adjacent nines*/
    if(getNines(game) > 0.5*dim*dim)
    {
        /*Discovers the best positon to start sweeping the board*/
        getFirstPositionVarrimento(game, &line_varr, &col_varr);
        varrimento(game, line_varr, col_varr, 1);
    }
    else
    {
        /*Searches for the coordinates of the first nine in the table*/
        game_state = searchNextNine(game, &line_aux, &col_aux);
        /*If there aren't any more 9s, the game is SOLVED. In this case*/
        /*we free everything that was in the stacks and returns*/
        if(game_state == SOLVED)
        {
            changeFlag(game, SOLVED);
            emptyStacks();
            return;
        }
        /*If there are still 9s, we call the function to sweep the board*/
        varrimento9(game, line_aux, col_aux, 1);
    }
    game_state = getFlag(game);

    /*If varrimento or varrimento9 dectect impossibilties, then the game is IMPOSSIBLE*/
    /*In this case we backtrack with the help of the stack pointers we saved earlier*/
    if(game_state == IMPOSSIBLE)
    {
        while(stack != stack_time_machine) restoreCoordinates(game);
        while(stack_hash_linhas != hash_line_time_machine) restoreHash(LINE, hash_linhas, getHashSize(game), dim);
        while(stack_hash_colunas != hash_col_time_machine) restoreHash(COL, hash_colunas, getHashSize(game), dim);
        return;
    }

    /*After sweeping the board, because our sweeping functions are not exhaustive*/
    /*we need to check if we aren't trying to fill a position that is already*/
    /*mandatory by the rules*/
    do
    {
        /*First, we search for a 9. If we can't find it, the game is already SOLVED*/
        game_state = searchNextNine(game, &line, &col);
        if(game_state == SOLVED)
        {
            changeFlag(game, SOLVED);
            emptyStacks();
            return;
        }

        /*If we find a 9, we call auxVarr, that tries to fill these coordinates*/
        /*using the rules. If it is impossible to fill the position with either*/
        /*1 or 0, the game is impossible*/
        auxVarr(game, line, col);
        game_state = getFlag(game);
        if(game_state == IMPOSSIBLE)
        {
            while(stack != stack_time_machine) restoreCoordinates(game);
            while(stack_hash_linhas != hash_line_time_machine) restoreHash(LINE, hash_linhas, getHashSize(game), dim);
            while(stack_hash_colunas != hash_col_time_machine) restoreHash(COL, hash_colunas, getHashSize(game), dim);
            return;
        }
    }while(matriz[line][col] != 9);

    /*If mode == 1, we choose try to fill this position with a the same number*/
    /*as the upper positon (if we aren't in the first line) is filled with,*/
    /*as this will likely make it mandatory to fill the position under it*/
    /*with 0 or 1*/
    if(mode == 1)
    {
        if(line!=0) test1 = matriz[line-1][col];
        else if(line==0 && col != 0) test1 = matriz[line][col-1];
    }
    /*If mode == 2, we can't apply this logic, as it will produce equal line*/
    /*so, after testing some options, we chose to shift the previous line*/
    else if(mode == 2)
    {
        if(line != 0)
        {
            if(col == 0) test1 = matriz[line-1][dim-1];
            else test1 = matriz[line-1][col-1];
        }

    }

    /*In the previous line we obtained the first number to test. The next number*/
    /*to test will be the opposite of it*/
    test2 = !test1;

    /*Tries to fill the position with the number test1*/
    tryNumber(game, line, col, test1);
    game_state = getFlag(game);
    /*If the game was solved returns*/
    if(game_state == SOLVED) return;

    /*If it was impossible to solve the puzzle for the first number, we change*/
    /*the value of the flag in the struct to UNSOLVED*/
    changeFlag(game, UNSOLVED);

    /*Then tries the opposite number*/
    tryNumber(game, line, col, test2);
    game_state = getFlag(game);

    /*If the game was impossible for both numbers we backtrack to the game state*/
    /*before the sweep of the board*/
     if(game_state == IMPOSSIBLE)
     {
         while(stack != stack_time_machine) restoreCoordinates(game);
         while(stack_hash_linhas != hash_line_time_machine) restoreHash(LINE, hash_linhas, getHashSize(game), dim);
         while(stack_hash_colunas != hash_col_time_machine) restoreHash(COL, hash_colunas, getHashSize(game), dim);
     }
     /*By this point, it is supposed that the stacks will have already been freed*/
     /*but for saftey we call the function that frees them again*/
     else if(game_state == SOLVED) emptyStacks();

    return;
}

/*****************************************************************************
 * varrimento
 * Arguments: t_puzzle* puzzle
 * int line, col - coordinates in the matrix
 * int dim - size of the matrix
 * int ramo - flag that tells us if we are in the main branch of the recursive
 * function or in one of its ramifications. 1 for the first case, 0 for the
 * second
 * Returns: void
 * Side-Effects: Looks to the adjacent positions and if they are filled with
 * 9 calls the function auxVarr for them. At the end, if we are in the main
 * branch of the function, it calls itself for the next position filled
 * with a one or a 0
 *****************************************************************************/
void varrimento(t_puzzle* game, int line, int col, int ramo)
{
    int** matriz = getTable(game);
    int next_line = line, next_col = col, dim = getDim(game);

    if(line != 0 && matriz[line-1][col] == 9)
    {
        auxVarr(game, line - 1, col);
        if(getFlag(game) == IMPOSSIBLE) return;
    }

    if(line != dim-1 && matriz[line+1][col] == 9)
    {
        auxVarr(game, line + 1, col);
        if(getFlag(game) == IMPOSSIBLE) return;
    }

    if(col != 0 && matriz[line][col-1] == 9)
    {
        auxVarr(game, line, col-1);
        if(getFlag(game) == IMPOSSIBLE) return;
    }

    if(col != dim-1 && matriz[line][col+1] == 9)
    {
        auxVarr(game, line, col+1);
        if(getFlag(game) == IMPOSSIBLE) return;
    }

    /*searchNotNine returns 0 if it finds a 1 or a 0 and updates next_line*/
    /*and next_col with the coordinates of the next 0 or 1 in the matrix*/
    if(ramo == 1 && searchNotNine(game, &next_line, &next_col) == 0)
    {
        /*Calls varrimento again with ramo = 1, because we are still in*/
        /*the main branch*/
        varrimento(game,next_line, next_col, 1);
    }
}

/*****************************************************************************
 * auxVarr
 * Arguments: t_puzzle* game
 * int line_test, col_test - coordinates in the matrix
 * Returns: void
 * Side-Effects: Fills the game table in the position [line_test][col_test]
 * with a 1 or 0 if it is mandatory to do so. Otherwise leaves a 9 there.
 *****************************************************************************/
void auxVarr(t_puzzle* game, int line_test, int col_test)
{
    int** matriz = getTable(game);
    int mode = getGamemode(game);
    int *vetor_linhas = getVetorLinhas(game), *vetor_colunas = getVetorColunas(game);
    int *zeros_cols = getZerosCols(game), *zeros_lines = getZerosLines(game);
    ANSWER flag = MAYBE;

    /*First we test to fill the position with a 0*/
    flag = checkSolution(game, line_test, col_test, 0);

    /*If checkSolution returned INVALID, it's because we can't fill this*/
    /*coordinate with either 0 or 1, so the game is IMPOSSIBLE and we return*/
    if(flag == INVALID)
    {
        changeFlag(game, IMPOSSIBLE);
        return;
    }

    /*If 0 is the only number we can put in this position, we fill it with 0*/
    /*and decrement or increment some auxiliar vectors*/
    if(flag == THE_ANSWER)
    {
        matriz[line_test][col_test] = 0;

        /*Increments the number of zeros in this line and column*/
        zeros_cols[col_test]++;
        zeros_lines[line_test]++;

        /*Decrements the total number of 9s*/
        decNines(game);

        /*Decrements the number of 9s in this line and column*/
        vetor_linhas[line_test]--;
        vetor_colunas[col_test]--;

        /*Saves the changes made in a stack node*/
        saveCoordinates(line_test, col_test, 0);

        /*If we are in mode 2, we call the fucntion mode2Checks, that will make*/
        /*sure that there aren't any equal lines or columns*/
        if(mode == 2)
        {
            mode2Checks(line_test, col_test, game);
            if(getFlag(game) == IMPOSSIBLE) return;
        }

        /*At the end, we call the function varrimento, but with the last*/
        /*parameter equal to 0, because we are going to enter a ramification*/
        varrimento(game, line_test, col_test, 0);
    }
    /*Same procedure as for flag == THE_ANSWER, but here we don't decrement*/
    /*the number of  in the line and column*/
    else if(flag == NOT_ANSWER)
    {
        matriz[line_test][col_test] = 1;
        decNines(game);
        vetor_linhas[line_test]--;
        vetor_colunas[col_test]--;
        saveCoordinates(line_test, col_test, 1);

        if(mode == 2)
        {
            mode2Checks(line_test, col_test, game);
            if(getFlag(game) == IMPOSSIBLE) return;
        }
        varrimento(game, line_test, col_test, 0);
    }
}

/*****************************************************************************
 * varrimento9
 * Arguments: t_puzzle* game
 * int line_test, col_test - coordinates in the matrix
 * int ramo - flag that tells us if we are in the main branch of the recursive
 * function or in one of its ramifications. 1 for the first case, 0 for the
 * second
 * Returns: void
 * Side-Effects: Fills the game table in the position [line_test][col_test]
 * with a 1 or 0 if it is mandatory to do so. Otherwise leaves a 9 there.
 * The difference between this function and varrimento, is that varrimento
 * looks for 1s and 0s with adjacent 9s, and varrimento9 looks for 9s with
 * adjacent 1s and 0s
 *****************************************************************************/
void varrimento9(t_puzzle* game, int line, int col, int ramo)
{
    int** matriz = getTable(game);
    int mode = getGamemode(game);
    int* vetor_linhas = getVetorLinhas(game), *vetor_colunas = getVetorColunas(game);
    int* zeros_lines = getZerosLines(game), *zeros_cols = getZerosCols(game);
    ANSWER flag = MAYBE;

    /*Checks if the position [line][col] can be filled with a 0*/
    flag = checkSolution(game, line, col, 0);

    /*If checkSolution returns INVALID, [line][col] can't be filled with either*/
    /*0 or 1, so the game is IMPOSSIBLE, and we return*/
    if(flag == INVALID)
    {
        changeFlag(game, IMPOSSIBLE);
        return;
    }

    /*If 0 is the correct number to insert in this position, we insert it and*/
    /*do all of the procedures previsouly described in the auxVarr function*/
    if(flag == THE_ANSWER)
    {
        matriz[line][col] = 0;
        decNines(game);
        saveCoordinates(line, col, 0);
        vetor_linhas[line]--;
        vetor_colunas[col]--;
        zeros_lines[line]++;
        zeros_cols[col]++;

        if(mode == 2)
        {
            mode2Checks(line, col, game);
            if(getFlag(game) == IMPOSSIBLE) return;
        }

        /*If we can fill this position, at the end we call auxVarr9*/
        auxVarr9(game, line, col);
        if(getFlag(game) == IMPOSSIBLE) return;
    }
    else if(flag == NOT_ANSWER)
    {
        decNines(game);
        matriz[line][col] = 1;
        saveCoordinates(line, col, 1);
        vetor_linhas[line]--;
        vetor_colunas[col]--;

        if(mode == 2)
        {
            mode2Checks(line, col, game);
            if(getFlag(game) == IMPOSSIBLE) return;
        }

        /*If we can fill this position, at the end we call auxVarr9*/
        auxVarr9(game, line, col);
        if(getFlag(game) == IMPOSSIBLE) return;
    }

    /*If we are in the main branch (ramo == 1), we get the coordinates of*/
    /*the next 9 and call varrimento9 for it*/
    if(ramo == 1)
    {
        /*If getNextPosition returns a value different than 0, it means that*/
        /*we have reached the end of the matrix*/
        if(getNextPosition(game, &line, &col) == 0)
        {
            /*If searchNextNine returns a value different than 0, it means it*/
            /*wasn't able to find a 9*/
            if(searchNextNine(game, &line, &col) == 0)
            {
                /*If we foun a 9, we call varrimento9 for its coordinates*/
                varrimento9(game, line, col, 1);
            }
        }
    }

}

/*****************************************************************************
 * auxVarr9
 * Arguments: t_puzzle* puzzle
 * int line, col - coordinates in the matrix
 * Returns: void
 * Side-Effects: Looks to the adjacent positions and if they are filled with
 * 9 calls the function auxVarr for them. At the end, if we are in the main
 * branch of the function, it calls itself for the next position filled
 * with a one or a 0
 *****************************************************************************/
void auxVarr9(t_puzzle* game, int line, int col)
{
    int** matriz = getTable(game);
    int dim = getDim(game);

    if(line != 0 && matriz[line-1][col] == 9)
    {
        varrimento9(game, line-1, col, 0);
        if(getFlag(game) == IMPOSSIBLE) return;
    }
    if(line != dim-1 && matriz[line+1][col] == 9)
    {
        varrimento9(game, line+1, col, 0);
        if(getFlag(game) == IMPOSSIBLE) return;
    }
    if(col != 0 && matriz[line][col-1] == 9)
    {
        varrimento9(game, line, col-1, 0);
        if(getFlag(game) == IMPOSSIBLE) return;
    }
    if(col != dim-1 && matriz[line][col+1] == 9)
    {
        varrimento9(game, line, col+1, 0);
        if(getFlag(game) == IMPOSSIBLE) return;
    }
}

/*****************************************************************************
* mode2Checks
* Arguments: t_puzzle* game
* int line, col - coordinates in the matrix
* Returns: void
* Side-Effects: Changes the game flag to INVALID if there are equal lines or
* columns. If the lines/columns are different saves the values associcated
* to the lines/columns that have just been fulfilled, in the respective hash
* tables
*****************************************************************************/
void mode2Checks(int line, int col, t_puzzle* game)
{
    int hash_size = getHashSize(game), dim = getDim(game);
    int** matriz = getTable(game);
    int* vetor_linhas = getVetorLinhas(game), *vetor_colunas = getVetorColunas(game);
    t_hash_list **hash_linhas = getHashLinhas(game), **hash_colunas = getHashColunas(game);
    int lineDecimal = 0, colDecimal = 0;

    /*First checks if the line is already completely fulfilled*/
    if(vetor_linhas[line] == 0)
    {
        /*If the line is complete, converts its bits from binary to decimal*/
        lineDecimal = convertLineToDecimal(matriz, line, dim);
        /*Then checks if there aren't any equal lines already*/
        if(checkRepetitionWithBackup(LINE, hash_linhas, lineDecimal, hash_size, line, matriz, dim) == INVALID)
        {
            /*If there are equal lines, changes the game flag to IMPOSSIBLE*/
            /*and returns*/
            changeFlag(game, IMPOSSIBLE);
            return;
        }
    }

    /*The same as for the lines*/
    if(vetor_colunas[col] == 0)
    {
        colDecimal = convertColToDecimal(matriz, col, dim);
        if(checkRepetitionWithBackup(COL, hash_colunas, colDecimal, hash_size, col, matriz, dim) == INVALID)
        {
            changeFlag(game, IMPOSSIBLE);
        }
    }
}

/*****************************************************************************
* checkWithBackupRepetition
* Arguments: t_hash_list**, unsigned int num, int hash_size
* Returns: MAYBE if num isn't in the hashTable, or INVALID if it is
* Side-Effects: If num isn't in the hashTable, inserts it in the proper position
*****************************************************************************/
ANSWER checkRepetitionWithBackup(ROW row, t_hash_list** hashTable, unsigned int num, int hash_size, int index, int **matriz, int size)
{
    int ret = -1;

    /*If ret == 0, num wasn't in the hash table, otherwise it was*/
    ret = checkRepetition(hashTable, num, hash_size, index, row, matriz, size);

    /*If we inserted a new number in the hash table, we save that info in*/
    /*a proper stack, to later allow us to backtrack, if necessary*/
    if (ret == 0)
    {
        if (row == COL) backupHash(COL, num, index);
        else backupHash(LINE, num, index);
        return MAYBE;
    }
    return INVALID;
}

/*****************************************************************************
* tryNumber
* Arguments: t_puzzle* game
* int line, col - coordinates in the matrix
* int number - number that we are trying to put in these coordinates of the matrix
* Returns: void
* Side-Effects: Puts a 0 or a 1 in a position of the matrix and calls recursiveSolver
* for that position, that will solve the puzzle or determine that it is
* impossible for the current state of the matrix
*****************************************************************************/
void tryNumber(t_puzzle* game, int line, int col, int number)
{
    int** matriz = getTable(game);
    int *vetor_linhas = getVetorLinhas(game), *vetor_colunas = getVetorColunas(game);
    int* zeros_lines = getZerosLines(game), *zeros_cols = getZerosCols(game);
    int mode = getGamemode(game);
    FLAGGY game_state = IMPOSSIBLE;

    /*Puts number in the matrix and if it is zero, increments the number of zeros*/
    /*in this line and column. Also, decrements the total number of nines*/
    /*and the number of nines in this line and column*/
    matriz[line][col] = number;
    if(number == 0)
    {
        zeros_lines[line]++;
        zeros_cols[col]++;
    }
    decNines(game);
    vetor_linhas[line]--;
    vetor_colunas[col]--;

    /*Saves the alterations we made in a stack node*/
    saveCoordinates(line, col, number);

    /*If we are in mode 2, makes sure that we didn't produce equal lines or*/
    /*columns, by calling mode2Checks*/
    if(mode == 2)
    {
        mode2Checks(line, col, game);
        game_state = getFlag(game);
        if(game_state == IMPOSSIBLE)
        {
            /*If we produced equal lines or columns we restore the changes we*/
            /*made by calling restoreCoordinates and we return*/
            restoreCoordinates(game);
            return;
        }
    }

    /*If we didn't break any mode 2 rules or if we are in mode 1, we call*/
    /*recursiveSolver for the current position*/
    recursiveSolver(game, line, col);
    game_state = getFlag(game);

    /*recursiveSolver will either solve the puzzle or conclude that it is*/
    /*impossible. If it was solved, we empty the stacks and return. Else*/
    /*we restore the changes made and return*/
    if(game_state == SOLVED)
    {
        emptyStacks();
        return;
    }
    else if(game_state == IMPOSSIBLE)
    {
        restoreCoordinates(game);
        return;
    }
}

/*****************************************************************************
 * getFirstPositionVarrimento
 * Arguments: t_puzzle* game
 * int line, col - coordinates in the matrix
 * Side-Effects: Saves in *line and *col the coordinates of a good position
 * to start the function varrimento at. We consider a position to be good
 * if there is a possibility of having 9s adjacent to it, so we avoid all
 * of the positions where we know there won't be any 9s around
 *****************************************************************************/
void getFirstPositionVarrimento(t_puzzle* game, int* line, int* col)
{
    int dim = getDim(game);
    int ** matriz = getTable(game);

    /*If we are in the first position of the matrix...*/
    if(*col == 0 && *line == 0)
    {
        /*And it is different than 9, this will be the starting position*/
        /*and we return*/
        if (matriz[*line][*col] != 9) return;
        /*If the number is 9, we search for the first 0 or 1 we can find and return*/
        if(searchNotNine(game, line, col) == 0) return;
    }

    /*If we are in the first line or the column of the 9 we're at is the last*/
    /*one, we start varrimento in this line but in the first column, so we*/
    /*update *col to be 0 and return*/
    if(*col == dim -1 || *line == 0)
    {
        *col = 0;
        return;
    }
    /*In all the other cases, we start in the previous line but in the next*/
    /*column*/
    else
    {
        *col = *col + 1;
        *line = *line - 1;
        return;
    }
}

/*****************************************************************************
 * searchNextNine
 * Arguments: t_puzzle* game
 * int *line, *col - coordinates in the matrix
 * Returns: Returns SOLVED when there are no more 9s ahead of the starting position
 * to fill, UNSOLVED otherwise
 * Side-Effects: Searches for a 9 that is after [*line][*col] in the matrix
 * and saves its coordinates in *line and *col if it finds one
 *****************************************************************************/
FLAGGY searchNextNine(t_puzzle* game, int* line, int* col)
{
    int *line_aux = line, *col_aux = col;
    int dim=getDim(game);
    int** matriz = getTable(game);
    int* vetor_linhas = NULL;
    int i = 0, j = 0, i_init = 0, j_init = 0, flag_i = 0, flag_j = 0;
    FLAGGY ret = UNSOLVED;

    /*If we are in a position filled with a 9 there's no need to search*/
    /*and we just return. We verify that *col is different than -1, because*/
    /*later we may call this function will *col == -1*/
    if(*col != -1) if(matriz[*line][*col] == 9) return UNSOLVED;

    /*Gets vetor_linhas from the struct*/
    vetor_linhas = getVetorLinhas(game);

    /*If we are in the last position of the matrix and it isn't a 9, there*/
    /*aren't any more 9s and we return SOLVED*/
    if(*line == dim -1 && *col == dim-1) return SOLVED;
    /*Else, if we are in the last column of a certain line, we start checking in*/
    /*the next line*/
    else if(*col == dim-1) i_init = *line + 1;
    /*Else we start checking in the line we are already at*/
    else i_init = *line;

    /*Using vetor_linhas, we look for the first line that has one or more 9s*/
    for(i = i_init; i < dim; i++)
    {
        if(vetor_linhas[i]!=0)
        {
            flag_i = 1; /*If we find a line with 9s, we save 1 in flag_i*/
            break;
        }
    }

    /*If flag_i == 0, we couldn't find any 9s and we return SOLVED*/
    if(flag_i == 0) return SOLVED;

    /*If the line with 9s is the same as the line where we started, we will*/
    /*only start checking in the column after our initial one*/
    if(i==*line) j_init = (*col)+1;
    /*Else, we start checking in the first column*/
    else j_init = 0;

    for(j=j_init; j<dim; j++)
    {
        if(matriz[i][j]==9)
        {
            flag_j = 1; /*If we find a 9 we break the cycle*/
            break;
        }
    }

    /*If we didn't find a 9, we will have to call the function again to search*/
    /*in the next lines*/
    if(flag_j == 0)
    {
        *line_aux = *line + 1;
        *col_aux = -1;

        /*We call the function again recursively*/
        ret = searchNextNine(game, line_aux, col_aux);
        /*If it doesn't find any 0s, we return SOLVED*/
        if(ret == SOLVED) return SOLVED;
        /*Otherwise, we update *line and *col and return UNSOLVED*/
        else
        {
            *line = *line_aux;
            *col = *col_aux;
            return UNSOLVED;
        }
    }

    *line = i;
    *col = j;
    return UNSOLVED;
}

/*****************************************************************************
 * searchNotNine
 * Arguments: t_puzzle* game
 * int *line, *col - coordinates in the matrix
 * Returns: Returns 1 when there are no more 0s or 1s ahead of the initial position
 * 0 otherwise
 * Side-Effects: Searches for a 1 or 0 that is after [*line][*col] in the matrix
 * and saves its coordinates in *line and *col
 *****************************************************************************/
int searchNotNine(t_puzzle* game, int* line, int* col)
{

    int dim=getDim(game);
    int** matriz = getTable(game);
    int* vetor_linhas;
    int i = 0, j = 0, i_init = 0, j_init = 0, flag_i = 0, flag_j = 0;
    int* line_aux = line, *col_aux = col;
    int ret = 0;

    vetor_linhas = getVetorLinhas(game);

    /*This functions concept is the same as searchNextNine's, but to search*/
    /*for 0s or 1s, so the same comments apply to it*/

    if(*line == dim -1 && *col == dim-1) return 1;
    else if(*col == dim-1) i_init = *line + 1;
    else i_init = *line;

    for(i=i_init; i<dim; i++)
    {
        if(vetor_linhas[i]!=dim)
        {
            flag_i = 1;
            break;
        }
    }

    if(flag_i == 0) return 1;
    if(i==*line) j_init = (*col)+1;
    else j_init = 0;

    for(j=j_init; j<dim; j++)
    {
        if(matriz[i][j]!=9)
        {
            flag_j = 1;
            break;
        }
    }

    if(flag_j == 0)
    {
        *line_aux = *line + 1;
        *col_aux = -1;

        ret = searchNotNine(game, line_aux, col_aux);
        if(ret == 1) return ret;
        else
        {
            *line = *line_aux;
            *col = *col_aux;
            return 0;
        }
    }

    *line = i;
    *col = j;
    return 0;
}

/*****************************************************************************
 * getNextPosition
 * Arguments: t_puzzle* game
 * int line, col - coordinates in the matrix
 * Returns: 1 if we were already at the end of the matrix, 0 otherwise
 * Side-Effects: Saves in *line and *col the coordinates of the next position
 * in the matrix
 *****************************************************************************/
int getNextPosition(t_puzzle* game, int* line, int* col)
{
    int dim = getDim(game);

    /*If we are in the last position, we return 1*/
    if(*line == dim -1 && *col == dim -1) return 1;
    /*If we are in the last column, we go to the first column of the next line*/
    else if(*col == dim -1)
    {
        *line = *line + 1;
        *col = 0;
    }
    /*Else, we just go to the next column*/
    else
    {
        *col = *col + 1;
    }
    return 0;
}

/*****************************************************************************
 * saveCoordinates
 * Arguments: int - line, col - coordinates in the matrix
 * int num - number we placed in those coordinates
 * Side-Effects:  Pushes into the stack the coordinates where we made changes
 * and the number we inserted there
 *****************************************************************************/
void saveCoordinates (int line, int col, int num)
{
    coord * coordinates= newCoord(line, col, num);
    pushItem(&stack, coordinates);
}

/*****************************************************************************
 * restoreCoordinates
 * Arguments: t_puzzle* game
 * Side-Effects:  Pops an Item out of the stack and puts a 9 in the respective
 * place in the matrix. Also increments the total number of nines, the number
 * of nines in this line and column, and decrements the number of zeros
 * in this line and column if the nnumber that was in this position was a 0.
 * At the end, frees the stack node
 *****************************************************************************/
void restoreCoordinates(t_puzzle* game)
{
    int** matriz = getTable(game);
    int* zeros_cols = getZerosCols(game), *zeros_lines = getZerosLines(game);
    int* vetor_linhas = getVetorLinhas(game), *vetor_colunas = getVetorColunas(game);

    coord * coordinates = (coord*) popItem(&stack);
    matriz[getLineFromCoord(coordinates)][getColFromCoord(coordinates)] = 9;
    incNines(game);
    if(getNumFromCoord(coordinates) == 0)
    {
        zeros_cols[getColFromCoord(coordinates)]--;
        zeros_lines[getLineFromCoord(coordinates)]--;
    }

    vetor_colunas[getColFromCoord(coordinates)]++;
    vetor_linhas[getLineFromCoord(coordinates)]++;
    freeCoord(coordinates);
}

/*****************************************************************************
 * backupHash
 * Arguments:  ROW row - indicates us if we are dealing with a line or
 * a column
 * unsigned int num, int col_line - number and index of its respective line/column
 * Side-Effects: Pushes num and col_line into the proper stack, dependeing
 * on the row parameter
 *****************************************************************************/
 void backupHash(ROW row, unsigned int num, int col_line)
 {
     hash_container * container= allocHashContainer(num, col_line);
     if (row == COL) pushItem(&stack_hash_colunas, container);
     else if (row == LINE) pushItem(&stack_hash_linhas, container);
 }

/*****************************************************************************
 * restoreHash
 * Arguments: ROW row - indicates if we are dealing with a line or a column
 * t_hash_list** hash_table - hash table from which we want to remove elements
 * int hash_table_size, matrix_dim - size of the hash table and the matrix,
 * respectively
 * Side-Effects: Removes from the stack the number and index of the line/column
 * it corresponds to, that will then let us remove it from the hash table.
 * At the end frees the stack node where it was
 *****************************************************************************/
 void restoreHash(ROW line_col, t_hash_list** hash_table, int hash_table_size, int matrix_dim)
 {
     hash_container* container = NULL;
     if (line_col == COL)
     {
         container = popItem(&stack_hash_colunas);
         removeNum(hash_table, getNumFromHashContainer(container), hash_table_size, getColLineFromHashContainer(container), matrix_dim);
     }
     else if (line_col == LINE)
     {
         container = popItem(&stack_hash_linhas);
         removeNum(hash_table, getNumFromHashContainer(container), hash_table_size, getColLineFromHashContainer(container), matrix_dim);
     }
     freeHashContainer(container);
 }

/*****************************************************************************
 * emptyStacks
 * Arguments: void
 * Side-Effects: Frees all of the stack nodes from all the stacks
 *****************************************************************************/
void emptyStacks(void)
{
    coord * coordinates;
    hash_container * container;
    while(stack != NULL)
    {
        coordinates = (coord*) popItem(&stack);
        freeCoord(coordinates);
    }
    stack = NULL;

    while(stack_hash_colunas != NULL)
    {
        container = (hash_container*) popItem(&stack_hash_colunas);
        freeHashContainer(container);
    }
    stack_hash_colunas = NULL;

    while(stack_hash_linhas != NULL)
    {
        container = (hash_container*) popItem(&stack_hash_linhas);
        freeHashContainer(container);
    }
    stack_hash_linhas = NULL;
}
