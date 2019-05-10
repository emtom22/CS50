/*
American Express uses 15-digit numbers, 
MasterCard uses 16-digit numbers, and 
Visa uses 13- and 16-digit numbers. 

All American Express numbers start with 34 or 37; 
most MasterCard numbers start with 51, 52, 53, 54, or 55; 
5555555555554444
and all Visa numbers start with 4. 

https://developer.paypal.com/docs/classic/payflow/payflow-pro/payflow-pro-testing/#credit-card-numbers-for-testing
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cs50.h>


long long inputCCNum(void);
int getDigit(long long num, int position);
string getCreditType(long long num);
bool validate_checksum(long long num);
int getChecksum(long long num);
int numberToArray(long long num, int * array, int length);
// int reverseArray(int * array, int * rev_array, int length);

int main(void)
{
	
	// Get user inputted CC Num and validat if it follows conventions
	long long cc_num;
	// cc_num = 5105105105105100;
	cc_num = inputCCNum();

	string cc_type = getCreditType(cc_num);
	if (strcmp(cc_type,"INVALID") == 0){
		printf("INVALID\n");
		return 0;
	}
	// If cc_type is not INVALID, then do checksum
	int checksum = getChecksum(cc_num);

	// Check if last digt of checksum = 0

    if(getDigit(checksum, 1) != 0){
        cc_type = "INVALID";
    }

    printf("%s\n", cc_type);

}

// Get user inputted CC number > 0
long long inputCCNum(void){
	long long num;

	do {
		num = get_long("Number: ");
	} while (num < 1);

	return num;
}

// Get the digit of a long long number @ given position
int getDigit(long long num, int position){
    long long divisor1 = pow(10, position - 1);
    long long divisor2 = 0;
    int length = 1;

    //  Can't log10 num = 0
    if(num != 0){
    	length = floor(log10(num)) + 1;
    }

    // Get the desired number to be at last digit placement
    long long digit = num / divisor1;
    
    // Truncate down to the last digit
    for(int i = length; i > 0; i--){
        divisor2 = pow(10, i);
        digit = digit % divisor2;
    }

	return digit;
}

string getCreditType(long long num){
	// Set valid cc lengths for major CC types
	const int VALID_AMEX_LEN = 15;
	const int VALID_MC_LEN = 16;
	const int VALID_VISA_LEN[] = {13, 16};

	string cc_type = "INVALID";
	int cc_length = 0;
	int first_digit = 0; 
	int second_digit = 0;
	
	// Assumes num is > 0
	cc_length = floor(log10(num)) + 1;

	// Get first 2 digits of the CC number
	if (cc_length > 10) 
	{
		first_digit = getDigit(num, cc_length);
		second_digit = getDigit(num, cc_length - 1);
	}
	// Find if valid CC number depending on CC type

	// Visa: 13 or 16 digits and starts with 4
	if ((cc_length == VALID_VISA_LEN[0] || cc_length == VALID_VISA_LEN[1]) 
		&& first_digit == 4)
	{
		cc_type = "VISA";
	}
	
	// MasterCard: 16 digits and starts with 51-55
	if (cc_length == VALID_MC_LEN
	 && first_digit == 5 && second_digit <= 5 && second_digit >= 1)
	{
		cc_type = "MASTERCARD";
	}

	// Amex: 15 digits and starts with 34 or 37
	if (cc_length == VALID_AMEX_LEN && first_digit == 3 && (
		second_digit == 4 || second_digit == 7))
	{
		cc_type = "AMEX";
	}
	return cc_type;
}

int getChecksum(long long num){
    int checksum = 0;

    // this assumes that # of digits > 0
    int cc_len = 1;

    if (num != 0){
        cc_len = floor(log10(num)) + 1;
    }

    int digit_array[cc_len];
    int product = 0;

    numberToArray(num, digit_array, cc_len);

    // Calculate checksum
    for(int i = 0; i < cc_len; i++){
        if(i % 2 == 0){
            checksum += digit_array[i];
        }
        else{
            product = digit_array[i] * 2;

            // Get each digit 1 or 2 assuming 18 (9*2) is highest)
            checksum += getDigit(product, 1);


            if (product > 9){
                checksum += getDigit(product, 2);
            }
        }
    }
    return checksum;
}

// Converts a number to an array of ints where...
// a[0] = 10s
// a[1] = 100s
// a[2] = 1000s
int numberToArray(long long num, int * array, int length){
    for(int i = 1; i <= length; i++){
        array[i-1] = getDigit(num, i);
    }
    return 0;
}