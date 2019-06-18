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

float round_f(float f, int sig_digit);
bool isFloat(char* f);

int TENTH = 10;
int HUNDRETH = 100;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize f infile outfile\n");
        return 1;
    }

    
    // Define the inputted float
    char* temp_f = argv[1];
    float resize = 0;
    
    // Defines full int value and each decimal digit of the inputted float
    // This enables logic to know how many dupes to make per pixel
    // and what pixels to dupe.
    int resize_full_int = 0;
    int resize_tenth_decimal = 0;
    int resize_hund_decimal = 0;

    // Defines what pixels to dupe based on the inputted resize value.
    // Tells what pixel to dupe based on the 10th decimal and 100th decimal.
    int dupe_tenth = 0;
    int dupe_hund = 0;

    // Variables to define what pixel in a row or column I am in
    // This will help know what 10th and 100th place pixels to dupe
    int pixel_row = 0;     
    int pixel_col = 0; 

    // Ensure 2md variable is a float
    if(!is_float(temp_f)){
        printf("Must input a valid float\n");
        return 1;
    }
    else{
        resize = strtof(temp_f, NULL);
        resize = round_f(resize, 2);
        resize_full_int = round(resize); //ensure this just rounds down all the time
        resize_tenth_decimal = round((resize - resize_full_int) * TENTH);
        resize_hund_decimal = round((resize - resize_full_int) * HUNDRETH) - (resize_tenth_decimal * TENTH);

        // Define which pixels need to be duped for the tenth and hundreth places
        // ensure I handle 0s good
        dupe_tenth = TENTH / resize_tenth_decimal;
        dupe_hund = HUNDRETH / resize_hund_decimal;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

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
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
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

bool isFloat(char* f){
    if (strlen(f) > 0 && !isspace((unsigned char) f[0])){
        char *tail;
        errno = 0;
        float f = strtof(f, &tail);
        if (errno == 0 && *tail == '\0' && isfinite(f) != 0 && f < FLT_MAX)
        {
            // Disallow hexadecimal and exponents
            if (strcspn(f, "XxEePp") == strlen(f))
            {
                return true;
            }
        }
    }
}

// Round a float 'f' to the significant digit 'sig_digit'
// 2 means round to the 100th place (i.e. 12.345 becomes 12.35)
// can only round to the 5th digit, else it will return same float passed
float round_f(float f, int sig_digit){
    int max_sig_digit = 5;
    
    // Only allow rounding to max of 5 significant digits
    if (sig_digit > max_sig_digit){
        return f;
    }

    float multiplier = pow(10.0, sig_digit);
    int temp_f = round(f * multiplier);
    return temp_f / multiplier;
}