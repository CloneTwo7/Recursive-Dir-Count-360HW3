#include <stdio.h>

int readable(char *);

int main(int argc, char **argv) {

	char* dir = argv[1];
	
	printf("readable returns: %d\n",readable(dir));

	return(0);

}
