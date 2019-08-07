#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover infile\n");
        return 1;
    }

    char *infile = argv[1];

    FILE *intputFile = fopen(infile, "r");
    if (intputFile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    BYTE buffer[512];
    int imageCount = 0;

    char filename[8];
    FILE *outputFile = NULL;

    while (true)
    {
        size_t bytesRead = fread(buffer, sizeof(BYTE), BLOCK_SIZE, intputFile);

        if (bytesRead == 0 && feof(intputFile))
        {
            break;
        }

        // check if we found a JPEG
        bool containsJpegHeader = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

        // If theres another jpeg, then close previous
        if (containsJpegHeader && outputFile != NULL)
        {
            fclose(outputFile);
            imageCount++;
        }

        // open file if jpeg found
        if (containsJpegHeader)
        {
            sprintf(filename, "%03i.jpg", imageCount);
            outputFile = fopen(filename, "w");
        }

        // if open file then write
        if (outputFile != NULL)
        {
            fwrite(buffer, sizeof(BYTE), bytesRead, outputFile);
        }
    }

    fclose(outputFile);
    fclose(intputFile);
    return 0;
}