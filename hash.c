/******************************************************************************
 * File Name: hash.c
 * Authors:    Marcelo Jacinto && Joao Silva
 * Last modified: 2017-12-02
 * Revision:  v1.0
 * COMMENTS: Implements hash tables and functions to operate them
 *****************************************************************************/
#include "hash.h"

/*****************************************************************************
* initializeHashTable
* Arguments: t_hash_list** hashTable, int hash_size
* Returns: void
* Side-Effects: Initializes an hash table
*****************************************************************************/
void initializeHashTable(t_hash_list** hashTable, int hash_size)
{
    int i = 0;
    for(i=0; i<hash_size; i++) hashTable[i] = NULL;
}

/*****************************************************************************
* createHashTable
* Arguments: int dim
* Returns: Pointer to a new hash table
* Side-Effects: Allocates memory for a hash table with size dim
*****************************************************************************/
t_hash_list** createHashTable(int dim)
{
    t_hash_list** newHashTable;
    int i = 0;

    newHashTable = (t_hash_list**)malloc(sizeof(t_hash_list*)*dim);
    if(newHashTable == NULL) exit(0);
    for (i = 0; i < dim; i++)
    {
        newHashTable[i] = NULL;
    }

    return newHashTable;
}

/*****************************************************************************
* chooseHashSize
* Arguments: int dim (dimension of our game matrix)
* Returns: Size of the hashTable to be created
* Side-Effects: none
*****************************************************************************/
int chooseHashSize(int dim)
{
    int min_size = 0, size = 0;
    min_size = 1.5*dim;
    size = getFirstPrime(min_size);
    return size;
}

/*****************************************************************************
* getIdx
* Arguments: unsigned int n, int hash_size
* Returns: n's index in the hashTable
* Side-Effects: none
*****************************************************************************/
int getIdx(unsigned int n, int hash_size)
{
    return (int)(n%hash_size);
}

/*****************************************************************************
* removeNum
* int num - decimal number associated to the line/column,
* int idx - index of the element in the hash table
* int col_line - index of the column/line whose value we are inserting
* int matriz_size - size of the matrix
* Returns: void
* Side-Effects: Checks if a number is present in the hashTable, and removes
* it if it is
*****************************************************************************/
void removeNum(t_hash_list** hash, unsigned int num, int hash_size, int col_line, int dim)
{
    int idx = getIdx(num, hash_size);

    if(hash[idx] == NULL) return;
    else findRemoveNum(hash, hash[idx], num, idx, col_line, dim);
}

/*****************************************************************************
* findRemoveNum
* Arguments: t_hash_list** hash, unsigned
* int num - decimal number associated to the line/column,
* int idx - index of the element in the hash table
* int col_line - index of the column/line whose value we are inserting
* int matriz_size - size of the matrix
* Returns: void
* Side-Effects: Checks if a number is present in an hashTable list, and removes
* it if it is
*****************************************************************************/
void findRemoveNum(t_hash_list** hashTable, t_hash_list* head, unsigned int num, int idx, int col_line, int matriz_size)
{
    t_hash_list* aux = head;

    /*When we hash and element to the table we only use MAX_SIZE bits to do it*/
    /*so in these cases, if there are collisions we must compare the whole*/
    /*line/column to know if it is equal to the one we are trying to insert*/

    /*Checks if the first element of the list is equal to the number we are trying*/
    /*to insert, because in this case we will need to put hashTable[idx] pointing*/
    /*to NULL*/
    if(getListPrev(aux) == NULL && getListNum(aux) == num)
    {
        if(matriz_size > MAX_SIZE)
        {
            if(getListColLine(aux) == col_line)
            {
                removeListNode(&aux);
                hashTable[idx] = NULL;
                return;
            }
        }
        else
        {
            removeListNode(&aux);
            hashTable[idx] = NULL;
            return;
        }
    }

    while(aux != NULL)
    {
        if(num == getListNum(aux))
        {
            if(matriz_size <= MAX_SIZE)
            {
                removeListNode(&aux);
                return;
            }
            else if (matriz_size > MAX_SIZE && getListColLine(aux) == col_line)
            {
                removeListNode(&aux);
                return;
            }
        }
        aux = getListNext(aux);
    }
}


/*****************************************************************************
* checkRepetition
* Arguments: t_hash_list**, unsigned int num, int hash_size
* Returns: returns 0 if num isn't in the hashTable, or 1 if it is
* Side-Effects: if num isn't in the hashTable, inserts it in the proper position,
* along with its line/column index
*****************************************************************************/
int checkRepetition(t_hash_list** hashTable, unsigned int num, int hash_size, int index, ROW row, int** matriz, int matriz_size)
{
    int idx = getIdx(num, hash_size);
    t_hash_list* aux = NULL;

    if(hashTable[idx] == NULL)
    {
        aux = createListNode(num, index);
        hashTable[idx] = aux;
        return 0;
    }
    else return findInsertNum(hashTable[idx], matriz, num, matriz_size, index, row);

}

/*****************************************************************************
* findInsertNum
* Arguments: t_hash_list* head, unsigned int num
* Returns: 0 if num isn't in this list of the hashTable, or -2 if it is
* Side-Effects: If num isn't in this hashTable list, inserts it
* at the end of it, along with its line/column index
*****************************************************************************/
int findInsertNum(t_hash_list* head, int ** matriz, unsigned int num, int matriz_size , int index1, ROW row)
{
    t_hash_list* aux = head;
    int flag = 0;
    int index2 = -1;

    if(getListNum(aux) == num)
    {
        if(matriz_size > MAX_SIZE)
        {
            index2 = getListColLine(aux);
            flag = -2;
        }
        else return -2;
    }

    if(flag != -2)
    {
        while(getListNext(aux) != NULL)
        {
            if(getListNum(aux) == num)
            {
                if(matriz_size > MAX_SIZE)
                {
                    index2 = getListColLine(aux);
                    flag = -2;
                    break;
                }
                else return -2;
            }
            aux = getListNext(aux);
        }
    }

    if(flag != -2)
    {
        if(getListNum(aux) == num)
        {
            if(matriz_size > MAX_SIZE)
            {
                index2 = getListColLine(aux);
                flag = -2;
            }
            else return -2;
        }
    }

    if(flag == -2)
    {
        if(compareLineCol(index1, index2, row, matriz_size, matriz) == -2) return -2;
        else if(getListNext(aux) == NULL)
        {
            insertListNode(num, index1, aux);
            return 0;
        }
        else return findInsertNum(getListNext(aux), matriz, num, matriz_size, index1, row);
    }
    else insertListNode(num, index1, aux);

    return 0;
}

/*****************************************************************************
* freeHashTable
* Arguments: t_hash_list** hashTable, int hash_size
* Returns: void
* Side-Effects: Frees all hashTable's lists
*****************************************************************************/
void freeHashTableElements(t_hash_list** hashTable, int hash_size)
{
    int i = 0;
    for(i=0; i<hash_size; i++) if(hashTable[i] != NULL)
    {
        freeList(hashTable[i]);
        hashTable[i] = NULL;
    }
}
