#include <cs50.h>
#include <stdio.h>

void check_cc(long int number);
void check_company(long int number, int length);

int main(void)
{
    // ask credit card number
    long int number = get_long("Number: ");

    // check credit card number
    check_cc(number);
}

int get_companyID(long int number, int lenght)
{
    // retrive first 2 digit of the number
    int a = -1;
    int b = -1;
    for (int i = 0; i < (lenght - 1); i++)
    {
        // 2nd digit
        b = number % 10;

        number /= 10;

        // 1st digit
        a = number % 10;
    }
    // merge the 2 digit
    int company_id = (a * 10) + b;
    return company_id;
}

void check_company(long int number, int lenght)
{
    /*
        Companies Info:

        AMEX
        lenght: uses 15-digit numbers
        company id: numbers start with 34

        MASTERCARD
        lenght: uses 16-digit numbers
        company id: numbers start with 51, 52, 53, 54, or 55

        VISA
        lenght: uses 13- and 16-digit numbers
        company id: start with 4
    */

    /*
        retrive the company id to determine
        which company the card belongs to
    */
    int company_id = get_companyID(number, lenght);

    // check the lenght and company id of the number to
    if (lenght == 16 && (company_id >= 51 && company_id <= 55))
    {
        printf("MASTERCARD\n");
    }
    else if (lenght == 15 && (company_id == 34 || company_id == 37))
    {
        printf("AMEX\n");
    }
    else if ((lenght == 13 || lenght == 16) && company_id / 10 == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

void check_cc(long int number)
{
    /*
        odds_sum represent the sum of all the digit
        present in a odd position of the given number
        such as 1st, 3rd, 5th... place

        even_sum represent the sum of all the digit
        present in a odd position of the given number
        such as 2nd, 4th, 6th... place
    */
    int odds_sum = 0;
    int even_sum = 0;
    int i = 1;

    long int tmp_number = number;
    while (tmp_number != 0)
    {
        /*
            in each loop retrieve the last digit of
            the remaining number
        */
        int reminder = tmp_number % 10;
        tmp_number /= 10;

        /*
            if the digit has an even index
            it is first multiplied by 2 and
            then summed up to the even_sum
            variable

            otherwise is directly summed up
            to the odds_sum variable
        */
        if (i % 2 == 0)
        {
            int product = reminder * 2;

            even_sum += (product / 10);
            even_sum += (product % 10);
        }
        else
        {
            odds_sum += reminder;
        }

        // increase the index
        i ++;
    }

    // compute the checksum
    int checksum = odds_sum + even_sum;

    if (checksum % 10 == 0)
    {
        // check the lenght and the company
        check_company(number, (i - 1));
    }
    else
    {
        printf("INVALID\n");
    }
}
