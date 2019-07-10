/*
Even though JPEGs are more complicated than BMPs, JPEGs have "signatures," 
patterns of bytes that can distinguish them from other file formats. 
Specifically, the first three bytes of JPEGs are:

0xff 0xd8 0xff

from first byte to third byte, left to right. The fourth byte, 
meanwhile, is either 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 
0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, or 0xef. 
Put another way, the fourth byte’s first four bits are 1110.

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

char* createFilename(int i, char* extension);
FILE *jpgptr findJpgSignature (FILE *ptr);
bool isJpgSignature(JPEG_SIGNATURE signature);

JPEG_SIGNATURE = 4 bytes;

int FAT_FORMAT_SIZE = 512; // number of bytes in smallest memory unit chunk

typedef uint8_t BYTE;

typedef struct
{
	BYTE first;
	BYTE second;
	BYTE third;
	BYTE fourth;
} __attribute__((__packed__))
JPEG_SIGNATURE;


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
			

	
	// Iterate through file to find all the jpgs
	JPEG_SIGNATURE valid_jpeg;
	valid_jpeg.first = "0xff";
	valid_jpeg.second = "0xd8";
	valid_jpeg.third = "0xff";
			
	do {
		if (EOF){
			break;
		}
		
		// Find the jpg header signature
		findJpgSignature(inptr)
		
		// Create filename to output jpg to.
		char *outfile = createFilename(file_num, ".jpg")
		file_num++;
		FILE *outptr = fopen(filename, "w");
		
		// Error check the output file is valid
		if (outptr == NULL) {
			fclose(inptr);
			printf("Could not create %s.\n", outfile);
			return 3;
		}
		
		// Write jpg signature
		// static write the 3 bytes jpeg signature
		fwrite(valid_jpeg, sizeof(BYTE) * 3, 1, outptr);
		
		// fseek back 1 byte to get the 4th byte in signature
		fseek(inptr, 1, SEEK_CUR);
		fwrite(inptr, sizeof(BYTE), 1, outptr);
		
		// Write jpg to output.
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
	else{
		fseek(inptr, -2, SEEK_CUR);
	}
	} while (!EOF);
	
	
	

	/*
	ven though JPEGs are more complicated than BMPs, JPEGs have "signatures," 
	patterns of bytes that can distinguish them from other file formats. 
	Specifically, the first three bytes of JPEGs are:

	0xff 0xd8 0xff

	from first byte to third byte, left to right. The fourth byte, 
	meanwhile, is either 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 
	0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, or 0xef. 
	Put another way, the fourth byte’s first four bits are 1110.
	*/
}

// Scan file until can find jpg signature
// Returns EOF pointer if cannot find one.
FILE *jpgptr findJpgSignature (FILE *ptr){
	
	// Search file for JPEG Signature
	JPEG_SIGNATURE header;
	
	while(true){
		
		fread(&header, sizeof(header), 1, ptr);
		
		// Check if EOF
		if(header.first == EOF ||
			header.second == EOF ||
			header.third == EOF ||
			header.fourth == EOF)
		
		// Check if JPG signature
		if(isJpgSignature){
			return *ptr;
		}
		else {
			fseek(ptr, 2, SEEK_CUR);
		}
	}
}

bool isJpgSignature(JPEG_SIGNATURE signature){
	
	// See if first 3 are good
	if(signature.first = "0xff" &&
		signature.second = "0xd8" &&
		signature.third = "0xff") {
		
		// use bit finding here
		if(header.fourth = "1110..."){
			return true;
		}	
	}
	return false;
}

char* createFilename(int i, char* extension){
	
	char *filename = NULL;
	// Assuming only storing up to 999 files, create filename
	if (i > 99){
		filename = itoa(file_num, filename, 3);
	}
	else if(i > 9){
		filename = "0" + itoa(file_num, filename, 2);
	}
	else{
		filename = "00" + itoa(file_num, filename, 1);
	}

	return filename + extension;
}