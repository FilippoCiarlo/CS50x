// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
//const unsigned int N = 26;
const unsigned int N = 3440;

// Hash table
node *table[N];

// Dictionary size
int dict_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char string[strlen(word)];
    strcpy(string, word);

    // Convert strint to Lowercase
    for (int i = 0; string[i] != '\0'; i++)
    {
        string[i] = tolower(string[i]);
    }

    // Retive the Hash value
    int index = hash(string);

    // Search the string in the dictionary
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(string, tmp->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Improve this hash function
    //return toupper(word[0]) - 'A';

    unsigned int hash_value = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_value += toupper(word[i]);
    }

    return hash_value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open Dictionary File
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fclose(file);
        return false;
    }

    // Read Strings from File
    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        // Create a New Node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        strcpy(n->word, buffer);
        n->next = NULL;

        // Hash Word
        int index = hash(buffer);

        // Insert Node into Hash Table
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }

        // Increment the word counter
        dict_size++;
    }

    // Close Dictionary File
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *next_node;

    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            next_node = table[i]->next;
            free(table[i]);
            table[i] = next_node;
        }
    }
    free(*table);

    return true;
}

