#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

<<<<<<< HEAD
#define SIZE_OF_BUFFER 1024


typedef struct line {
    int offset;
    int len;
} line;


void addMemory(int *extension, int numberOfLine, line *temp, line *lines) {
    if ((*extension) < numberOfLine){
        (*extension) *= 2;
        temp = realloc(lines, (*extension) * sizeof(line));
        if (temp  != NULL){
            lines = temp;
        }
        else{
            perror("Realloc failed");
        }
    }
}


void printLine(line *lines, int fd, int choice) {
    int lenStr = lines[choice - 1].len;
    char strOut[lenStr];
    if (lseek(fd, lines[choice - 1].offset, SEEK_SET) != lines[choice - 1].offset) {
        perror("Not correct number of bytes.");
    };
    if (read(fd, strOut, lenStr) == -1){
        perror("Cannot read line.");
    };
    printf("%.*s\n", lenStr, strOut);
}


int main() {

    int fd = open("./text.txt", O_RDONLY);
    if (fd == -1)
    {
=======
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
    size_t lenStr = lines[choice - 1].len, specialOff = 0;
    char strOut[BUFSIZ];
    while (lenStr - specialOff > BUFSIZ){
        if (lseek(fd, lines[choice - 1].offset + specialOff, SEEK_SET) != lines[choice - 1].offset) {
            perror("Not correct number of bytes.");
            return -1;
        };
        if (read(fd, strOut, BUFSIZ) != BUFSIZ) {
            perror("Cannot read line.");
            return -1;
        };
        if (fwrite(strOut, 1, BUFSIZ, stdout) != BUFSIZ){
            perror("Cannot write line.");  
            return -1;
        };
        specialOff += BUFSIZ;
    }
    
    if (lenStr - specialOff > 0){
        char lineOut[lenStr];
        if (lseek(fd, lines[choice - 1].offset + specialOff, SEEK_SET) != lines[choice - 1].offset + specialOff) {
            perror("Not correct number of bytes.");
            return -1;
        };
        if (read(fd, lineOut, lenStr - specialOff) == -1) {
            perror("Cannot read line.");
            return -1;
        };
        if (fwrite(lineOut, 1, lenStr - specialOff, stdout) != lenStr - specialOff){
            perror("Cannot write line.");
            return -1;
        };
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
>>>>>>> 2bdc01572bd1b7ae77d0bf6422639ccb2edd9ec9
        perror("Opening failed.");
        return -1;
    }
    
<<<<<<< HEAD
    int lenStr = 0,
        posInBuffer = 0,
        numberOfLine = 0,
        extension = 1,
        previousLen = 0;
    
    char choice[100];
    
    char buffer[SIZE_OF_BUFFER];

    line *temp = NULL;
    line *lines;
    lines = (line*) malloc(sizeof(line));
    if ( lines == NULL)
    {
        perror("Creating malloc failed");
        return -1;
    }
    
    
    ssize_t bytesRead;
    bytesRead = read(fd, &buffer, SIZE_OF_BUFFER);
    

    while (bytesRead > 0)
    {
        while (buffer[posInBuffer] != '\n' && bytesRead > 0)
        {
            bytesRead--;
            lenStr++;
            posInBuffer++;
        }
        
        if (buffer[posInBuffer] == '\n')
        {
            addMemory(&extension, numberOfLine, temp, lines);
            lines[numberOfLine].offset = previousLen + posInBuffer - lenStr;
            lines[numberOfLine].len = lenStr;

            lenStr = 0;
            numberOfLine++;
            posInBuffer++;
            bytesRead--;
        }
        
        if (bytesRead == 0)
        {
            previousLen += posInBuffer;
            posInBuffer = 0;
            bytesRead = read(fd, &buffer, SIZE_OF_BUFFER);
        }
    }

    
    do
    {
        printf("Choose the line:\n");
        scanf("%s", choice);
        while (atol(choice) < 0 || atol(choice) == 0 && choice[0] != '0'){
            printf("You need choose a positive number:\n");
            scanf("%s", choice);
        }
        
        if (choice[0] == '0'){
            printf("End of work.");
            break;
        }
        
        if (atol(choice) > numberOfLine){
            printf("There are fewer lines in the file.\n");
        }
        else{
            printLine(lines, fd, atol(choice));
        }
    }
    while (choice != 0);

    
    free(lines);
    free(temp);
    close(fd);
}
=======
    off_t lenStr = 0,
        previousLen = 0, 
        current = 0;
        
    int extension = 1, numberOfLine = 0, choice;  
    
    char buf[BUFSIZ];
    
    line *lines;
    if (initLine(&lines) == -1){
        return -1;
    };
    
    size_t amountBytes;
    
    amountBytes = read(fd, buf, BUFSIZ);
     
    while (amountBytes > 0){
        if (buf[current] == '\n'){
            if (addMemory(&extension, numberOfLine, &lines) == -1){
                return -1;
            };
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
                };
                lines[numberOfLine].offset = previousLen;
                lines[numberOfLine].len = lenStr + 1;
                break;
            }
            current = 0;
        }
        
    }
    
    do {
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
            };
        }
    } while (choice != 0);

    free(lines);
    close(fd);
    
    return 0;
}
>>>>>>> 2bdc01572bd1b7ae77d0bf6422639ccb2edd9ec9
