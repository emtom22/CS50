// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
// #define N 26

#define N 703 // 26 first letter * 27 second letter + 1

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 703, inclusive, based on combinations of first 2 letters 
// This assumes that 1st is alpha and 2nd is alpha or apostrophe
// also add an extra bucket for 1 letter words
// 25 first letters * 26 second letters with apostrophe + 1 bucket for single char words
unsigned int hash(const char *word)
{
    int digit_1 = tolower(word[0]) - 'a';
    int digit_2 = 0;

    // Check if 2nd digit is non-alpha or non-existent
    // If only 1 digit, then it goes in last bucket
    if (word[1] == '\''){
        digit_2 = 26;
    }
    else if (word[1] == '\0'){
        return 703;
    }
    else {
        digit_2 = tolower(word[1] - 'a');
    }

    return (digit_1 * 26) + (digit_2 + digit_1);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        hash(word);

        // if new,then add a new header
        // if not then, add to existing linked list

        // It is assumed that all dictionaries loaded are alphabetized and lower case
        // Therefore, will do a binary search to sort into hash tables.


        // REMEMBER THAT DICTIONARY IS SORTED ALPHABETICALLY
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO

    // Convert word to lower case
	char word_lower[strlen(word)];
	int i = 0;
	while (word[i]) {
		word_lower[i] = tolower(word[i]);
		i++;
	}
    
    // Try to find word in dictionary
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
