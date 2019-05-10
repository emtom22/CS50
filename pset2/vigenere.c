#include <cs50.h>
#include <stdio.h>

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

int main(int argc, string argv[])
{

}
