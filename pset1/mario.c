/**
Let’s recreate those pyramids in C, albeit in text, using hashes (#) 
for bricks, a la the below. Each hash is a bit taller than it is wide, 
so the pyramids themselves are also be taller than they are wide.

   #  #
  ##  ##
 ###  ###
####  ####
The program we’ll write will be called mario. And let’s allow the 
user to decide just how tall the pyramids should be by first prompting 
them for a positive integer between, say, 1 and 8, inclusive.

Here’s how the program might work if the user inputs 8 when prompted:

$ ./mario
Height: 8
       #  #
      ##  ##
     ###  ###
    ####  ####
   #####  #####
  ######  ######
 #######  #######
########  ########

If the user doesn’t, in fact, input a positive integer between 1 and 8, 
inclusive, when prompted, the program should re-prompt the user until 
they cooperate:

Notice that width of the “gap” between adjacent pyramids is equal 
to the width of two hashes, irrespective of the pyramids’ heights.

*/
#include <stdio.h>

void printPyramid(int n);
void printSteps(int n);


int main(void)
{
    // Prompt user for postive int between 1-8
    int height;
    int min_bound = 1;
    int max_bound = 8;
    do{
    	height = get_int("Height: ");
    } while (height < min_bound || height > max_bound);

    // Print out pyramid lines
    printPyramid(height);
}

// Helper function to print out a step of n length
void printSteps(int n)
{
	for(int i = 1; i <= n; i++) {
		printf("#");
	}
}

// Function to create a pyramid of steps for given height
void printPyramid(int height)
{
	// Print out smallest to largest line
	for(int i = 1; i <= height; i++) {
		// Print empty spaces to ensure pyramid stacking
		for(int j = 0; j < height - i; j++){
			printf(" ");
		}
		printSteps(i);
		printf("  ");
		printSteps(i);
		printf("\n");
	}
}