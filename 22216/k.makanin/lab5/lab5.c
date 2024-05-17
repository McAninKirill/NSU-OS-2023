#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct line {
    off_t offset;
    off_t len;
} line;

int initLine(line **lines){
    *lines = (line*) malloc(sizeof(line));
    if (*lines == NULL)
    {
        perror("Creating malloc failed");
        return -1;
    }
    return 1;
}

int addMemory(int *extension, int numberOfLine, line **lines) {
    if ((*extension) <= numberOfLine) {
        (*extension) *= 2;
        *lines = realloc(*lines, (*extension) * sizeof(line));
        if (*lines == NULL) {
            perror("Realloc failed");
            return -1;
        }
    }
    return 1;
}


int printLine(line *lines, int fd, int choice) {
    off_t lenStr = lines[choice - 1].len, specialOff = 0;
    char strOut[BUFSIZ];
    while (lenStr - specialOff > BUFSIZ){
        if (lseek(fd, lines[choice - 1].offset + specialOff, SEEK_SET) != lines[choice - 1].offset + specialOff) {
            perror("Not correct number of bytes.");
            return -1;
        }
        if (read(fd, strOut, BUFSIZ) != BUFSIZ) {
            perror("Cannot read line.");
            return -1;
        }

        if (fwrite(strOut, 1, BUFSIZ, stdout) != BUFSIZ){
            perror("Cannot write line.");
            return -1;
        }
        specialOff += BUFSIZ;
    }

    if (lenStr - specialOff > 0){
        char lineOut[lenStr - specialOff];
        if (lseek(fd, lines[choice - 1].offset + specialOff, SEEK_SET) != lines[choice - 1].offset + specialOff) {
            perror("Not correct number of bytes.");
            return -1;
        }

        if (read(fd, lineOut, lenStr - specialOff) != lenStr - specialOff) {
            perror("Cannot read line.");
            return -1;
        }
        
        if (fwrite(strOut, 1, lenStr - specialOff, stdout) != lenStr - specialOff){
            perror("Cannot write line.");
            return -1;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "No filename.\n");
        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Opening failed.");
        return -1;
    }

    off_t lenStr = 0,
        previousLen = 0, 
        current = 0;

    int extension = 1, numberOfLine = 0, choice = 1;  

    char buf[BUFSIZ];

    line *lines;
    if (initLine(&lines) == -1){
        return -1;
    }

    size_t amountBytes;

    amountBytes = read(fd, buf, BUFSIZ);

    while (amountBytes > 0){
        if (buf[current] == '\n'){
            if (addMemory(&extension, numberOfLine, &lines) == -1){
                return -1;
            }
            lines[numberOfLine].offset = previousLen;
            lines[numberOfLine].len = lenStr + 1;
            previousLen += lenStr + 1;
            lenStr = 0;
            numberOfLine++;
            amountBytes--;
            current++;
        } else {
            lenStr++;
            current++;
            amountBytes--;
        }
        if (amountBytes == 0){
            amountBytes = read(fd, buf, BUFSIZ);
            if (amountBytes == 0 && lenStr > 0){
                if (addMemory(&extension, numberOfLine, &lines) == -1){
                    return -1;
                }
                lines[numberOfLine].offset = previousLen;
                lines[numberOfLine].len = lenStr;
                
                numberOfLine++;
                break;
            }
            current = 0;
        }

    }

    while (choice != 0) {
        scanf("%d", &choice);
        while (choice < 0 || choice > numberOfLine) {
            fprintf(stderr, "Incorrect number.\nTry again: ");
            scanf("%d", &choice);
        }
        if (choice == 0) {
            break;
        } else {
            if (printLine(lines, fd, choice) == -1){
                return -1;
            }
        }
    }

    free(lines);
    close(fd);

    return 0;
}