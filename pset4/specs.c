
/*
Implement load, size, check, and unload as efficiently as possible using
a hash table in such a way that TIME IN load, TIME IN check,
TIME IN size, and TIME IN unload are all minimized.
This problem is your chance to design.  Although we invite you to
minimize space, your ultimate enemy is time.

SPECIFICATIONS
1. You may not alter speller.c or Makefile.
2. You may alter dictionary.c (and, in fact, must in order to complete
    the implementations of load, size, check, and unload), but you may not
    alter the declarations (i.e., prototypes/function signature)
    of load, size, check, or unload. You may, though, add new functions
    and (local or global) variables to dictionary.c.
3. You may alter dictionary.h, but you may not alter the declarations of
    load, size, check, or unload    
4. Your implementation of check must be case-insensitive. In other words,
    if foo is in dictionary, then check should return true given any
    capitalization thereof; none of foo, foO, fOo, fOO, fOO, Foo, FoO,
    FOo, and FOO should be considered misspelled.
5. Capitalization aside, your implementation of check should only return
    true for words actually in dictionary. Beware hard-coding common words
    (e.g., the), lest we pass your implementation a dictionary without those
    same words. Moreover, the only possessives allowed are those actually
    in dictionary. In other words, even if foo is in dictionary, check should
    return false given foo’s if foo’s is not also in dictionary.
6. You may assume that any dictionary passed to your program will be structured
    exactly like ours, alphabetically sorted from top to bottom with one word
    per line, each of which ends with \n. You may also assume that dictionary
    will contain at least one word, that no word will be longer than LENGTH
    (a constant defined in dictionary.h) characters, that no word will appear
    more than once, that each word will contain only lowercase alphabetical
    characters and possibly apostrophes, and that no word will start with an apostrophe.
7. You may assume that check will only be passed words that contain (uppercase
    or lowercase) alphabetical characters and possibly apostrophes.
8. Your spell checker may only take text and, optionally, dictionary as input.
    Although you might be inclined (particularly if among those more
    comfortable) to "pre-process" our default dictionary in order to derive an
    "ideal hash function" for it, you may not save the output of any such
    pre-processing to disk in order to load it back into memory on subsequent
    runs of your spell checker in order to gain an advantage.
9. You may alter the value of N and the implementation of hash.
10. Your spell checker must not leak any memory. Be sure to check for leaks with valgrind.
*/