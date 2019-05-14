#include <cs50.h>
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
	char current_key = keyword[0];

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
	string plaintext = get_string("Input text to encrypt: ");
	int text_len = strlen(plaintext);
	
	// Copy text into ciphertext to iterate through
	// char ciphertext[text_len];
	string ciphertext;
	strcpy(ciphertext, plaintext);

	// Iterate over entire text that needs encrypting
	encrypt(ciphertext, keyword, text_len, keyword_len);
	printf("Ciphertext is: %s\n", ciphertext);
}


// Only rotates alpha characters. If non-alpha, return inputted char
char rotateChar(char ch, char key){
	
	// Return char if non-alpha
	if(!isalpha(ch)){
		return ch;
	}

	// Rotate char by key where...
	// A = 1 rotation up (i.e. A becomes B)
	// B = 2 rotations up (i.e. A becomes C)
	char rotated_ch = toupper(ch) + key - 'A';

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
	for(int i = 0, j = 0; i != '\0'; i++, j++)
	{
		rotateChar(text[i], key[j]);

		// Keyword retuns to first letter of encryption when get to end of keyword
		if (j == key_len)
		{
			j = 0;
		}
	}
	return 0;
}