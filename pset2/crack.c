#include </home/etgonehomie/projects/cs50/libcs50-9.0.1/src/cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <crypt.h>

/*
1. program will accept only 1 command-line arguemtn: a hashed password
2. if program executed without meeting this 1 command-line argument criteria
   print an error of my choice and exit immediatly returning 1;
3. Program must proceed to crack the given password (as quickly as possible)
   printing the password in the clear followed by \n
4. Assume each password has been hashed with C's DES-based 
	(not MD5-based) crypt function
5. Assume each password is no longer than 5 characters. Gasp!
6. Assume that each password is composed entirely of alpha charc (upper/lower)

Examples:
./crack
Usage: ./crack hash

./crack 50cI2vYkF0YU2
LOL
*/

/*
DESCRIPTION
       crypt()  is  the  password  encryption  function.  It is based on the Data Encryption
       Standard algorithm with variations intended (among other things) to discourage use of
       hardware implementations of a key search.

       key is a user's typed password.

       salt  is  a  two-character  string chosen from the set [a-zA-Z0-9./].  This string is
       used to perturb the algorithm in one of 4096 different ways.

       By taking the lowest 7 bits of each of the first  eight  characters  of  the  key,  a
       56-bit  key  is  obtained.   This 56-bit key is used to encrypt repeatedly a constant
       string (usually a string consisting of all zeros).  The returned value points to  the
       encrypted  password, a series of 13 printable ASCII characters (the first two charac‐
       ters represent the salt itself).  The return value points to static data  whose  con‐
       tent is overwritten by each call.

       Warning:  the  key  space consists of 2**56 equal 7.2e16 possible values.  Exhaustive
       searches of this key space are possible using massively  parallel  computers.   Soft‐
       ware,  such as crack(1), is available which will search the portion of this key space
       that is generally used by humans for passwords.  Hence, password selection should, at
       minimum,  avoid  common  words and names.  The use of a passwd(1) program that checks
       for crackable passwords during the selection process is recommended.

       The DES algorithm itself has a few quirks which make the use of the crypt() interface
       a very poor choice for anything other than password authentication.  If you are plan‐
       ning on using the crypt() interface for a cryptography project, don't do  it:  get  a
       good book on encryption and one of the widely available DES libraries.

       crypt_r()  is  a  reentrant  version of crypt().  The structure pointed to by data is
       used to store result data and bookkeeping information.  Other than allocating it, the
       only  thing that the caller should do with this structure is to set data->initialized
       to zero before the first call to crypt_r().

       $ crypt("abc", "ab") = abFZSxKKdq5s6
       	> This says that the hashed encryption of "abc" using a salt of "ab" is abFZSxKKdq5s6
       		using the DES encryption method

*/
int main(int argc, string argv[]){
	printf("a encrypted with salt ab is %s\n", crypt("a", "ab"));
	printf("a encrypted with salt AB is %s\n", crypt("a", "AB"));

	// 312k possibilities with 5 char with upper/lower case alpha restrictions 50^5 
	// this formula is (possible letter value ^ # of letters in password)

	// what about the salt, what is the salt?
	// The salt is always the first 2 characters of the hash
	// with DES encryption the salt will be upper/lower alpha only

	exit(0);
}