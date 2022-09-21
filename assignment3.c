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
int isNonReadableFile(char*);

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

	/* First, check if the path passed is a file
	 * If it is, check if it is readable or nonreadable*/
	if(isReadableFile(workingPath)) {
		count++;
		return(count);
	} else if(isNonReadableFile(workingPath)) {
		return(0);
	}

	/* Since the path is not a file, we change into
	 * the path provided so we ensure we're operating
	 * in the directory provided */
	if(chdir(workingPath)) return (-errno);
	getcwd(workingPath, PATH_MAX);

	/* Now we can start processing the directory recursively */
	int check = 0; /* check verifies that no errno was returned */
	if(check = isDirectory(workingPath)) {
		if(check <= 0) return (-errno);
		DIR *dr;
		struct dirent *entry;

		/*Appropriate error checking in case cannot
		 * open the directory*/
		if((dr = opendir(workingPath)) == NULL) {
			/*If the directory is null, we return errno, close, and return*/
			if(chdir("..")) return (-errno);
			closedir(dr);
			return (-errno);
		} else {
			/*Otherwise, begin navigating the contents of the directory...
			 * The following loop traverses all elements of the directory
			 *if another directory is discovered, it adds the count of
			 * that directory to the total count for this function*/
			while((entry = readdir(dr))!=NULL) {
				if(isReadableFile(entry->d_name)) {
					/* if it is a readable file, increment count by 1 */
					count++;
				} else if(isDirectory(entry->d_name)) {
					/*increment by the readable() value of the directory if
					 *the entry is a directory */
					int flag = readable(entry->d_name);
					if(flag < 0) flag = 0;
					count += flag;
				}
			}
			//Navigate out of that directory before returning
			if(chdir("..")) return (-errno);
			closedir(dr);
		}
	} 
	//return the total count incremented
	return(count);
}


/* The following helper directories exist to help smooth over some repetetive calls
 * mainly, the work to check if a directory passed to it is readable and exists*/
int isDirectory(char *path) {
	struct stat sstat, *pstat = &sstat;
	if(!strcmp(path, ".") || !strcmp(path, "..")) return (0);
	if(lstat(path, pstat) == 0) { 
		return ((S_ISDIR(pstat->st_mode)) ? 1 : 0 );
	}
	return(0);
} 

/* Check if the file passed to it is readable, regular, and exists */
int isReadableFile(char *path) {
	struct stat sstat, *pstat = &sstat;
	if(lstat(path, pstat) == 0 && (S_ISREG(pstat->st_mode)) && !(isDirectory(path))) {
		return (pstat->st_mode & S_IRUSR);
	}
	return (0);
} 

/*Check if the file passed to it is NOT readable, regular, and exists*/
int isNonReadableFile(char *path) {
	struct stat sstat, *pstat = &sstat;
	if(lstat(path, pstat) == 0 && (S_ISREG(pstat->st_mode)) && !(isDirectory(path))) {
		return(!(pstat->st_mode & S_IRUSR));
	}
	return(0);
}
