#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "No text file.\n");
        return -1;
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Failed to fork");
        return -1;
    }

    if (child_pid == 0) {
        execl("/usr/bin/cat", "cat", argv[1], (char *) 0);
        perror("Failed to execute cat");
        return -1;
    }

    if (child_pid > 0) {
        int wstatus;

        if (wait(&wstatus) == -1) {
            perror("Failed to execute wait");
            return -1;
        }

        if (!WIFEXITED(wstatus)) {
            fprintf(stderr, "Child process was terminated not by calling exit.\n");
        }

        printf("\nSome text from parent process.\n");
    }
    
    return 0;
}
