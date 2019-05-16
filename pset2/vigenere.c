#include </home/etgonehomie/projects/cs50/libcs50-9.0.1/src/cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
The Vigenere encryption uses a keyphrase to encrypt messages. 
For example, if the keyword is "dog" the shift would be
4 (d) -> 14 (o) -> 6 (g)
So if message was "hello world" it would encrypt by:
shift h by 3 (d)
shift e by 14 (o)
shift l by 6 (g)
shift l by 3 (d)
etc...

A is a shift of 0
Z is a shift of 25

At execution of program the user will type a keyword to use for encryption
(i.e. /vigenere.c testkey)
	> If user does not input keyword OR 
		inputs non-alpha characters (including spaces, digits, symbols)
	  	end the execution with message "Usage ./vigenere keyword"

After execution, prompt user for plaintext "plaintext: "
Then program will use the keyword to encrypt the message and display the
message on the next line as "ciphertext: ___________"
*/
char rotateChar(char ch, char key);
int encrypt(string text, string key, int text_len, int key_len);
int main(int argc, string argv[])
{	
	
	// Ensure user inputs only one encryption keyword during execution
	if(argc != 2)
	{
		printf("Usage ./vigenere keyword\n");
		return 1;
	}
	string keyword = argv[1];
	int keyword_len = strlen(keyword);

	// Ensure keyword is all alpha
	for(int i = 0; i < keyword_len; i++)
	{
		if(!isalpha(keyword[i]))
		{
			printf("Usage ./vigenere keyword\n");
			return 1;
		}
	}
	
	// Ask user for message to encrypt
	string plaintext = get_string("plaintext: ");
	int text_len = strlen(plaintext);

	// Copy text into ciphertext to iterate through
	string ciphertext = plaintext;

	// Iterate over entire text that needs encrypting
	encrypt(ciphertext, keyword, text_len, keyword_len);
	printf("ciphertext: %s\n", ciphertext);
}


// Only rotates alpha characters. If non-alpha, return inputted char
char rotateChar(char ch, char key){
	int num_of_letters = 26;
	// int normalize_key = toupper(key) - 'A';
	// Return char if non-alpha
	if(!isalpha(ch)){
		return ch;
	}

	// Rotate char by key where...
	// A = 0 rotation up (i.e. Z becomes Z)
	// B = 1 rotations up (i.e. Z becomes A)
	// printf("upper ch: %i %c\n", toupper(ch), toupper(ch));
	// printf("upper key: %i %c\n", toupper(key), toupper(key));


	char rotated_ch = ((toupper(ch) - 'A' + toupper(key) - 'A') % 26) + 'A';

	// printf("ch: %c : %c\n", ch, rotated_ch);

	// Return the original case of the character
	if(isupper(ch))
	{
		return rotated_ch;
	}
	
	return tolower(rotated_ch);
}

// Encrypt message
int encrypt(string text, string key, int text_len, int key_len){
	// can i modify string directly? or do i have to copy and make an array?
	// i should be able to modify directly
	// printf("plaintext: %s\n", text);
	// printf("key: %s\n", key);
	for(int i = 0, j = 0; i < text_len; i++)
	{
		// Only rotate through key if alpha character
		if(isalpha(text[i])){
			// printf("j beginning of loop is = %i\n", j); 	
			text[i] = rotateChar(text[i], key[j]);
			j++;
		}

		
		// Keyword retuns to first letter of encryption when get to end of keyword
		if (j == key_len)
		{
			j = 0;
		}
		// printf("j end of loop is = %i\n", j);
	}
	return 0;
}