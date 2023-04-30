#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
}

int convert(string input)
{
    // input length
    int length = strlen(input);

    // base case
    if (length == 0)
    {
        return 0;
    }

    // last-element index
    int last = strlen(input) - 1;

    // convert char to int
    int integer = (int) input[last] - '0';

    // reduce the size of he string
    input[last] = '\0';

    // recursive call
    return (convert(input) * 10) + integer;
}