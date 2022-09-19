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
	getcwd(workingPath, PATH_MAX);
	if(inputPath == NULL) {
		getcwd(workingPath, PATH_MAX);
	} else {
		strcpy(workingPath, inputPath);
	}
	if(chdir(workingPath) != 0) return (-1);
	getcwd(workingPath, PATH_MAX);

	if(isReadableFile(workingPath)) {
		count++;
	}

	else if(isDirectory(workingPath)) {
		DIR *dr;
		struct dirent *entry;

		if((dr = opendir(workingPath)) == NULL) {
			chdir("..");
			closedir(dr);
		} else {
			if(chdir(workingPath)) {
				return (-1);
			}
			/*The following loop traverses all elements of the directory
			 *if another directory is discovered, it adds the count of
			 * that directory to the total count for this function*/
			while((entry = readdir(dr))!=NULL) {
				if(isReadableFile(entry->d_name)) {
					/* if it is a readable file, increment count by 1 */
					count++;
				} else if(isDirectory(entry->d_name)) {
					/*increment by the readable() value of the directory if
					 *the entry is a directory */
					count += readable(entry->d_name);
				}
			}
			chdir("..");
			closedir(dr);
		}
	} 

	return(count);
}


int isDirectory(char *path) {
	struct stat sstat, *pstat = &sstat;
	if(lstat(path, pstat) == 0 && path[0] != '.') { 
		return ((S_ISDIR(pstat->st_mode)) ? 1 : 0 );
	}
	return(0);
} 

int isReadableFile(char *path) {
	struct stat sstat, *pstat = &sstat;
	if(lstat(path, pstat) == 0 && (S_ISREG(pstat->st_mode)) && !(isDirectory(path))) {
		return (pstat->st_mode & S_IRUSR);
	}
	return (0);
} 
