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
		
	// Create new BITMAP headers
	BITMAPFILEHEADER bf_new = bf; // Does this copy or just create a pointer to the same?
	BITMAPINFOHEADER bi_new = bi; // Does this copy or just create a pointer to the same?

	// Resize file info variables
	bi_new.biWidth =* resize;
	bi_new.biHeight =* resize;
	
	// Determine if any padding needed for width
	int padding = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
	
	bi_new.biSizeImage = abs((bi_new.biWidth + padding) * bi_new.biHeight);
		
	// Resize file header variables
	bf_new.biSize = sizeof(bf_new) + sizeof(bi_new) + bi_new.biSizeImage;
	
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    // START RESIZING HERE

    // iterate over infile's scanlines
	for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
	{
		// Write entire row to outfile resized times
		int num_col_dupes = 0;
		do {
			// iterate over pixels in scanline
			for (int j = 0; j < bi.biWidth; j++)
			{
				// temporary storage
				RGBTRIPLE triple;

				// read RGB triple from infile
				fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

				// write RGB triple to outfile resized times
				int num_row_dupes = 0;
				do {
					fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
		
					num_row_dupes++;
				} while (resize > num_row_dupes);
			}

			// skip over padding, if any
			fseek(inptr, padding, SEEK_CUR);

			// then add it back (to demonstrate how)
			for (int k = 0; k < padding; k++)
			{
				fputc(0x00, outptr);
			}
			
			// Reset pointer to beginning of row if still need to resize
			num_col_dupes++;
			if (resize > num_col_dupes) {
				fseek(inptr, (bi.biWidth * -1), SEEK_CUR);
			}
		} while (resize > num_col_dupes);
	}

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

int strtoi(char* i){
    
     // Return an true if only a valid int was provided
    char *tail;
    errno = 0;
    long n = strtol(i, &tail, 10);
    if (errno == 0 && *tail == '\0' && n >= INT_MIN && n < INT_MAX)
        return n;
    
    return -1;
}