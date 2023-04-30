#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// alphabet length
#define ALPH_LEN 26

int letter_idx(char c);
bool check_key(string key);
string encrypt(string key, string plaintext);


int main(int argc, string argv[])
{
    string key = argv[1];
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (check_key(key))
    {
        string plaintext = get_string("plaintext: ");
        string ciphertext = encrypt(key, plaintext);
        printf("ciphertext: %s\n", ciphertext);
        return 0;
    }
    return 1;
}

int letter_idx(char c)
{
    // compute and return the index of a letter
    // where a/A = 0, b/B = 1, ...
    return (int) toupper(c) - 'A';
}

bool check_key(string key)
{
    // check the length of the key
    int length = strlen(key);
    if (length != ALPH_LEN)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    // check if the key contain only characters
    // and if there are duplicates among them
    int letters[ALPH_LEN];
    for (int i = 0; i < length; i++)
    {
        // check characters
        if (!isalpha(key[i]))
        {
            printf("Key must contain only characters.\n");
            return false;
        }

        // check duplicates
        int letter = letter_idx(key[i]);
        if (letters[letter] == 1)
        {
            printf("Key must not contain duplicates characters.\n");
            return false;
        }
        else
        {
            letters[letter]++;
        }
    }
    return true;
}

string encrypt(string key, string plaintext)
{
    // encrypt the plaintext
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++)
    {
        int letter = letter_idx(plaintext[i]);
        if (isalpha(plaintext[i]))
        {
            // uppercase and lower case management
            if (isupper(plaintext[i]))
            {
                plaintext[i] = toupper(key[letter]);
            }
            else
            {
                plaintext[i] = tolower(key[letter]);
            }
        }
    }
    return plaintext;
}
