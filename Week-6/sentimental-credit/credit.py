"""
    AMEX
    15-digit
    start with 34 or 37

    MASTERCARD
    16-digit
    start with 51, 52, 53, 54, or 55

    VISA
    13-digit
    16-digit
    start with 4
"""

from keyword import iskeyword
import re


def check_company(length, number):
    # Check the length of the number
    # and and which company does it belongs to
    if length == 15 and re.search("^3[4,7].*$", number):
        return "AMEX"
    elif length == 16:
        if re.search("^4.*$", number):
            return "VISA"
        elif re.search("^5[1-5].*$", number):
            return "MASTERCARD"
        else:
            return "INVALID"
    elif length == 13 and re.search("^4.*$", number):
        return "VISA"
    else:
        return "INVALID"


def checksum(number):
    # Length and Company Check
    length = len(number)
    company = check_company(length, number)
    if company == "INVALID":
        return "INVALID"

    # Checksum
    summ_1 = 0
    summ_2 = 0
    for i in range(1, length + 1):
        idx = length - i
        if i % 2 == 0:
            n = int(number[idx]) * 2
            if n > 9:
                summ_1 += int(n / 10)
                summ_1 += int(n % 10)
            else:
                summ_1 += n
        else:
            summ_2 += int(number[idx])

    # Cheksum response
    if (summ_1 + summ_2) % 10 == 0:
        return company
    else:
        return "INVALID"


def main():
    # Ask a number in input
    # until a an only-digits
    # input is submitted
    valid_input = False
    while not valid_input:
        number = input("Number: ")
        if number.isdigit():
            valid_input = True
            print(checksum(number))


main()
