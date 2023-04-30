#include <cs50.h>
#include <stdio.h>


int get_size(void);
void print_char(int n, char c);
void print_pyramid(int height);

int main(void)
{
    // ask for the size
    int size = get_size();

    // print the pyramid
    print_pyramid(size);
}

int get_size(void)
{
    // pyramid max/min size
    const int min_size = 1;
    const int max_size = 8;

    // ask the size until it is in the allowed range
    int size;
    do
    {
        size = get_int("Height: ");
    }
    while (size > max_size | size < min_size);
    return size;
}

void print_char(int n, char c)
{
    // given a character "c" print it n-times
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}

void print_pyramid(int height)
{
    for (int i = 0; i < height; i++)
    {
        // print the spaces before the blocks, if there is any
        int n_spaces = height - (i + 1);
        print_char(n_spaces, ' ');

        // print the two triangles with 2 white-space between
        int n_block = i + 1;
        print_char(n_block, '#');
        print_char(2, ' ');
        print_char(n_block, '#');

        // start a new line of blocks
        printf("\n");
    }
}

