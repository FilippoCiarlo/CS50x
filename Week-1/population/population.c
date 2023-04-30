#include <cs50.h>
#include <stdio.h>

int get_size(int min_size, bool flag);
int get_years(int start_size, int end_size);

int main(void)
{
    // minimum llamas' popultion size
    const int min_size = 9;

    // start/end size flags
    bool s_sz = true;
    bool e_sz = false;

    // TODO: Prompt for start size
    int start_size = get_size(min_size, s_sz);

    // TODO: Prompt for end size
    int end_size = get_size(start_size, e_sz);

    // TODO: Calculate number of years until we reach threshold
    int years = get_years(start_size, end_size);

    // TODO: Print number of years
    printf("Years: %i\n", years);
}

int get_size(int min_size, bool flag)
{
    int size = 0;
    do
    {
        if (flag)
        {
            size = get_int("Start size: ");
        }
        else
        {
            size = get_int("End size: ");
        }
    }
    while (size < min_size);
    return size;
}

int get_years(int start_size, int end_size)
{
    int population = start_size;
    int years = 0;
    while (population < end_size)
    {
        int new = (int)(population / 3);
        int passed_away = (int)(population / 4);
        population = population + new - passed_away;
        years += 1;
    }
    return years;
}
