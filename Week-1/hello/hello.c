#include <cs50.h>
#include <stdio.h>

int main()
{
    // ask the user name
    string name = get_string("What's your name? ");

    // say hello to the user
    printf("hello, %s\n", name);
}