import re


def colemanLiau_formula(l, s):
    # Compute the Coleman-Liau Formula
    grade = round(0.0588 * l - 0.296 * s - 15.8)

    if grade >= 16:
        return "Grade: 16+"
    elif grade < 1:
        return "Before Grade 1"
    else:
        return str(f"Grade: {grade}")


def main():
    # Ask a text in input
    text = input("Text: ")

    # Count the number of lettes in the text
    # in lowercase and UPPERCASE
    letters = len(re.findall(r"[a-zA-Z]", text))

    # Assume that a space divide two words
    words = len(re.findall(r"[ ]", text)) + 1

    # Assume that a period, esclamation point or
    # question mark indicates the end of a sentence
    sentences = len(re.findall(r"[.!?]", text))

    # Average of letters per 100 words
    avgL = (letters / words) * 100

    # Average of sentences per 100 words
    avgS = (sentences / words) * 100

    # Compute the grade with Coleman-Liau Formula
    result = colemanLiau_formula(avgL, avgS)
    print(result)


if __name__ == "__main__":
    main()
