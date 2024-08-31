#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
    uid_t r_uid = getuid();
    uid_t e_uid = geteuid();

    printf("Real uid: %u\nEffective uid: %u\n", r_uid, e_uid);
    FILE *file = fopen("file", "r");
    
    if (file != NULL) {
        fclose(file);
    } else {
        perror("Failed to fopen");
        return -1;
    }

    if (setuid(r_uid) == -1) {
        perror("Failed to set");
        return -1;
    }

    uid_t new_e_uid = geteuid();

    printf("Real uid: %u\nEffective uid: %u\n", r_uid, new_e_uid);
    file = fopen("file", "r");

    if (file != NULL) {
        fclose(file);
    } else {
        perror("Failed to fopen");
        return -1;
    }

    return 0;
}