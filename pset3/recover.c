/*
Implement a program called recover that recovers JPEGs from a 
forensic image.

Implement your program in a file called recover.c in a directory 
called recover.
Your program should accept exactly one command-line argument, 
the name of a forensic image from which to recover JPEGs. 
+ If your program is not executed with exactly one command-line argument, 
it should remind the user of correct usage, as with fprintf (to stderr), 
and main should return 1.
If the forensic image cannot be opened for reading, 
your program should inform the user as much, 
as with fprintf (to stderr), and  main should return 2.
Your program, if it uses malloc, must not leak any memory.
When executed, your program should recover every one of the JPEGs 
from card.raw, storing each as a separate file in your current 
working directory. Your program should number the files it outputs 
by naming each ###.jpg, where ### is three-digit decimal number from 
000 on up.
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

#define JPG_SIGN_SIZE       4
#define FAT_BLOCK_SIZE      512     // number of bytes in smallest memory unit chunk

typedef uint8_t  BYTE;

typedef struct
{
	BYTE byte_1;
	BYTE byte_2;
	BYTE byte_3;
	BYTE byte_4;
} __attribute__((__packed__))
JPG_HEADER;

typedef struct 
{
    BYTE fat_bytes[FAT_BLOCK_SIZE];
} __attribute__((__packed__))
FAT_BLOCK;

char* createFilename(int i, char* extension);
long int findJpgSignature (FILE *ptr);
bool isJpgSignature(JPG_HEADER header);

int main(int argc, char *argv[])
{	
	int file_num = 0; // Keep track of number of jpgs found

	// Ensure proper usage of function
	if (argc != 2)
	{
		printf("Usage: ./recover corrupted_file\n");
		return 1;
	}

	// Open file
	char *infile = argv[1];
	FILE *inptr = fopen(infile, "r");
	if (inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    file_num++; // increment file number count
	
	// Find the 1st jpg header signature
	long int jpg_start = findJpgSignature(inptr);
	
	// Move forward 1 FAT block to not get the same jpg signature
	fseek(inptr, FAT_BLOCK_SIZE, SEEK_CUR);
	
	// Find the 2nd jpg header signature (aka end of 1st jpg header)
	long int jpg_end = findJpgSignature(inptr);
	
	// Iterate through file to find all the jpgs			
	do {
		
		// Set file ptr to start of jpg
		fsetpos(inptr, jpg_start); 
		
		if (inptr == EOF){
			break;
		}
        
		// Create filename to output jpg file
		char *outfile = createFilename(file_num, ".jpg");
		FILE *outptr = fopen(outfile, "w");	
		
		// Error check the output file is valid
		if (outptr == NULL) {
			fclose(inptr);
			printf("Could not create %s.\n", outfile);
			return 3;
		}

        // Write found jpg to output file
		do {
			// Write in FAT_BLOCK size chunks  
			FAT_BLOCK chunk;
			fread(&chunk, FAT_BLOCK_SIZE, 1, inptr);			
            fwrite(&chunk, FAT_BLOCK_SIZE, 1, outptr);
			
		} while ((ftell(inptr) < jpg_end && jpg_end <> EOF) || ftell(inptr) == EOF);  
		
		// Set new start and end for next jpg
		jpg_start = jpg_end; 	//jpg_end should == ftell(inptr)
		jpg_end = findJpgSignature(inptr);

	} while (jpg_start == EOF);
}

// Scan file from given pointer until it can find jpg signature.
// Returns the position of the start of the Jpg signature if found.
// If not found, then return EOF pointer.
long int findJpgSignature (FILE *ptr){
	
	// Initialize ptrs
	long int original_ptr = ftell(ptr);
	long int jpg_header_ptr = orginal_ptr;  
	
    // Temporary storage for header
    JPG_HEADER header;
	
	// Scan through input file until find a jpg signature
	while(true) {
		
        // Read in a jpg_header to test if valid
		fread(&header, sizeof(JPG_HEADER), 1, ptr);
		
		// If 1st byte is EOF return call
		if(header.byte_1 == EOF) {
		
			// Set read file pointer back to original position
            fsetpos(ptr, orginal_ptr);	
			return EOF;
        }

		// Return ptr value if jpg signature found
		if(isJpgSignature(header)) {
			
            // Set read file pointer back to original position
            fsetpos(ptr, orginal_ptr);

            // return the jpg signature ptr
			return jpg_header_ptr;
		}

        // Move to next FAT Block if jpg signature not found
		else {
			fseek(inptr, FAT_BLOCK_SIZE - sizeof(JPG_HEADER), SEEK_CUR);
			jpg_header_ptr = ftell(ptr);
		}
	}
}

// Define if a given set of 4 bytes is a jpg signature
bool isJpgSignature(JPG_HEADER header) {
	// See if first 3 are good
	if(header.byte_1 == "0xff" && header.byte_2 == "0xd8" &&
		header.byte_3 == "0xff") 
	{
		
		// use bit finding here instead
        // Put another way, the fourth byte’s first four bits are 1110.
		if(header.byte_4 == "0xe0" || header.byte_4 == "0xe1" header.byte_4 == "0xe2"
            header.byte_4 == "0xe3" || header.byte_4 == "0xe4" header.byte_4 == "0xe5"
            header.byte_4 == "0xe6" || header.byte_4 == "0xe7" header.byte_4 == "0xe8"
            header.byte_4 == "0xe9" || header.byte_4 == "0xea" header.byte_4 == "0xeb"
            header.byte_4 == "0xec" || header.byte_4 == "0xed" header.byte_4 == "0xee"
            header.byte_4 == "0xef")
        {
			return true;
		}	
	}
	return false;
}

// Creates a numbered filename with a given number and extension
char* createFilename(int i, char* extension){
	
	char *filename = NULL;
	// Assuming only storing up to 999 files, create filename
	if (i > 99){
		filename = itoa(i, filename, 3);
	}
	else if(i > 9){
		filename = "0" + itoa(i, filename, 2);
	}
	else{
		filename = "00" + itoa(i, filename, 1);
	}

	return strcat(filename, extension);
}
