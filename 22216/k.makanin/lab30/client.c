#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

char *sock_path = "./socket";

int main() {
    struct sockaddr_un sock_addr;

    ssize_t bytes_size;

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("Failed to execute socket.");
        return -1;
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strncpy(sock_addr.sun_path, sock_path, sizeof(sock_addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
        perror("Failed to connect.");
        return -1;
    }

    
    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));

    while ((bytes_size = read(0, buffer, BUFSIZ)) > 0) {

        if (write(sock, buffer, bytes_size) == -1) {
            perror("Failed to write.");
            return -1;
        }
    }

    if (bytes_size == -1) {
            perror("Failed to read.");
            return -1;
        }

    close(sock);

    return 0;
}
