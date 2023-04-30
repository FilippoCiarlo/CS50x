#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


// Coleman–Liau index Letters' Coefficient
#define L_COEFF 0.0588

// Coleman–Liau index Sentences' Coefficient
#define S_COEFF 0.296

// Coleman–Liau index Constant
#define COLEMAN_CONST 15.8

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int get_grade(string text);

int main(void)
{
    string text = get_string("Text: ");
    int grade = get_grade(text);

    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    // compute the number of letters present in the text
    int letters = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    // compute the number of word present in the text
    int words = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        if (isblank(text[i]))
        {
            words++;
        }
    }
    // last word
    words += 1;
    return words;
}

int count_sentences(string text)
{
    // compute the number of sentences in the text
    int sentences = 0;
    int n = strlen(text);
    for (int i = 0; i < n; i++)
    {
        char c = text[i];
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }
    return sentences;
}


int get_grade(string text)
{
    int words = count_words(text);
    int letters = count_letters(text);
    int sentences = count_sentences(text);

    float L = 0.0;
    float S = 0.0;

    if (words != 0)
    {
        // average of the numbers of letters per 100 words
        L = (float) letters / words * 100;

        // average of the numbers of sentences per 100 words
        S = (float) sentences / words * 100;
    }

    // compute the Coleman-Liau index
    float index = (L_COEFF * L) - (S_COEFF * S) - COLEMAN_CONST;
    return (int) round(index);
}
