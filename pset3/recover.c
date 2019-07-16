/*
Fortunately, digital cameras tend to store photographs contiguously on 
memory cards, whereby each photo is stored immediately after the previously 
taken photo. Accordingly, the start of a JPEG usually demarks the end of 
another. However, digital cameras often initialize cards with a FAT file 
system whose "block size" is 512 bytes (B). The implication is that these 
cameras only write to those cards in units of 512 B. A photo that’s 1 MB 
(i.e., 1,048,576 B) thus takes up 1048576 ÷ 512 = 2048 "blocks" on a 
memory card. But so does a photo that’s, say, one byte smaller 
(i.e., 1,048,575 B)! The wasted space on disk is called "slack space." 
Forensic investigators often look at slack space for remnants of suspicious 
data.

Rather than read my memory card’s bytes one at a time, you can read 
512 of them at a time into a buffer for efficiency’s sake. Thanks to FAT, 
you can trust that JPEGs' signatures will be "block-aligned." 
That is, you need only look for those signatures in a block’s first 
four bytes.

A file called card.raw. So that you don’t waste time iterating over 
millions of 0s unnecessarily, I’ve only imaged the first few megabytes 
of the memory card. But you should ultimately find that the image 
contains 50 JPEGs.

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

#define JPG_SIGN_SIZE   4
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
			
	// Iterate through file to find all the jpgs			
	do {
		if (EOF){
			break;
		}
		
		// Find the first jpg header signature
		long int jpg1 = findJpgSignature(inptr);

        // Find the next jpg header signature
        fseek(inptr, 1, SEEK_CUR);
        long int jpg2 = findJpgSignature(inptr);
        fseek(inptr, -1, SEEK_CUR);

        // what to do if jpg1 or 2 is EOF?
        if (jpg1 == EOF || jpg2 == EOF) {
            
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

        // Seek back to the start of the jpg signature
        fseek(inptr, -JPG_SIGN_SIZE, SEEK_CUR);

        // Write found jpg to output file

		do{
			// store first 4 bytes
			// may need to write some of the bytes at times
			JPEG_SIGNATURE temp_jpg;
			fread(&temp_jpg, sizeof(JPEG_SIGNATURE), 1, inptr);
			fseek(inptr, sizeof(JPEG_SIGNATURE), SEEK_CUR);
			if(isJpgSignature(temp_jpg)){
				break;
			}
			
			else {
				// Do i need to subtract the 4 bytes from jpg header?
				// before using the FAT_FORMAT_SIZE?
				fwrite(inptr, FAT_FORMAT_SIZE, 1, outptr);
			}
	
		}while (true);
	} while (true);

    
		
	// Move back 2 bytes 
    do{
        fseek(inptr, -2, SEEK_CUR);
    } while (!EOF);
}

// Scan file from given pointer until it can find jpg signature.
// Returns the position of the start of the Jpg signature if found.
// If not found, then return EOF pointer.
long int findJpgSignature (FILE *ptr){
	
    // Keep track of original ptr position
    long int orginal_ptr = ftell(ptr);

    // Temporary storage for header
    JPG_HEADER header;

	while(true) {

        // read in a jpg_header to test if valid        
		fread(&header, sizeof(JPG_HEADER), 1, ptr);
		
		// If any value is EOF return call
		if(header.byte_1 == EOF || header.byte_2 == EOF ||
			header.byte_3 == EOF || header.byte_4 == EOF) {
            return EOF;
        }
		
		// Move pointers and return if JPG signature valid
		if(isJpgSignature) {

            // Move ptr to beginning of jpg header
            fseek(ptr, -sizeof(JPG_HEADER), SEEK_CUR);
            long int jpg_header_ptr = ftell(ptr);

            // Set read file pointer back to original position
            fsetpos(ptr, orginal_ptr); 

            // return the jpg signature ptr
			return jpg_header_ptr;
		}

        // Move back read pointer if the previously read values can be part of the jpg header
		else {
            if (header.byte_2 == "0xff" && header.byte_3 == "0xd8" && header.byte_4 == "0xff") {
                fseek(ptr, -3, SEEK_CUR);    
            }
            else if (header.byte_3 = "0xff" && header.byte_4 == "0xd8") {
                fseek(ptr, -2, SEEK_CUR); 
            }
            else if (header.byte_4 = "0xff") {
                fseek(ptr, -1, SEEK_CUR);
            }
		}
	}
}

bool isJpgSignature(JPG_HEADER header) {
	// See if first 3 are good
	if(header.byte_1 == "0xff" &&
		header.byte_2 == "0xd8" &&
		header.byte_3 == "0xff") {
		
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