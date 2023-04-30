// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>
#include <string.h>

string replace(string word);

int main(int argc, string argv[])
{
    // check input aguments
    if (argc == 2)
    {
        string new_word = replace(argv[1]);
        printf("%s\n", new_word);
    }
    else
    {
        printf("Usage: ./no-vowels word\n");
        return 1;
    }
}

string replace(string word)
{
    // apply leetspeak
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        char x = word[i];
        switch (x)
        {
            case 'a':
                word[i] = '6';
                break;
            case 'e':
                word[i] = '3';
                break;
            case 'i':
                word[i] = '1';
                break;
            case 'o':
                word[i] = '0';
            default:
                word[i] = word[i];
                break;
        }
    }
    return word;
}
