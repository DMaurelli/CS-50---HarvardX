/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if(n < 0)
    {
        return false;
    }
    int mid = n/2, inf = 0, sup = n;
    while((inf <= sup) && (value != values[mid]))
    {
        if(value < values[mid])
        {
            sup = mid - 1;
        }
        else
        {
            inf = mid + 1;
        }
        mid = (inf + sup)/2;
    }
    if(value == values[mid])
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int aux;
    for(int i = 1; i <= n; i++)
    {
        for(int j = 0; j <= n-i; j++)
        {
            if(values[j] > values[j+1])
            {
                aux = values[j];
                values[j] = values[j+1];
                values[j+1] = aux;
            }
        }
    }
}
