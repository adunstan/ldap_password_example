

#include <stdio.h>
#include <string.h>
#include "playfair.h"


int main(int argc, char ** argv, char ** env)
{
	static char *key_container = "asnxhimbqhioxqmvnhkimxelqhigormgxewjhizfqm";

	char *key = strndup(key_container + 6, 14);
	char *plaintext = strdup(argv[1]);

	encryptByPlayfairCipher(plaintext, key);
	printf("%s\n",plaintext);
}


