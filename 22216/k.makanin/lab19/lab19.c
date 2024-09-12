#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fnmatch.h>
#include <errno.h>

int main(int argc, char** argv) {
    DIR* dir;
    struct dirent* entry;
    int amount = 0;
    errno = 0;

    if (argc != 2) {
	fprintf(stderr, "No pattern.\n");
	return -1;
    }

    if (strchr(argv[1], '/') != NULL) {
	fprintf(stderr, "Pattern can not contain /\n");
	return -1;
    }
    
    dir = opendir(".");
    if (!dir) {
	perror("Failed to execute opendir.");
	return -1;
    }

    entry = readdir(dir);

    while (entry != NULL) {
	if (fnmatch(argv[1], entry->d_name, FNM_PATHNAME) == 0) {
            printf("%s\n", entry->d_name);
	    amount++;
	}
        entry = readdir(dir);
    }

    if (errno) {
        perror("Failed to execute readdir.");
        return -1;
    }

    if (amount == 0) {
	printf("Can not match files such pattern: %s\n", argv[1]);
    }
    closedir(dir);

    return 0;
}
