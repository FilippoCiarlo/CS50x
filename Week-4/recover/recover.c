#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


// Number of bytes in a FAT sector
#define BLOCK_SIZE 512

// Number of character in the name of a new JPEG file
#define NAME_SIZE 8

// Define a new name for 8-bit unsigned integer
typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    // Check the number of arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Retrieves the file name
    char filename[strlen(argv[1])];
    strcpy(filename, argv[1]);

    // Opens the file
    FILE *file = fopen(filename, "r");

    // JPEGs file counter
    int fileNumber = 0;

    // File Name and File Pointer
    char img_filename[NAME_SIZE];
    FILE *img = NULL;

    // Reading buffer
    BYTE buffer[BLOCK_SIZE];

    // Loops until the end of the file
    while (fread(buffer, BLOCK_SIZE, 1, file) == 1)
    {
        // Check if is the start of a new JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Check if there is another image is already open
            if (fileNumber != 0)
            {
                // Closes the previous JPEG file
                fclose(img);
            }

            // Creates new JPEG file name
            sprintf(img_filename, "%03i.jpg", fileNumber);

            // Creates a new JPEG file
            img = fopen(img_filename, "w");

            // Writes into the new JPEG file
            fwrite(buffer, BLOCK_SIZE, 1, img);

            fileNumber++;
        }
        // If is not, continue to write
        else if (img != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }

    fclose(img);
    fclose(file);

    return 0;
}