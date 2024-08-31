#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Failed to execute pipe.");
        return -1;
    }

    pid_t child_pid = fork(); 

    if (child_pid == -1) {
        perror("Failed to fork.");
        return -1;
    }

    if (child_pid == 0) {
        close(fd[0]);
        char string1[13] = "Hello, World.";
        ssize_t write_bytes;

        if ((write_bytes = write(fd[1], string1, 13)) == -1) {
            perror("Failed to write.");
            close(fd[1]);
            return -1;
        }

        close(fd[1]);
    } 

    if (child_pid > 0) {
        close(fd[1]);
        char string2[13];

        if (read(fd[0], string2, 13) == -1) {
            perror("Failed to read.");
            close(fd[0]);
            return -1;
        }

        for (int i = 0; i < 13; i++){
            printf("%c", toupper(string2[i]));
        }

        close(fd[0]);
        return 0;
    }
}