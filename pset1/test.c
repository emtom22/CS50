
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(void)
{
	long num;

	do {
		printf("Number: ");
		num = get_long("Number: ");
	} while (num < 1);	

	printf("num: %ld\n", num);
}