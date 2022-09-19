#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>

int isDirectory(char*);
int isReadableFile(char*);

int readable(char *inputPath) {
	int count = 0; //counts the readable files;

	/*the following sets the workable path to the
	 * current working directory if the char* passed
	 * was NULL*/
	char workingPath[PATH_MAX];
	if(inputPath == NULL) {
		getcwd(workingPath, PATH_MAX);
	} else {
		strcpy(workingPath, inputPath);
	}

	if(isDirectory(workingPath)) {
		DIR *dr;
		struct dirent *entry;

		if((dr = opendir(workingPath)) == NULL) {
			return (0);
		} else {
			while((entry = readdir(dr))!=NULL) {
				printf("%s\t\t%10d\n", entry->d_name, isReadableFile(entry->d_name));
			}
			closedir(dr);
			return count;
		}


	} else if(isReadableFile(workingPath)) {
		return (count += 1);
	} else {
		return (0);
	}
}


int isDirectory(char *path) {
	struct stat sstat, *pstat = &sstat;
	if(stat(path, pstat) == 0) { 
		return ((S_ISDIR(pstat->st_mode)) ? 1 : 0 );
	}
	return(-1);
} 

int isReadableFile(char *path) {
	struct stat sstat, *pstat = &sstat;

	if(stat(path, pstat) == 0 && (S_ISREG(pstat->st_mode))) {
		return (pstat->st_mode & S_IRUSR);
	}
	return (-1);
} 
