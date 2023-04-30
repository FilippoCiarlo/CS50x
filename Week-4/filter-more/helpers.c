#include "helpers.h"
#include <stdbool.h>
#include <math.h>

// Swap two RGB pixels
void swapRGB(RGBTRIPLE *rgb1, RGBTRIPLE *rgb2);



// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int sum = 0;
    double average = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute the average
            sum = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            average = (double) sum / 3;

            // Set the new RGB values (grayscale filter)
            image[i][j].rgbtRed = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtBlue = round(average);
        }
    }

    return;
}



// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // Swap the horizontally opposite pixels until the end of the left-half of the image
        for (int j = 0, k = width - 1, n = width / 2; j < n; j++, k--)
        {
            swapRGB(&image[i][j], &image[i][k]);
        }
    }

    return;
}



// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    int sumRed = 0;
    int sumGreen = 0;
    int sumBlue = 0;
    int count = 0;

    // Creates a temporary image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reinitializes the variable used to store the sum and the counter
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            count = 0;

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if (i - 1 + x < 0 || i + x > height || j - 1 + y < 0 || j + y > width)
                    {
                        continue;
                    }
                    else
                    {
                        sumBlue += image[i - 1 + x][j - 1 + y].rgbtBlue;
                        sumGreen += image[i - 1 + x][j - 1 + y].rgbtGreen;
                        sumRed += image[i - 1 + x][j - 1 + y].rgbtRed;
                        count++;
                    }
                }
            }

            // Stores the average
            tmp[i][j].rgbtRed = round((double) sumRed / count);
            tmp[i][j].rgbtGreen = round((double) sumGreen / count);
            tmp[i][j].rgbtBlue = round((double) sumBlue / count);
        }
    }

    // Copies the temporary image in the real image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}



// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    const int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int sumGx_red = 0, sumGx_green = 0, sumGx_blue = 0;
    int sumGy_red = 0, sumGy_green = 0, sumGy_blue = 0;

    RGBTRIPLE tmp[height][width];

    // Creates a temporary image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reinitializes the variable used to store the sum
            sumGx_red = sumGx_green = sumGx_blue = 0;
            sumGy_red = sumGy_green = sumGy_blue = 0;

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if (i - 1 + x < 0 || i + x > height || j - 1 + y < 0 || j + y > width)
                    {
                        continue;
                    }
                    else
                    {
                        sumGx_red += (image[i - 1 + x][j - 1 + y].rgbtRed * Gx[x][y]);
                        sumGx_green += (image[i - 1 + x][j - 1 + y].rgbtGreen * Gx[x][y]);
                        sumGx_blue += (image[i - 1 + x][j - 1 + y].rgbtBlue * Gx[x][y]);

                        sumGy_red += (image[i - 1 + x][j - 1 + y].rgbtRed * Gy[x][y]);
                        sumGy_green += (image[i - 1 + x][j - 1 + y].rgbtGreen * Gy[x][y]);
                        sumGy_blue += (image[i - 1 + x][j - 1 + y].rgbtBlue * Gy[x][y]);
                    }
                }
            }

            // Computes the magnitude
            int red = round(sqrt(pow(sumGx_red, 2) + pow(sumGy_red, 2)));
            int green = round(sqrt(pow(sumGx_green, 2) + pow(sumGy_green, 2)));
            int blue = round(sqrt(pow(sumGx_blue, 2) + pow(sumGy_blue, 2)));

            // Stores, and evtually caps, the magnitude
            tmp[i][j].rgbtRed = red > 255 ? 255 : red;
            tmp[i][j].rgbtGreen = green > 255 ? 255 : green;
            tmp[i][j].rgbtBlue = blue > 255 ? 255 : blue;
        }
    }

    // Copies the temporary image in the real image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}



void swapRGB(RGBTRIPLE *rgb1, RGBTRIPLE *rgb2)
{
    RGBTRIPLE tmp = *rgb1;
    *rgb1 = *rgb2;
    *rgb2 = tmp;
}
