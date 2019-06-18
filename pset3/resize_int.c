/*
Implement a program called resize that resizes (i.e., enlarges or shrinks) 24-bit uncompressed BMPs by a factor of f.
Implement your program in a file called resize.c inside of ~/workspace/pset3/resize/more/.
Your program should accept exactly three command-line arguments, whereby
the first (f) must be a floating-point value in (0.0, 100.0],
the second must be the name of a BMP to be resized, and
the third must be the name of the resized version to be written.
+ If your program is not executed with such, it should remind the user of correct usage, as with printf, and main should return 1.
Your program, if it uses malloc, must not leak any memory.
*/

// Copies a BMP file
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

int strtoi(char* i);

int main(int argc, char *argv[])
{
    // Ensure proper usage of calling the program
    if (argc != 4)
    {
        printf("Usage: ./resize i infile outfile\n");
        return 1;
    }

    // Store the inputted parameters
    int resize = 0;
    char *infile = argv[2];
    char *outfile = argv[3];

    // Ensure 2nd variable is an int > 0
    resize = strtoi(argv[1]);
    if (resize < 1) {
        printf("Must input a valid int greater than 0\n");
        return 1;
    }
    
    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER

    // modify outfile bitmap info header
    // width = round(bi.biWidth * resize + 0.5)
    // bi.biHeight  = round(bi.biHeight * resize + 0.5)
    // size = bi.biHeight * bi.biWidth

    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // determine padding for scanlines
    // padding will change according to the resize.
    /*
    say width is 10 pixels
    that would be a padding of 2 (4 + 4 + 2 with 2 padding since its group of 4)
    say multiple width by 2 (20 % 4 = 0, so 0 padding)
    say multiply width by 2.25 (22.5), round this up to 23 (23 % 4 = 1, so 1 padding)

    */
    // padding:
    // say it is 10 pixels
    

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // START RESIZING HERE
    /**
     * for every digit do "decimal place (ie 10, 100, 1000, / digit"
     * 
     * for 2.49 it would be
     * 10/4 = 2 --> dupe every 2nd digit then when count is 4, skip to 10 and start at
     *              1 again
     * 100/9 = 11  --> dupe every 11th digit, then when it hits 100 and start at 1 again
     *  
     * 
     * **/
    // TO-DO

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // Set the pixel col
        if(pixel_col > HUNDRETH){
            pixel_col = 0;
        }
        pixel_col++;

        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // Set the pixel row
            if(pixel_row > TENTH){
                pixel_row = 0;
            }
            pixel_row++;

            // Only dupe if full int of resize is non-zero
            if(resize_full_int > 0){ 

            }

            // Only dupe 10th place if it is a non-zero
            if(resize_tenth_decimal > 0){

            }

            // Only dupe 100th place if it is a non-zero
            if(resize_hund_decimal > 0){
                
            }

            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

int strtoi(char* i){
    
    // Return error if int is negative value
    if (i[0] == '-')
        return -1;

     // Return an true if only an int (in range) was provided
    char *tail;
    errno = 0;
    long n = strtol(i, &tail, 10);
    if (errno == 0 && *tail == '\0' && n >= INT_MIN && n < INT_MAX)
        return n;
    
    return -1;
}