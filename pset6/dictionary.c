/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

char word[LENGTH + 1];
int cant;

typedef struct node
{
    char word[LENGTH];
    struct node* next;
}node;

node* hashtable[500];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    char cad[LENGTH + 1];
    int len = strlen(word), index;
    
    for(int i = 0; i < len; i++)
    {   
        if(word[i] == '\'')
        {
            cad[i] = '\'';
        }
        else if(isupper(word[i]))
        {
            cad[i] = tolower(word[i]);
        }
        else
        {
            cad[i] = word[i];
        }
    }
    cad[len] = '\0';
    index = (cad[0] - 'a' + 33) % 500;
    node* n = hashtable[index];
    
    // traverse dictionary while next is not null 
    while(n->next != NULL)
    {
        if(strcmp(cad, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    char key[LENGTH];
    int c,index, j = 0;
    
    FILE* file = fopen(dictionary, "r");
    if(file == NULL)
    {
        return 1;
    }

    for(int i = 0; i < 500; i ++)
    {
        hashtable[i] = malloc(sizeof(node));
        hashtable[i]->next = NULL;
    }

    while((c = getc(file)) != EOF)
    {
        if(c == '\n')
        {   
            key[j] = '\0';
            j = 0;
            index = (key[0] - 'a' + 33) % 500;
            node* n = hashtable[index];
        
            while(n->next != NULL)
            {
                if(strcmp(key, n->word) != 0)
                {
                    break;
                }
                n = n->next;
            }

            node* n_node = malloc(sizeof(node));
            strcpy(n_node->word, key);
            n_node->next = hashtable[index];
            hashtable[index] = n_node;
            cant++;
        }
        else
        {   
            if(c == '\'')
            {
                key[j] = '\'';
            }
            else if(c >= 'A' && c <= 'Z')
            {
                key[j] = ((char)c - 'A' + 'a');
            }
            else
            {
                key[j] = (char)c;
            }
            j++;
        }
    }
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return cant;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    node* n;
    for(int i = 0; i < 500; i++)
    {
        n = hashtable[i];
        while(n->next != NULL)
        {
            node* tmp = n;
            n = n->next;
            free(tmp);
        }
    }
    return true;
}
