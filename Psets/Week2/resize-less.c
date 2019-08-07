#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n input output\n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *input = argv[2];
    char *output = argv[3];

    // open input file
    FILE *inputFile = fopen(input, "r");
    
    if (inputFile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", input);
        return 2;
    }

    // open output file
    FILE *outputFile = fopen(output, "w");
    if (outputFile == NULL)
    {
        fclose(inputFile);
        fprintf(stderr, "Could not create %s.\n", output);
        return 3;
    }

    // read input's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inputFile);

    // read input's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inputFile);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outputFile);
        fclose(inputFile);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // set new dimensions
    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    int newWidth = oldWidth * n;
    int newHeight = oldHeight * n;

    // determine padding for scanlines
    int inPadding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outPadding = (4 - (newWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // change headers
    bi.biHeight = newHeight;
    bi.biWidth = newWidth;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * newWidth) + outPadding) * abs(newHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write output's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outputFile);

    // write output's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outputFile);

    // allocate a memory to store one scanline
    RGBTRIPLE scanline[newWidth * sizeof(RGBTRIPLE)];

    // iterate over input's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        // iterate over pixels in a scanline
        for (int j = 0; j < oldWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inputFile);

            for (int k = 0; k < n; k++)
            {
                scanline[(j * n) + k] = triple;
            }
        }

        // skip over padding, if any
        fseek(inputFile, inPadding, SEEK_CUR);

        // write the current scanline n times
        for (int j = 0; j < n; j++)
        {
            // write a new scanline once
            fwrite(scanline, sizeof(RGBTRIPLE), newWidth, outputFile);

            // write new padding
            for (int k = 0; k < outPadding; k++)
            {
                fputc(0x00, outputFile);
            }
        }
    }

    // close input
    fclose(inputFile);

    // close output
    fclose(outputFile);

    return 0;
}
