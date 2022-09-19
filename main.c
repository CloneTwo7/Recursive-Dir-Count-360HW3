#include <stdio.h>

int readable(char *);

int main(int argc, char **argv) {

	char* dir = argv[1];
	
	printf("reading directory: %s\n",dir);
	printf("readable returns: %d\n",readable(dir));

	return(0);

}
