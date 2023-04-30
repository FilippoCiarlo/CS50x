// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool valid(string password);

int main(void)
{
    // ask a password in input
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{
    // lenght of the password
    int n = strlen(password);

    // conditions
    bool special = false;
    bool lowerCase = false;
    bool upperCase = false;
    bool numeric = false;

    // check the passowrd character by character
    for (int i = 0; i < n; i++)
    {
        char c = password[i];
        if (special && lowerCase && upperCase && numeric)
        {
            return true;
        }
        else if (ispunct(c))
        {
            special = true;
        }
        else if (islower(c))
        {
            lowerCase = true;
        }
        else if (isupper(c))
        {
            upperCase = true;
        }
        else if (isdigit(c))
        {
            numeric = true;
        }
    }
    return false;
}
