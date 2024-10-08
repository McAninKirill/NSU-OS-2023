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
        char string1[BUFSIZ];
        ssize_t bytes_size1;

        while((bytes_size1 = read(0, string1, BUFSIZ))) {
            if (write(fd[1], string1, bytes_size1) == -1) {
                perror("Failed to write.");
                close(fd[1]);
                return -1;
            }
        }

        if (bytes_size1 == -1) {
            perror("Failed to read.");
            close(fd[1]);
            return -1;
        }

        close(fd[1]);
    } 

    if (child_pid > 0) {
        close(fd[1]);
        char string2[BUFSIZ];
        ssize_t bytes_size2;

        while((bytes_size2 = read(fd[0], string2, BUFSIZ))) {
            for (int i = 0; i < bytes_size2; i++){
            putchar(toupper(string2[i]));
        }
        }

        if (bytes_size2 == -1) {
            perror("Failed to read.");
            close(fd[0]);
            return -1;
        }

        close(fd[0]);
        return 0;
    }
}

