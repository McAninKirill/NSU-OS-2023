#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No arguments.\n");
        return -1;
    }
    
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Failed to fork.");
        return -1;
    }

    if (child_pid == 0) {
        execvp(argv[1], argv + 1);
        perror("Failed to execute execvp.");
        return -1;
    } 

    if (child_pid > 0) {
        int wstatus;

        if (wait(&wstatus) == -1) {
            perror("Failed to execute wait.");
            return -1;
        }

        if (WIFEXITED(wstatus)) {
            printf("Child process was terminated normally by calling exit with status: %d.\n", WEXITSTATUS(wstatus));
        } else {
            printf("Child process was terminated not by calling exit, but by signal: %d.\n", WTERMSIG(wstatus));
        }
    }
    
    return 0;
}
