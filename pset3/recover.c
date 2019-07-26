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

// Method definitions
fpos_t findJpgSignature (FILE *ptr);
bool isJpgSignature(JPG_HEADER header);
char* createFilename(int i, char* extension);

int main(int argc, char *argv[])
{	
	int file_num = 93; // Keep track of number of jpgs found

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
	
	// Initialize jpg start
	fpos_t jpg_start;
	
	// // Move forward 1 FAT block to not get the same jpg signature
	// fseek(inptr, FAT_BLOCK_SIZE, SEEK_CUR);
	
	// // Find the 2nd jpg header signature (aka end of 1st jpg header)
	// fpos_t jpg_end = findJpgSignature(inptr);
	
	// // Move back 1 FAT block to go to beginning of first jpg
	// fseek(inptr, -FAT_BLOCK_SIZE, SEEK_CUR);

	// Iterate through file to find all the jpgs until EOF		
	while (!feof(inptr)) {

		// Find jpg and store ptr
		jpg_start = findJpgSignature(inptr);

		// Set file ptr to start of jpg
		// shouldn't need to set to start because it already is the start
		// after finding the jpg
		// fsetpos(inptr, &jpg_start); 
        
		// Create filename to output jpg file
		char *outfile = createFilename(file_num, "jpg");
		printf("outfilename %s", outfile);
		FILE *outptr = fopen(outfile, "w");	
		file_num++; // increment file number count
		
		// Error check the output file is valid
		if (outptr == NULL) {
			fclose(inptr);
			printf("Could not create %s.\n", outfile);
			return 3;
		}

        // Write jpg FAT blocks to output file until EOF or start of new jpeg
		while (!feof(inptr)) 
		{	
			// Write in FAT_BLOCK size chunks  
			FAT_BLOCK chunk;
			JPG_HEADER header;
			fpos_t chunk_ptr;

			fread(&chunk, FAT_BLOCK_SIZE, 1, inptr);
	    	fwrite(&chunk, FAT_BLOCK_SIZE, 1, outptr);

			
			// Read in a jpg_header and reset ptr to original position
			fgetpos(inptr, &chunk_ptr); 
			fread(&header, sizeof(JPG_HEADER), 1, inptr);
        	fsetpos(inptr, &chunk_ptr);	
			
			// Break if start of new jpeg
			if(isJpgSignature(header)){
				break;
			}
		}
	}
}

// Scan file from given pointer until it can find jpg signature.
// Returns the position of the start of the Jpg signature if found.
// If not found, then return EOF pointer.
// Always resets ptr to original position from when it was called
fpos_t findJpgSignature (FILE *ptr){
	
	// Initialize ptrs
	fpos_t original_ptr;
	fpos_t jpg_header_ptr;
	fgetpos(ptr, &original_ptr);
	jpg_header_ptr = original_ptr;
	
	// Scan through input file until find a jpg signature
	// If EOF return ptr immediately
	while(!feof(ptr)) {
		
        // Read in a jpg_header and reset ptr to jpg start position
		JPG_HEADER header;
		fread(&header, sizeof(JPG_HEADER), 1, ptr);
        fsetpos(ptr, &jpg_header_ptr);	

		// Return ptr value if jpg signature found
		if(isJpgSignature(header)) {
			fsetpos(ptr, &original_ptr);
			return jpg_header_ptr;
		}

        // Move to next FAT Block if jpg signature not found
		else {
			fseek(ptr, FAT_BLOCK_SIZE, SEEK_CUR);
			fgetpos(ptr, &jpg_header_ptr);
		}
	}

	fsetpos(ptr, &original_ptr);
	return original_ptr;
}

// Define if a given set of 4 bytes is a jpg signature
bool isJpgSignature(JPG_HEADER header) {
	printf("int 0xff = %d\n", 0xff);
	printf("byte1 = %d\n", header.byte_1);
	printf("byte3 = %d\n\n", header.byte_3);

	printf("int 0xd8 = %d\n", 0xd8);
	printf("byte2 = %d\n", header.byte_2);
	printf("***************************\n");
	// See if first 3 are good
	if(header.byte_1 == 0xff && header.byte_2 == 0xd8 &&
		header.byte_3 == 0xff) 
	{
		
		// use bit finding here instead
        // Put another way, the fourth byte’s first four bits are 1110.
		if(header.byte_4 == 0xe0 || header.byte_4 == 0xe1 || header.byte_4 == 0xe2 ||
            header.byte_4 == 0xe3 || header.byte_4 == 0xe4 || header.byte_4 == 0xe5 ||
            header.byte_4 == 0xe6 || header.byte_4 == 0xe7 || header.byte_4 == 0xe8 ||
            header.byte_4 == 0xe9 || header.byte_4 == 0xea || header.byte_4 == 0xeb ||
            header.byte_4 == 0xec || header.byte_4 == 0xed || header.byte_4 == 0xee ||
            header.byte_4 == 0xef)
        {
			return true;
		}	
	}
	return false;
}

// Creates a numbered filename with a given number and extension
// extension does not include the '.' character
char* createFilename(int i, char* extension){
	

	int file_len = 6 + strlen(extension);
	char filename[file_len];
	// Assuming only storing up to 999 files, create filename
	snprintf(filename, file_len, "%i", i);
	if(i < 10) {
		snprintf(filename, file_len, "00%i.%s", i, extension);	
	}

	else if (i < 100) {
		snprintf(filename, file_len, "0%i.%s", i, extension);
	}
	else {
		snprintf(filename, file_len, "%i.%s", i, extension);
	}

	printf("filename is %s", filename);
	
	return filename;
	
}
