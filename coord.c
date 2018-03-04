/******************************************************************************
 * File Name: coord.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements the data types that will save the info related to
 * the bactrack system and the functions to operate it
 *****************************************************************************/
#include "coord.h"

/*Declares the data type that will save a number (0 or 1) and the coordinates*/
/*where we put it*/
struct _t_coord {
    int line;
    int col;
    int num;
};

/*Declares the data type that will hold info related to what we saved in the*/
/*hash tables*/
struct _t_hash_container{
    unsigned int num; /*Decimal number associated to the line/col*/
    int col_line;     /*Index of the correspondent column or line*/
};

/*****************************************************************************
* newCoord
* Arguments: int line, int col - coordinates
* int num - number we put in the position [line][col]
* Returns: coord* coordinates
* Side-Effects: Alocates memory a new coord type element
*****************************************************************************/
coord * newCoord(int line, int col, int num)
{
    coord * coordinates = (coord*)malloc(sizeof(coord));
    if (coordinates == NULL) exit(0);
    coordinates->line = line;
    coordinates->col = col;
    coordinates->num = num;
    return coordinates;
}

/*****************************************************************************
* getLineFromCoord
* Arguments: coord * coordinates
* Returns: int container->line
*****************************************************************************/
int getLineFromCoord(coord * coordinates)
{
    return coordinates->line;
}

/*****************************************************************************
* getColFromCoord
* Arguments: coord * coordinates
* Returns: int container->col
*****************************************************************************/
int getColFromCoord(coord * coordinates)
{
    return coordinates->col;
}

/*****************************************************************************
* getNumFromCoord
* Arguments: coord * coordinates
* Returns: int container->num
*****************************************************************************/
int getNumFromCoord(coord * coordinates)
{
    return coordinates->num;
}

/*****************************************************************************
* freeCoord
* Arguments: coord * coordinates
* Side-Effects: Frees the pointer passed as argument
*****************************************************************************/
void freeCoord(coord * coordinates)
{
    free(coordinates);
    coordinates = NULL;
}

/*****************************************************************************
* allocHashContainer
* Arguments: unsigned int num
* Returns: hash_container* container
* Side-Effects: Alocates memory for a hash_container* element with
* container->num = num and container->col_lina = col_line
*****************************************************************************/
hash_container * allocHashContainer (unsigned int num, int col_line)
{
    hash_container * container= (hash_container*)malloc(sizeof(hash_container));
    if (container == NULL) exit(0);
    container->num = num;
    container->col_line = col_line;
    return container;
}

/*****************************************************************************
* getNumFromHashContainer
* Arguments: hash_container * container
* Returns: container->num
*****************************************************************************/
unsigned int getNumFromHashContainer(hash_container * container)
{
    return container->num;
}

/*****************************************************************************
* getColLineFromHashContainer
* Arguments: hash_container * container
* Returns: container->col_line
* Side-Effects: none
*****************************************************************************/
unsigned int getColLineFromHashContainer(hash_container * container)
{
    return container->col_line;
}

/*****************************************************************************
* freeHashContainer
* Arguments: hash_container * container
* Returns: void
* Side-Effects: Frees the pointer passed as argument
*****************************************************************************/
void freeHashContainer (hash_container * container)
{
    free(container);
    container = NULL;
}
