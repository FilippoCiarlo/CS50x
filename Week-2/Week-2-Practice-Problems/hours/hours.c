#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

float calc_hours(int hours[], int weeks, char output);

int main(void)
{
    // ask the number of weeks
    int weeks = get_int("Number of weeks taking CS50: ");
    int hours[weeks];

    // ask the hours per week
    for (int i = 0; i < weeks; i++)
    {
        hours[i] = get_int("Week %i HW Hours: ", i);
    }

    // ask the kind of the output
    char output;
    do
    {
        output = toupper(get_char("Enter T for total hours, A for average hours per week: "));
    }
    while (output != 'T' && output != 'A');

    printf("%.1f hours\n", calc_hours(hours, weeks, output));
}




// TODO: complete the calc_hours function
float calc_hours(int hours[], int weeks, char output)
{
    // summ of the hours
    float h_sum = 0;
    for (int i = 0; i < weeks; i++)
    {
        h_sum += hours[i];
    }

    // return the request
    switch (output)
    {
        case 'T':
            return h_sum;
        case 'A':
            return h_sum / weeks;
        default:
            printf("Switch Error\n");
            return -1;
    }
}